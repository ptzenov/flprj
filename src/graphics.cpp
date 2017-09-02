/*
 * graphics.cpp
 *
 *  Created on: 26 May 2014
 *      Author: kenny
 */

#include "common.hpp"
#include "graphics.hpp"
#include "simulation_engine.hpp"
// queue for the point list...

#include <time.h>
#include <assert.h>

#define dist 0.5

int N_particles;
int DIMENSION;

int current_width = 400;
int current_height = 400;

GLfloat angle_x = 0;
GLfloat angle_y = 0;
GLfloat z_translate = -20.0f;
GLfloat x_translate = 0.0f;
GLfloat y_translate = 0.0f;

bool connect_points = true;

//GLuint vbo;
bool disp_text = true;
float t = 0;
float dt = 0.01;

void keyboardNavigator(int key, int x, int y) {

	switch (key) {
	case GLUT_KEY_DOWN: {
		angle_x += 5;
		if (angle_x > 360)
			angle_x -= 360;
		break;
	}

	case GLUT_KEY_UP: {
		angle_x -= 5;
		if (angle_x < -360)
			angle_x += 360;
		break;
	}

	case GLUT_KEY_LEFT: {

		angle_y -= 5;
		if (angle_y < -360)
			angle_y += 360;
		break;

	}

	case GLUT_KEY_RIGHT: {
		angle_y += 5;
		if (angle_y > 360)
			angle_y -= 360;
		break;
	}

	default: {
		// do nothing
		break;// at least ...
	}

	}

	glutPostRedisplay();
}

void handleKeypress(unsigned char key, int x_coord, int y_coord) {

	if (key == '+')
		z_translate += dist;
	if (key == '-')
		z_translate -= dist;

	if (key == 'a')
		x_translate += dist;
	if (key == 'd')
		x_translate -= dist;

	if (key == 's')
		y_translate += dist;
	if (key == 'w')
		y_translate -= dist;
	if (key == 'z')
		disp_text = !disp_text;
	if (key == 'f') {
		x_translate = -x[0];
		y_translate = -x[1];
		z_translate = -x[2] - 20.0;
		angle_x = 0;
		angle_y = 0;
	}

	if (key == 'p')
		connect_points = !connect_points;

	if (key == 'c') {
		x_translate = 0;
		y_translate = 0;
		z_translate = -20.0;
		angle_x = 0;
		angle_y = 0;
	}

	if (key == 'r')
		t = 0;
	glutPostRedisplay();

}

//Initializes 3D rendering
void initGraphics() {

	//OpenGL Stuff
	glEnable(GL_DEPTH_TEST);
//	glEnable(GL_TEXTURE_2D);
	glDepthFunc(GL_LEQUAL);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat mat_shininess[] = { 50.0 };
//	glMaterialfv(GL_FRONT,GL_SPECULAR,mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
	glShadeModel(GL_SMOOTH);

	// specify light position, light type and light color
	GLfloat diffuse_light_pos[] = { 0.0f, 5.0f, 10.0f, 0 }; // make the diffuse light source positional...
	GLfloat diffuse_light_col[] = { 1.5f, 1.5f, 1.0f, 0.0f }; // make the diffuse light color --> dark yellow
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse_light_col);
	glLightfv(GL_LIGHT0, GL_POSITION, diffuse_light_pos);

	// enable the light sources...
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
//	glEnable(GL_LIGHT1);
	glEnable(GL_DEPTH_TEST);

}

//Called when the window is resized
void handleResize(int w, int h) {

	current_width = w;
	current_height = h;

	//Tell OpenGL how to convert from coordinates to pixel values
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION); //Switch to setting the camera perspective
	//Set the camera perspective
	glLoadIdentity(); //Reset the camera
	gluPerspective(45.0, (float) w / (float) h, 1.0, 1000.0);
	glMatrixMode(GL_MODELVIEW);

}
long long int t_prev = PAPI_get_real_usec();

void idle_callback() {

	//after finished rendering the scene
	if (start_simulation) {
		//save the previous points
		move();
		long long int t_now = PAPI_get_real_usec();
		dt = (float) (t_now - t_prev) / 1e6;
		t = t + dt;
		t_prev = t_now;
	} else {
		t_prev = PAPI_get_real_usec();
	}
	//upon exiting the main ->
	glutPostRedisplay();

}

