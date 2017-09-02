

*************************************

Type of project: C or C++ programming.

Platform: Linux

********
Summary:
********

This program models the state and location of an hypothetical object in a 3D space.
The object can be instructed to move, either in a line, arc or a helix.

********
Details:
********

This program continuously calculates the position, state of motion, movement and trajectory.

The program operated by passing it instructions and queries. These are simple strings which are designed to require minimal parsing.

The calculation is run continuously, but iteratively. Each iteration is based on time, referred to as the "time base".
Between each iteration, the program can respond to instructions. For example, changing to a new velocity or requesting a specific state variable.

As new values are programmed the behaviour of the objects motion must change.

Parameters will be given to this "solver" which define needed details such as  maximum velocity, acceleration, deceleration and position.


The instructions and query format is basic.

For example:

SET_ACCELERATION(1.2650)   this is an instruction from the user interface to the solver.
GET_ACCELERATION			this is an instruction from the user interface to the solver requesting data.
ACCELERATION=1.25600		this is the reply of the solver to the request above.

***********************
INSTALLATION
**********************

1)For most accurate time reference the project uses the papi library as specified here 
http://icl.utk.edu/papi/. I have included a precompiled version of libpapi.a in the lib directory so you 
do not need to install papi yourself, in case you wish to, please follow the compilation instructions in the official web site of 
the papi project to download compile and install the library. Also for successful linking, do not forget to modify the CMakeLists.txt file accordingly

2) additionally you will need to have some of the OpenGL and related dev libraries: OpenGl (GLUT and GL) as well as the GLEW utilities library. 
to install the latter do the following: in the command line type in 
$ sudo apt-get install freeglut3-dev
to install opengl development toolkit (glut and gl components).

3) for the GLEW utility library, again I have included precompiled version of GLEW directly into the project.
However successful linking to GLEW might require the following dependencies:
$ sudo apt-get install build-essential libxmu-dev libxi-dev libosmesa-dev


In case you wish to download and isntall a different version of glew, then download glew tarball archive from
https://sourceforge.net/projects/glew/files/glew/2.1.0/glew-2.1.0.tgz/download 
and extract the archive using
$ tar -xf glew-2.1.0.tgz 
$ cd glew-2.1.0 
$ make
$ sudo make install
If you are installing GLEW independently, do not forget to modify the inclulde_directories and link_directories
in the CMakeLists.txt file accordingly.


4) Finally if everything is done correctly, you simply need to navigate to the build directory 
$ cd build
and execute
$ cmake ..
$ make

to launch the executable use ./flprj

*************************************
Commands:
*************************************
once you run the simulation you are able to input control commands from the command line. 
To display a list of all supported commands, type in
help

To start the simulation, type in 
move 

To stop the simulation, type in 
pause 

The rest of the commands are self-explanatory. 


*************************************
Instructions which are supported:
*************************************

Currently supported commands are:
 0: set_move_type
 1: move
 2: pause
 3: resume
 4: set_linear_move_direction
 5: start_continuous_linear_move_x
 6: stop_continuous_linear_move_x
 7: start_continuous_linear_move_y
 8: stop_continuous_linear_move_y
 9: start_continuous_linear_move_z
 10: stop_continuous_linear_move_z
 11: set_position_x
 12: set_position_y
 13: set_position_z
 14: set_velocity_1
 15: set_velocity_2
 16: set_velocity_selector_to_1
 17: set_velocity_selector_to_2
 18: set_acceleration_1
 19: set_acceleration_2
 20: set_acceleration_selector_to_1
 21: set_acceleration_selector_to_2
 22: set_rotation_axis
 23: set_center_of_rotation
 24: set_angular_velocity
 25: set_angular_acceleration
 26: set_axial_velocity
 27: go_to
 28: set_axis_max
 29: set_axis_min
 30: set_rotation_plane
 31: set_arch_angle
 32: end (to exit the program)
PLease note that in order to start the simulation, you need to enter the command [MOVE]




