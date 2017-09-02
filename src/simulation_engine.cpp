/*
 * command_executioner.cpp
 *
 *  Created on: 26 May 2014
 *      Author: kenny
 */

#include "simulation_engine.hpp"
#include "common.hpp"

unsigned int CURRENT_TRAJ_TYPE = TRAJ_TYPE_LINEAR;
unsigned int CURRENT_ROTATION_PLANE =  ROT_PLANE_XY; // set default plane of rotation to be xy!
unsigned int CURRENT_ACC_SELECTOR = 0;
unsigned int CURRENT_VEL_SELECTOR = 0;

//float ACCELERATION[3][2];
//float VELOCITY[3][2];
float ACCELERATION[2] = { 0.0f, 0.0f };
float VELOCITY[2] = { 1.0f, 0.0f };

GLfloat trajectory[MAX_POINTS][3];
int pt_ctr_ = 0;
int dimension_toggle = 0;

//bool start_simulation = false;
//
//vector specifying the current direction of motion
float direction[3] = { 1.0f, 1.0f, 0.0f };
//vector specifying the current velocity
float current_velocity[3] = { 0.0f, 0.0f, 0.0f };
//vector specifying the current direction of acceleration
float current_acceleration[3] = { 0.0f, 0.0f, 0.f };

//vectors specifying the min and max dimensions of the x
float xyz_min[3] = { -5, -5, -5 };
float xyz_max[3] = { +5, +5, +5 };

//vector specifying the current position
float x[3] = { 0.0f, 0.0f, 0.0f };
// this is the motion target vector!!! Notice that it is a 4-vector, initialized with 0 at the end
// this means that targeted motion is DEACTIVATED!!! Set to a value different than 0 and this will activate the checks
// if target is reached.
// the directed motion towards the target
float target[4] = { 0.0f, 0.0f, 0.0f, 0.0f };

// special params for spherical and helical motion
// n_vect: vector perpendicular to the plane of rotation
float n_vect[3] = { 0.0f, 0.0f, 1.0f };
// t_vect -> first basis vector for the plane of rotation
float t_vect[3] = { 0.0f, 0.0f, 0.0f };
//b_vect --> second basis vector for the plane of rotation: b = n x t (so that t,b,n form a right handed coordinate system)
float b_vect[3] = { 0.0f, 0.f, 0.f };
// central point of rotation
float c_0[3] = { 0.0f, 0.0f, 0.0f };
// rotation angle, angular velocity and acceleration...
float theta = 0;
float angular_velocity = 0.0f;
float angular_acceleration = 0.0f;

//this is the axial velocity for the helical motion...
float axial_vel = 0.0;
float arch_angle = M_PI; // this is the variable specifying the arch angle for execution of the go_to command along circular trajectory.
// default is 180 degrees. Possible values are arch_angle (15; 180] degrees ( set 15 as the minimum since when arch angle is 0 then the
// Radius of the trajectory becomes infinity...

// helical motion vector in the t,b,n basis... start with R 0 0 !
float v_helix[3];
float radius = 0;
bool start_simulation = false;

//vector opeartions
float normalize_vector(float _n[3]) {

	float norm = 0;
	for (int d = 0; d < 3; d++)
		norm += _n[d] * _n[d];
	norm = sqrt(norm);
	// unless we are dealing with an empty vector...
	if (norm != 0)
		for (int d = 0; d < 3; d++)
			_n[d] /= norm;

	return norm;

}

float scalar_product(float _x[3], float _y[3]) {
	float sum = 0;

	for (int d = 0; d < 3; d++)
		sum += _x[d] * _y[d];

	return sum;
}

// vector operations
void vector_product(const float _n[3], const float _t[3], float _b[3]) {

	_b[0] = _n[1] * _t[2] - _n[2] * _t[1];
	_b[1] = _n[2] * _t[0] - _n[0] * _t[2];
	_b[2] = _n[0] * _t[1] - _n[1] * _t[0];

}

