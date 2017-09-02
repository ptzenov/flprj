/*
 * common.hpp
 *
 *  Created on: 26 May 2014
 *      Author: kenny
 */

#ifndef COMMON_HPP_
#define COMMON_HPP_

//// string management headers
#include <iostream>
#include <istream>
#include <string>
#include <time.h>
#include <math.h>
// some useful containers
#include <list>
#include <vector>
#include <deque>




// good old standard library stuffs
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include <omp.h> // include a little of openmp
// openGL headers

#include <GL/glew.h> // include glew before the otehrs!

#include <GL/glut.h>
#include <GL/gl.h>

//a nice library thet provides high resolution clock
#include <papi.h>


// define the thread types...

#define THREAD_SIMULATION 0
#define THREAD_USER_IO 1

#define ROT_PLANE_XY 2 // corresponds to 0 0 1
#define ROT_PLANE_XZ 1 // corresponds to 0 1 0
#define ROT_PLANE_YZ 0 // corresponds to 1 0 0

// constants specifying the possible trajectory types
#define TRAJ_TYPE_LINEAR 0
#define TRAJ_TYPE_CIRCULAR 1
#define TRAJ_TYPE_HELICAL 2
#define TRAJ_TYPE_SEQUENTIAL 3


// constant specifying the current trajectory selection
extern unsigned int CURRENT_TRAJ_TYPE;
extern unsigned int CURRENT_ROTATION_PLANE;
extern unsigned int CURRENT_ACC_SELECTOR;
extern unsigned int CURRENT_VEL_SELECTOR;

//extern float ACCELERATION[3][2]; // ={0.0f,0.0f};
//extern float VELOCITY[3][2]; // = {0.0f,0.0f};DS MACRO DEFINITION
extern float ACCELERATION[2]; // ={0.0f,0.0f};
extern float VELOCITY[2]; // = {0.0f,0.0f};DS MACRO DEFINITION


/// this macro here specifies the total number of commands currently supported
#define NR_CMDS 32
//tell the compiler that the commands list will be defined somewhere else
extern char* commands[NR_CMDS];
//current position vector
extern float x[3];
extern float target[4];
// motion direction vector
extern float direction[3];
//vector specifying the current velocity
extern float current_velocity[3];
//vector specifying the current direction of acceleration
extern float current_acceleration[3];
extern float xyz_min[3];
extern float xyz_max[3];


extern bool start_simulation;
extern float dt; //timestep;
extern float t; // global time...
extern int tid;

#define MAX_POINTS 5000
extern GLfloat trajectory[MAX_POINTS][3];
extern int pt_ctr_;
extern int first_;
// command name --> comand idx in the commands array!!!
#define SET_MOVE_TYPE_cmd 0  // takes the number 0 1 2 or 3
#define MOVE_cmd 1
#define STEP_RELATIVE_X_cmd 2  // changes the velocity vector
#define STEP_RELATIVE_Y_cmd 3
#define STEP_RELATIVE_Z_cmd 4
#define START_CONTINUOUS_LINEAR_MOVE_X_cmd 5 // move continuously in x direction with velocity = current velocity
#define STOP_CONTINUOUS_LINEAR_MOVE_X_cmd 6
#define START_CONTINUOUS_LINEAR_MOVE_Y_cmd 7
#define STOP_CONTINUOUS_LINEAR_MOVE_Y_cmd 8
#define START_CONTINUOUS_LINEAR_MOVE_Z_cmd 9
#define STOP_CONTINUOUS_LINEAR_MOVE_Z_cmd 10
#define SET_POSITION_X_cmd 11
#define SET_POSITION_Y_cmd 12
#define SET_POSITION_Z_cmd 13
#define SET_VELOCITY_1_cmd 14
#define SET_VELOCITY_2_cmd 15
#define SET_VELOCITY_SELECTOR_TO_1_cmd 16
#define SET_VELOCTIY_SELECTOR_TO_2_cmd 17
#define SET_ACCELERATION_1_cmd 18
#define SET_ACCELERATION_2_cmd 19
#define SET_ACCELERATION_SELECTOR_TO_1_cmd 20
#define SET_ACCELERATION_SELECTOR_TO_2_cmd 21


// pointer to the control loop of the program...
extern void exec_control_loop();

#endif /* COMMON_HPP_ */