//Draws the 3D scene
void drawScene() {

	//Clear information from last draw...
	//add the current point to the trajectory array

	glClearDepth(1);
	glClearColor(.324, .1534, .1123, 0.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	if (connect_points) {
		glDisable(GL_LIGHTING);
		glColor3f(0.0, 0.0, 1.0);

		glBegin(GL_LINE_STRIP);

		for (int k = 0; k < pt_ctr_; k++) {
			glVertex3f(trajectory[k][0], trajectory[k][1], trajectory[k][2]);
		}

		glEnd();
		glEnable(GL_LIGHTING);
	}
	// display bitmap information ...
	if (disp_text) {
		int x_ = 5;
		int y_ = 30;
		int font_h = 18 + 2;

		setOrthographicProjection();
		glLoadIdentity();
		char text[256];
		sprintf(text, "Position [%.3f;%.3f;%.3f]", x[0], x[1], x[2]);
		render_strings(x_, y_, GLUT_BITMAP_HELVETICA_18, text, strlen(text));
		y_ += font_h;
		sprintf(text, "Velocity [%.3f;%.3f;%.3f]", current_velocity[0],
				current_velocity[1], current_velocity[2]);
		render_strings(x_, y_, GLUT_BITMAP_HELVETICA_18, text, strlen(text));

		y_ += font_h;
		sprintf(text, "Acceleration [%.3f;%.3f;%.3f]", current_acceleration[0],
				current_acceleration[1], current_acceleration[2]);
		render_strings(x_, y_, GLUT_BITMAP_HELVETICA_18, text, strlen(text));
		y_ += font_h;
		sprintf(text, "Direction of Motion [%.3f;%.3f;%.3f]", direction[0],
				direction[1], direction[2]);
		render_strings(x_, y_, GLUT_BITMAP_HELVETICA_18, text, strlen(text));
		y_ += font_h;
		sprintf(text, "Elapsed Time: [%.3f s]", t);
		render_strings(x_, y_, GLUT_BITMAP_HELVETICA_18, text, strlen(text));
		restorePerspectiveProjection();

	}

	// go into modelview mode
	glMatrixMode(GL_MODELVIEW);
	// restore the idenity...
	glLoadIdentity();

	drawAxes();
	// translate and rotate the world view...

	GLfloat diffuse_light_pos[] = { 0.0f, 5.0f, 10.0f, 1 }; // make the diffuse light source positional...
	glLightfv(GL_LIGHT0, GL_POSITION, diffuse_light_pos);
	glTranslatef(x_translate, y_translate, z_translate);
	glRotatef(angle_x, 1, 0, 0);
	glRotatef(angle_y, 0, 1, 0);
	//draw the particle with fixed radius to the new position...

	glPushMatrix();
	GLfloat sphere_color[] = { 1.0, 0.0, 0.0, 1.0f };
	glMaterialfv(GL_FRONT, GL_DIFFUSE, sphere_color);
	glTranslatef(x[0], x[1], x[2]);
	glutSolidSphere(0.5f, 25, 25);
	glPopMatrix();

	if (target[3] != 0) {

		glPushMatrix();
		GLfloat target_color[] = { 0.0, 1.0, 0.0, 1.0f };
		glMaterialfv(GL_FRONT, GL_DIFFUSE, target_color);
		glTranslatef(target[0], target[1], target[2]);
		glutSolidSphere(0.05f, 10, 10);
		glPopMatrix();

	}
//	draw_simulation_domain();

	glutSwapBuffers(); //Send the 3D scene to the screen
}

void draw_simulation_domain() {

	float size = 3;
	// do some other drawing
	//	draw the borders of the simulation domain

	glBegin(GL_LINES);

	int a = 0;
	int b = a;
	int _bit_1, _bit_2, _bit_3;


	// i do not know how I DID THAT !?!?

	for (int i = 1; i <= 4; i++) {
		for (int j = 1; j <= 3; j++) {
			//reverse the jth bit of a;
			b = a ^ (1 << (j - 1));

			_bit_1 = (a % 2);
			_bit_2 = ((a >> (1)) % 2);
			_bit_3 = ((a >> (2)) % 2);

			glLineWidth(2.5f);
			glColor3f(1, 0, 0);
			glVertex3f((2 * _bit_1 - 1) * size / 2, (2 * _bit_2 - 1) * size / 2,
					(2 * _bit_3 - 1) * size / 2);
			_bit_1 = (b % 2);
			_bit_2 = ((b >> (1)) % 2);
			_bit_3 = ((b >> (2)) % 2);
			glVertex3f((2 * _bit_1 - 1) * size / 2, (2 * _bit_2 - 1) * size / 2,
					(2 * _bit_3 - 1) * size / 2);

		}
		// reverse the ith bit of a
		a = a ^ (1 << (i - 1));
	}
	a = 4;
	for (int j = 1; j <= 3; j++) {
		//reverse the jth bit of a;
		b = a ^ (1 << (j - 1));

		_bit_1 = (a % 2);
		_bit_2 = ((a >> (1)) % 2);
		_bit_3 = ((a >> (2)) % 2);

		glLineWidth(2.5f);
		glColor3f(1, 0, 0);
		glVertex3f((2 * _bit_1 - 1) * size / 2, (2 * _bit_2 - 1) * size / 2,
				(2 * _bit_3 - 1) * size / 2);
		_bit_1 = (b % 2);
		_bit_2 = ((b >> (1)) % 2);
		_bit_3 = ((b >> (2)) % 2);
		glVertex3f((2 * _bit_1 - 1) * size / 2, (2 * _bit_2 - 1) * size / 2,
				(2 * _bit_3 - 1) * size / 2);
	}

	a = 6;
	for (int j = 1; j <= 3; j++) {
		//reverse the jth bit of a;
		b = a ^ (1 << (j - 1));
		_bit_1 = (a % 2);
		_bit_2 = ((a >> (1)) % 2);
		_bit_3 = ((a >> (2)) % 2);
		glLineWidth(2.5f);
		glColor3f(1, 0, 0);
		glVertex3f((2 * _bit_1 - 1) * size / 2, (2 * _bit_2 - 1) * size / 2,
				(2 * _bit_3 - 1) * size / 2);
		_bit_1 = (b % 2);
		_bit_2 = ((b >> (1)) % 2);
		_bit_3 = ((b >> (2)) % 2);
		glVertex3f((2 * _bit_1 - 1) * size / 2, (2 * _bit_2 - 1) * size / 2,
				(2 * _bit_3 - 1) * size / 2);
	}

	glEnd();

}

void render_strings(GLint x, GLint y, void * font, char* string,
		int stringlength) {

	//render text in white
	glDisable(GL_LIGHTING);

	glColor3f(1.0f, 1.0f, 1.0f);
	glRasterPos2i(x, y);
	for (int i = 0; i < stringlength; i++) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, string[i]);
	}
	glEnable(GL_LIGHTING);
}