void move() {

	pt_ctr_ = pt_ctr_ + 1;
	if (pt_ctr_ == MAX_POINTS)
		pt_ctr_ = 0;

	trajectory[pt_ctr_][0] = x[0];
	trajectory[pt_ctr_][1] = x[1];
	trajectory[pt_ctr_][2] = x[2];

	if (CURRENT_TRAJ_TYPE == TRAJ_TYPE_LINEAR)
		if (target[3] != 0)
			go_to_linear_motion();
		else
			execute_linear_motion();

	// move x first then y then z ...
	if (CURRENT_TRAJ_TYPE == TRAJ_TYPE_SEQUENTIAL)
		if (target[3] != 0)
			go_to_sequential_motion();
		else
			execute_sequential_motion();

	if (CURRENT_TRAJ_TYPE == TRAJ_TYPE_CIRCULAR)
		if (target[3] != 0)
			go_to_circular_motion();
		else
			execute_circular_motion();

	if (CURRENT_TRAJ_TYPE == TRAJ_TYPE_HELICAL)
		if (target[3] != 0)
			go_to_helical_motion();
		else
			execute_helical_motion();

}

void stop_motion() {

	for (int d = 0; d < 3; d++) {
		current_velocity[d] = 0;
		current_acceleration[d] = 0;
		direction[d] = 0;
	}
}

void go_to_linear_motion() {

	float x_new[3];
	for (int d = 0; d < 3; d++)
		x_new[d] = x[d] + VELOCITY[CURRENT_VEL_SELECTOR] * dt * direction[d]
				+ 0.5 * ACCELERATION[CURRENT_ACC_SELECTOR] * dt * dt
						* direction[d];
	// if targeted moton is activated
	// check whether we are aoubt to jump over the target!!!
	// if we are about to overshoot (i.e. jumped over) the target, stop where we are!
	// else proceed the motion as usual.

	bool target_reached = check_target_reached(x, x_new, target);
	if (target_reached) { // if we still have some time to go proceed as usual
		stop_motion();
	} else { //else set the current coordinate to be x[d];
		execute_linear_motion();
	}

}


void execute_linear_motion() {

	float x_new[3];
	// calculate the new position
	for (int d = 0; d < 3; d++)
		x_new[d] = x[d] + VELOCITY[CURRENT_VEL_SELECTOR] * dt * direction[d]
				+ 0.5 * ACCELERATION[CURRENT_ACC_SELECTOR] * dt * dt
						* direction[d];

	// check if you have reached the boundary
	if(check_bdry_reached(x_new)){
		stop_motion();
		return;
	}

	// if we are not executing targeted linear move proceed with the usual motion !!!
	for (int d = 0; d < 3; d++) {
		x[d] = x_new[d];
		//change the current velocity and current acceleration to reflect the current state of moiton
		current_velocity[d] = direction[d] * VELOCITY[CURRENT_VEL_SELECTOR];
		current_acceleration[d] = direction[d]
				* ACCELERATION[CURRENT_VEL_SELECTOR];
	}
	// increase the velocity a bit.
	for (int d = 0; d < 3; d++) {
		VELOCITY[CURRENT_VEL_SELECTOR] += ACCELERATION[CURRENT_ACC_SELECTOR]
				* dt;
	}

}

void go_to_sequential_motion() {


}

void execute_sequential_motion() {

//	x[dimension_toggle] += VELOCITY[CURRENT_VEL_SELECTOR] * dt
//			+ 0.5 * ACCELERATION[CURRENT_ACC_SELECTOR] * dt * dt;
//	// change the current velocity and acceleration vectors to point in the correct direction
//	for (int d = 0; d < 3; d++) {
//		if (d == dimension_toggle) {
//			current_velocity[d] = VELOCITY[CURRENT_VEL_SELECTOR];
//			current_acceleration[d] = ACCELERATION[CURRENT_VEL_SELECTOR];
//		} else {
//			current_velocity[d] = 0;
//			current_acceleration[d] = 0;
//		}
//	}
//
//	// set the index to the next coordinate 0 -> x 1 -> y 2 -> z
//	dimension_toggle += 1;
//	dimension_toggle %= 3;
//
//	// increase the velocity a bit.
//	VELOCITY[CURRENT_VEL_SELECTOR] = VELOCITY[CURRENT_VEL_SELECTOR]
//			+ ACCELERATION[CURRENT_ACC_SELECTOR] * dt;

}

void go_to_circular_motion() {

	//predict the next step
	float x_new[3];
	for (int d = 0; d < 3; d++) {
		x_new[d] = c_0[d]
				+ radius * (t_vect[d] * cos(theta) + b_vect[d] * sin(theta));
	}

	bool target_reached = check_target_reached(x, x_new, target);
	if (target_reached) {
		stop_motion();
	} else {
		// proceed with sequential motion as usual
		execute_circular_motion();
	}

}

