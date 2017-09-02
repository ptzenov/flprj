

*************************************

Type of project: C or C++ programming.

Platform: Linux

********
Summary:
********

This program models the state and location of an hypothetical object in a 3D space.
The object can be instruction to move, either in a line, arc or a helix.

********
Details:
********

This program continuously calculates the position, state of motion, movement and trajectory.
Calculations will be based on trapezoidal trajectory profiles.

The program operated by passing it instructions and queries.
These are simple strings which are designed to require minimal parsing.

The calculation is run continuously, but iteratively.
Each iteration is based on time, referred to as the "time base".
Between each iteration, the program can respond to instructions.
For example, changing to a new velocity or requesting a specific state variable.

As new values are programmed the behaviour of the objects motion must change.

Parameters will be given to this "solver" which define needed details such as 
maximum velocity, acceleration, deceleration and position.


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

SET MOVE TYPE SELECTOR					You specify LINEAR, ARC, HELIX or SEQUENTIAL2

MOVE 									Causes a move of the set type. Values are absolute.
Sequential2 results in movement along X and Y then Z.
This does not follow a coordinated interpolation between each axis.
Occurs at velocity #2

STEP RELATIVE X							Causes a linear movement to occur. Value is a relative input. 
STEP RELATIVE Y							
STEP RELATIVE Z

START CONTINUOUS LINEAR MOVE X
STOP CONTINUOUS LINEAR MOVE X
START CONTINUOUS LINEAR MOVE Y
STOP CONTINUOUS LINEAR MOVE Y
START CONTINUOUS LINEAR MOVE Z
STOP CONTINUOUS LINEAR MOVE Z

SET ACCELERATION/DECELERATION #1
SET ACCELERATION/DECELERATION #2

SET ACCELERATION SELECTOR TO #1
SET ACCELERATION SELECTOR TO #2

SET ACCELERATION/DECELEATION RAMP ON		This dictates if accel/decel calcs will be performed or not.
SET ACCELERATION/DECELERATION RAMP OFF		This only applies to LINEAR type movements.

SET PLANE SELECT TO XY
SET PLANE SELECT TO XZ
SET PLANE SELECT TO YZ

SET VELOCITY #1
SET VELOCITY #2

SET VELOCITY SCALE #1
SET VELOCITY SCALE #2

SET POSITION X
SET POSITION Y
SET POSITION Z

SET AXIS POSITIVE RANGE X
SET AXIS POSITIVE RANGE Y
SET AXIS POSITIVE RANGE Z

SET AXIS NEGATIVE RANGE X
SET AXIS NEGATIVE RANGE Y
SET AXIS NEGATIVE RANGE Z

GET POSITION X
GET POSITION Y
GET POSITION Z

GET DELTA ACCELERATION X
GET DELTA ACCELERATION Y
GET DELTA ACCELERATION Z

GET DELTA VELOCITY X
GET DELTA VELOCITY Y
GET DELTA VELOCITY Z

GET ACCELERATION SELECTOR					These selectors report the state 	programmed by the corresponding SET instructions. 
GET PLANE SELECTOR
GET RAMP SELECTOR

GET VELOCITY SCALE #1
GET VELOCITY SCALE #2








