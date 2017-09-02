/*
 * graphics.hpp
 *
 *  Created on: 26 May 2014
 *      Author: kenny
 */

#ifndef GRAPHICS_HPP_
#define GRAPHICS_HPP_


//include glut and opengl headers

#include <math.h>

#define milisec 25
#define BALL_RADIUS 10.0f;
#define TIME_BASE 1.0

void idle(void);
void drawScene();
void handleResize(int w, int h);
void initGraphics();
void keyboardNavigator(int key, int x, int y);
void handleKeypress(unsigned char key, int x, int y);
void idle_callback();
void render_strings(GLint x, GLint y,void * font, char* string, int strlength);
void restorePerspectiveProjection();
void setOrthographicProjection();
void draw_simulation_domain();
void drawAxes();
#endif /* GRAPHICS_HPP_ */