void execute_circular_motion() {

	float x_new[3];

	//execute motion
	for (int d = 0; d < 3; d++) {
		x_new[d] = c_0[d]
				+ radius * (t_vect[d] * cos(theta) + b_vect[d] * sin(theta));
	}


	// check if you have reached the boundary
	if(check_bdry_reached(x_new)){
		stop_motion();
		return;
	}

	// calculate the tangential vector to the circular orbit, i.e. "current direction"...
	for (int d = 0; d < 3; d++) {
		x[d] = x_new[d];
		direction[d] = (-t_vect[d] * sin(theta) + b_vect[d] * cos(theta));
	}
	normalize_vector(direction);
	// calculate the tangential acceleration and tangential velocity...
	float tangential_vel = angular_velocity * radius;
	float tangential_acc = angular_acceleration * radius;
	//set the current velocity to be a vector pointing in the direction of motion
	for (int d = 0; d < 3; d++) {
		{
			current_velocity[d] = tangential_vel * direction[d];
			current_acceleration[d] = tangential_acc * direction[d];
		}
	}

	// update the angle and increase the angular velocity.
	theta = theta + dt * angular_velocity
			+ 0.5 * dt * dt * angular_acceleration;
	angular_velocity += dt * angular_acceleration;

}
void go_to_helical_motion() {


	//predict the next step
	v_helix[0] = radius * cos(theta);
	v_helix[1] = radius * sin(theta);
	v_helix[2] += axial_vel * dt;

	float x_new[3];
	//transform from the local coordinate system to the global coordinate system :)
	x_new[0] = c_0[0] + t_vect[0] * v_helix[0] + b_vect[0] * v_helix[1]
			+ n_vect[0] * v_helix[2];
	x_new[1] = c_0[1] + t_vect[1] * v_helix[0] + b_vect[1] * v_helix[1]
			+ n_vect[1] * v_helix[2];
	x_new[2] = c_0[2] + t_vect[2] * v_helix[0] + b_vect[2] * v_helix[1]
			+ n_vect[2] * v_helix[2];


	bool target_reached = check_target_reached(x, x_new, target);
	if (target_reached) {
		stop_motion();
	} else {
		// proceed with sequential motion as usual
		execute_circular_motion();
	}



}

void execute_helical_motion() {

	v_helix[0] = radius * cos(theta);
	v_helix[1] = radius * sin(theta);
	v_helix[2] += axial_vel * dt;

	float x_new[3];
	//transform from the local coordinate system to the global coordinate system :)
	x_new[0] = c_0[0] + t_vect[0] * v_helix[0] + b_vect[0] * v_helix[1]
			+ n_vect[0] * v_helix[2];
	x_new[1] = c_0[1] + t_vect[1] * v_helix[0] + b_vect[1] * v_helix[1]
			+ n_vect[1] * v_helix[2];
	x_new[2] = c_0[2] + t_vect[2] * v_helix[0] + b_vect[2] * v_helix[1]
			+ n_vect[2] * v_helix[2];

	if(check_bdry_reached(x_new)){
		stop_motion();
		return;
	}
	//else make the move!!!
	for(int d = 0 ; d< 3; d++)
		x[d] = x_new[d];


	theta = theta + dt * angular_velocity
			+ 0.5 * dt * dt * angular_acceleration;
	angular_velocity += dt * angular_acceleration;

	// now calculate the tangential direction of the motion.
	//TODO TODO TODO !!!!



}

bool check_bdry_reached(float _x[3]){

	bool bdry_reached = false;

	/// check if the ball has reached the boundary...
	for (int d = 0; d < 3; d++) {
		if (_x[d] >= xyz_max[d] || _x[d] <= xyz_min[d]) {
			bdry_reached = true;
			break;
		}
	}

	return bdry_reached;

}

bool check_target_reached(float _x[3], float _x_new[3], float _target[3]) {

	float sprod = 0; // scalar prod of target-x and target-xnew
	for (int d = 0; d < 3; d++)
		sprod += (_target[d] - _x[d]) * (target[d] - _x_new[d]);

	return sprod < 0;

}

/// parse the commands....