void setOrthographicProjection() {

	// switch to projection mode
	glMatrixMode(GL_PROJECTION);

	// save previous matrix which contains the
	//settings for the perspective projection
	glPushMatrix();

	// reset matrix
	glLoadIdentity();
	// set a 2D orthographic projection
	gluOrtho2D(0, current_width, 0, current_height);
	// invert the y axis, down is positive
	glScalef(1, -1, 1);
	// mover the origin from the bottom left corner
	// to the upper left corner
	glTranslatef(0, -current_height, 0);

	// switch back to modelview mode
	glMatrixMode(GL_MODELVIEW);
}

void restorePerspectiveProjection() {

	glMatrixMode(GL_PROJECTION);
	// restore previous projection matrix
	glPopMatrix();
	// get back to modelview mode
	glMatrixMode(GL_MODELVIEW);
}

void drawAxes() {
	glDisable(GL_LIGHTING);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glViewport(0, 0, current_width / 4, current_height / 4);

	// x axis
	glLoadIdentity();
	glTranslatef(0, 0, -4);
	glRotatef(-angle_x, 1, 0, 0);
	glRotatef(-angle_y, 0, 1, 0);
	glRotatef(90, 0, 1, 0);
	glColor3f(1.0, 0, 0);
	GLUquadricObj *quadObj = gluNewQuadric();
	gluCylinder(quadObj, 0.2, 0.0, 1.5, 25, 25);

	// y axis
	glLoadIdentity();
	glTranslatef(0, 0, -4);
	glRotatef(-angle_x, 1, 0, 0);
	glRotatef(-angle_y, 0, 1, 0);
	glRotatef(-90, 1.0, 0, 0);
	glColor3f(0, 1.0f, 0);
	quadObj = gluNewQuadric();
	gluCylinder(quadObj, 0.2, 0.0, 1.5, 25, 25);

	// z axis
	glLoadIdentity();
	glTranslatef(0, 0, -4);

	glRotatef(-angle_x, 1, 0, 0);
	glRotatef(-angle_y, 0, 1, 0);

	glRotatef(180, 1, 0, 0);
	glColor3f(0, 0, 1.f);
	quadObj = gluNewQuadric();
	gluCylinder(quadObj, 0.2, 0.0, 1.5, 25, 25);

	// restore the prev viewport
	glViewport(0, 0, current_width, current_height);

	glPopMatrix();
	glEnable(GL_LIGHTING);
}

