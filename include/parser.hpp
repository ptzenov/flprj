/*
 * parser.hpp
 *
 *  Created on: 26 May 2014
 *      Author: kenny
 */

#ifndef PARSER_HPP_
#define PARSER_HPP_
#include "common.hpp"
//include old std string librarhy
#include <string.h>

// constants specifying the possible trajectory types
#define TRAJ_TYPE_LINEAR 0
#define TRAJ_TYPE_SPHERICAL 1
#define TRAJ_TYPE_HELICAL 2
#define TRAJ_TYPE_SEQUENTIAL 3

// constant specifying the current trajectory selection
//unsigned int CURRENT_TRAJECTORY = 0;
#define SELECTOR_1 0
#define SELECTOR_2 1

void parse_next_command(std::string s);
std::string normalizeStr(std::string s);

class Abstract_Control {

public:
	virtual void loop() = 0;
protected:
	virtual void list_commands() {
		std::cout
				<< "\n############################ Welcome to motion3D v 1.0 ############################ \n";
		std::cout << "Currently supported commands are:\n";
		for (int i = 0; i < NR_CMDS; i++) {
			std::cout << " " << i << ": " << commands[i] << "\n";
		}
		std::cout << " " << NR_CMDS << ": " << "end (to exit the program)\n";

		std::cout
				<< "PLease note that in order to start the simulation, you need to enter the command [MOVE]\n";
	}

};

class Console_IO_Control: public Abstract_Control {

public:
	void loop();
};

class Socket_IO_Control: public Abstract_Control {
public:
	void loop();

};

#endif /* PARSER_HPP_ */