void execute_command(const int command_idx, char* command_values[3]) {

#pragma omp critical
	{

		switch (command_idx) {
		case 0: { //SET_MOVE_TYPE_cmd 0
			if (!strcmp(command_values[0], "linear"))
				CURRENT_TRAJ_TYPE = TRAJ_TYPE_LINEAR;
			if (!strcmp(command_values[0], "circular")) {
				CURRENT_TRAJ_TYPE = TRAJ_TYPE_CIRCULAR;
				theta = 0;
				target[3] = 0;
			}
			if (!strcmp(command_values[0], "helical")) {
				CURRENT_TRAJ_TYPE = TRAJ_TYPE_HELICAL;
				theta = 0;
				v_helix[2] = 0;
				target[3] = 0;
			}

			if (!strcmp(command_values[0], "sequential")) {
				CURRENT_TRAJ_TYPE = TRAJ_TYPE_SEQUENTIAL;
				dimension_toggle = 0;
				target[3] = 0;
			}
			break;
		}
		case 1: { //MOVE_cmd 1
			start_simulation = true;
			break;
		}

		case 2: { // pause
			start_simulation = false;
			break;

		}
		case 3: { // resume

			start_simulation = true;
			break;
		}

		case 4: { //set_linear_move_direction
			for (int d = 0; d < 3; d++) {
				if (strcmp(command_values[d], "_"))
					direction[d] = atof(command_values[d]);
			}
			break;
		}
		case 5: { //START_CONTINUOUS_LINEAR_MOVE_X_cmd 5
			direction[0] = 1.0f;
			direction[1] = 0.0f;
			direction[2] = 0.0f;
			target[3] = 0;
			break;
		}
		case 6: { //STOP_CONTINUOUS_LINEAR_MOVE_X_cmd 6
			direction[0] = 0.0f;
			break;
		}
		case 7: { //START_CONTINUOUS_LINEAR_MOVE_Y_cmd 7
			direction[0] = 0.0f;
			direction[1] = 1.0f;
			direction[2] = 0.0f;
			target[3] = 0;
			break;
		}
		case 8: { //STOP_CONTINUOUS_LINEAR_MOVE_Y_cmd 8
			direction[1] = 0.0f;
			break;
		}
		case 9: { //START_CONTINUOUS_LINEAR_MOVE_Z_cmd 9
			direction[0] = 0.0f;
			direction[1] = 0.0f;
			direction[2] = 1.0f;
			target[3] = 0;
			break;
		}
		case 10: { //STOP_CONTINUOUS_LINEAR_MOVE_Z_cmd 10
			direction[2] = 0.0f;
			break;
		}
		case 11: { //SET_POSITION_X_cmd 11
			x[0] = atof(command_values[0]);
			break;
		}
		case 12: { //SET_POSITION_Y_cmd 12
			x[1] = atof(command_values[0]);
			break;
		}
		case 13: { //SET_POSITION_Z_cmd 13
			x[2] = atof(command_values[0]);
			break;
		}

		case 14: { //SET_VELOCITY_1_cmd 14
			if (strcmp(command_values[0], "_"))
				VELOCITY[0] = atof(command_values[0]);

			break;
		}
		case 15: { //SET_VELOCITY_2_cmd 15
			if (strcmp(command_values[0], "_"))
				VELOCITY[1] = atof(command_values[0]);

			break;
		}
		case 16: { //SET_VELOCITY_SELECTOR_TO_1_cmd 16
			CURRENT_VEL_SELECTOR = 0;
			break;
		}
		case 17: { //SET_VELOCTIY_SELECTOR_TO_2_cmd 17
			CURRENT_VEL_SELECTOR = 1;
			break;
		}
		case 18: { //SET_ACCELERATION_1_cmd 18

			if (strcmp(command_values[0], "_"))
				ACCELERATION[0] = atof(command_values[0]);

			break;
		}
		case 19: { //SET_ACCELERATION_2_cmd 19

			if (strcmp(command_values[0], "_"))
				ACCELERATION[1] = atof(command_values[0]);

			break;
		}
		case 20: { //SET_ACCELERATION_SELECTOR_TO_1_cmd 20
			CURRENT_ACC_SELECTOR = 0;
			break;
		}
		case 21: { //SET_ACCELERATION_SELECTOR_TO_2_cmd 21
			CURRENT_ACC_SELECTOR = 1;
			break;
		}
		case 22: // set_rotation_axis;
		{
			for (int d = 0; d < 3; d++) {
				if (strcmp(command_values[d], "_"))
					n_vect[d] = atof(command_values[d]);
			}
			reinit_vector_triplet();
			//as the helical/circular motion will have to restart, set theta = 0 and v_helix = 0;
			theta = 0;
			v_helix[2] = 0;
			break;
		}

		case 23: // set_center_of_rotation
		{
			for (int d = 0; d < 3; d++) {
				if (strcmp(command_values[d], "_"))
					c_0[d] = atof(command_values[d]);
			}
			//reinitialize the local coordinate system of the rotation
			reinit_vector_triplet();
			//as the helical/circular motion will have to restart, set theta = 0 and v_helix = 0;
			theta = 0;
			v_helix[2] = 0;
			break;
		}
		case 24: { //set_angular_velocity
			if (strcmp(command_values[0], "_"))
				angular_velocity = (float) atof(command_values[0]);
			break;
		}
		case 25: { // set_angular_acceleration
			if (strcmp(command_values[0], "_"))
				angular_acceleration = (float) atof(command_values[0]);
			break;
		}
		case 26: { //set_axial_velocity
			if (strcmp(command_values[0], "_"))
				axial_vel = (float) atof(command_values[0]);
			break;
		}
		case 27:{

			//"go_to",
			for (int d = 0; d < 3; d++) {
				if (strcmp(command_values[d], "_"))
					target[d] = atof(command_values[d]);
			}

			if (CURRENT_TRAJ_TYPE == TRAJ_TYPE_LINEAR)
				for (int d = 0; d < 3; d++)
					direction[d] = target[d] - x[d];

			if(CURRENT_TRAJ_TYPE == TRAJ_TYPE_CIRCULAR) {

				//setup the rotation axis to be perpendicular to the currently selected rotation plane!
				/*Mind that the motion might not always be in that plane!!! This WILL NOT be the case
				 * when the line connecting the current posiotion and the target position is not perpendicular
				 * to any of the coordinate axis, x,y,z.
				 *
				 * */
				n_vect[0]=0 ; n_vect[1] = 0 ;n_vect[2] = 0;
				n_vect[CURRENT_ROTATION_PLANE] = 1;

				/**
				 *  now to find the center of rotation c_0 as well as the coordinate basis triplet (t,b,n) we need to solve a linear
				 * 	system of equations Ax = b
				 *  Below I setup the matrix A and the r.h.s b and solve it using my own gaussian ellimination procedure (with pivoting)
				 *	Basically the system we have to solve is the following
				 *
				 *	arbitrary circular motion in 3D space can be represented as follows
				 *
				 *	x(theta) = c_0 + A*v(theta) -> where A is a coordinate transformation matrix from the normal carthesian coordinate basis, (1,0,0)
				 *	(0,1,0) (0,0,1) to the basis determined by the triplet (t,b,n), and v is the trajectory of circular motion in the xy plane
				 *	v(theta) = (Rcos(theta), Rsin(theta), 0)
				 *  given that we know the normal vector n, and the condition that n x t = b find c_0 and t such that the following
				 *  conditions are statisfied
				 *  x(0) = initial_position = c_0 + A*v(0)
				 *  x(arch_angle) = target_position = c_0 + A*v(arch_angle); -> these two conditions give us 6 equations and luckily we have
				 *  exactly 6 unknonws (3 for c_0 and 3 for t) ---> setup the system and solve it!!!
				 */

				//first calculate the radius of the circular motion with the formula... R = distance(target,x)/2*sin(arch_angle/2);
				float distance = 0;
				for(int d = 0; d< 3; d++)
					distance+= (target[d]-x[d])*(target[d]-x[d]);
				distance = sqrt(distance);

				radius = distance/(2*sin(arch_angle/2));
				// done! now setup the system ( note that we are not solving a 6x6 linear system, but rather 3x3 -->
				// if one does the math he/she will see that this suffices.
				//allocate space for the matrix A
				float A[9];
				// first row of A
				A[0] = radius*cos(arch_angle)-radius; A[1] = -radius*n_vect[2]*sin(arch_angle); A[2] = radius*n_vect[1]*sin(arch_angle);
				// second row of A
				A[3] = radius*n_vect[2]*sin(arch_angle); A[4] = radius*cos(arch_angle)-radius; A[5] = -radius*n_vect[0]*sin(arch_angle);
				//third row of A
				A[6] = -radius*n_vect[1]*sin(arch_angle); A[7] = radius*n_vect[0]*sin(arch_angle); A[8] = radius*cos(arch_angle)-radius;

				//setup the right hand side...
				float b[3];
				for(int d = 0; d< 3; d++){
					b[d] = target[d] - x[d];
				}

				gaussianElimination(A,b,t_vect,3);
				// now calculate the center c_0;
				for(int d = 0; d<3; d++)
					c_0[d] = x[d] - radius*t_vect[d];


//				normalize_vector(t_vect);
				vector_product(n_vect, t_vect, b_vect);
//				normalize_vector(b_vect); //not needed !!!b should already have norm 1!

				//reinitialize the local coordinate system of the rotation
				//reinit_vector_triplet();
				//as the helical/circular motion will have to restart, set theta = 0 and v_helix = 0;
				theta = 0;

			}
			if (CURRENT_TRAJ_TYPE == TRAJ_TYPE_HELICAL) {
				//set the rotation axis and the center of rotation!! also calculate the axial velocity and if needed the
				//angular velocity so that we are able to reach the target in some finite time. and in k number of fully completed
				// round abouts
			}

			//activate targeted motion!

			target[3] = 1;
			break;
		}

		case 28: { 	//"set_axis_max" // 29

			for (int d = 0; d < 3; d++) {
				if (strcmp(command_values[d], "_"))
					xyz_max[d] = atof(command_values[d]);
			}
			break;
		}

		case 29: { //"set_axis_min"// 30
			for (int d = 0; d < 3; d++) {
				if (strcmp(command_values[d], "_"))
					xyz_min[d] = atof(command_values[d]);
			}
			break;
		}

		case 30:{ 	//		set_rotation_plane
			if(strcmp(command_values[0],"xy") || strcmp(command_values[0],"yx"))
						CURRENT_ROTATION_PLANE = ROT_PLANE_XY;
			if(strcmp(command_values[0],"xz") || strcmp(command_values[0],"zx") )
						CURRENT_ROTATION_PLANE = ROT_PLANE_XZ;
			if(strcmp(command_values[0],"yz") || strcmp(command_values[0],"zy"))
						CURRENT_ROTATION_PLANE = ROT_PLANE_YZ;
			break;

		}
		case 31: {// set_arch_angle
			if (strcmp(command_values[0], "_"))
			{
				float tmp = atof(command_values[0]);

				// if the given value is outside of the allowed boundary, set the arch angle to default 180 degrees
				if(tmp < 15 || tmp > 180)
				{
					std::cout<<"\nWarning: The specified arch angle value is outside of the allowed domain [15;180] degrees. "
							"Reseting arch_angle to default value - 180 degrees!!!\n";
					tmp = 180;
				}
				arch_angle = M_PI*tmp/(180.0f); // convert the angle to radians...
			}

			break;
		}
		default:
			std::cout << "COMMAND EXECUTION FAILED: INVALID COMMADN NUMBER\n";
			break;
		}
		//finally normalize the direction vector...
		normalize_vector(direction);
	} // end of critical section
}

