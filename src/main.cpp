/*
 * main.cpp
 *
 *  Created on: 26 May 2014
 *      Author: kenny
 */
#include "common.hpp"
#include "parser.hpp"
#include "graphics.hpp"

#define OMP_NUM_THREADS 5
bool exit_prog = false;
int tid;
void list_commands();
Abstract_Control* controller = NULL;
void exec_control_loop();

int main(int argc, char** argv) {

	// set one visualizer thread and one thread to interract with the user...
	omp_set_num_threads(2);
	//Set up all the socket conneciton props...
	//Abstract_Control* controller = new Socket_IO_Control();
	controller = new Console_IO_Control();

#pragma omp parallel
	{
		tid = omp_get_thread_num();

		//	setup initially the control loop
		//	 either work with console user input or with sockets...

		// initialize display window
		if(tid == THREAD_SIMULATION){
			glutInit(&argc, argv);
			glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
			glutInitWindowSize(400, 400); //Set the window size
			//Create the window
			glutCreateWindow("motion3D Simulation");

			/// register callback functions
			glutDisplayFunc(drawScene);
			glutKeyboardFunc(handleKeypress);
			glutSpecialFunc(keyboardNavigator);
			glutReshapeFunc(handleResize);
			glutIdleFunc(idle_callback);
			initGraphics();
			// call the control_loop at least once!!
			glutMainLoop(); //Start the main loop.  glutMainLoop doesn't return.
		}else{
			exec_control_loop();
		}
	}
	return 0;

}

void exec_control_loop() {
	if (controller != NULL)
		controller->loop();
}