void rotate_x(float angle, float* vect) {

	float c = cos(angle);
	float s = sin(angle);
	float tmp1 = c * vect[1] - s * vect[2];
	float tmp2 = s * vect[1] + c * vect[2];
	vect[1] = tmp1;
	vect[2] = tmp2;
}

void rotate_y(float angle, float* vect) {
	float c = cos(angle);
	float s = sin(angle);
	float tmp0 = c * vect[0] + s * vect[2];
	float tmp2 = -s * vect[0] + c * vect[2];
	vect[0] = tmp0;
	vect[2] = tmp2;

}

void rotate_z(float angle, float*vect) {

	float c = cos(angle);
	float s = sin(angle);
	float tmp0 = c * vect[0] - s * vect[1];
	float tmp1 = s * vect[0] - c * vect[1];
	vect[0] = tmp0;
	vect[1] = tmp1;

}

void reinit_vector_triplet() {

	normalize_vector(n_vect);
	for (int d = 0; d < 3; d++)
		t_vect[d] = x[d] - c_0[d]; // t_vector does NOT lie on the plane determined by n_vect!!!!

	//take the radius!
	radius = normalize_vector(t_vect);
	vector_product(n_vect, t_vect, b_vect);
	normalize_vector(b_vect); //not needed !!!b should already have norm 1!

}

