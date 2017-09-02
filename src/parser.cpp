/*
 * parser.cpp
 *
 *  Created on: 26 May 2014
 *      Author: kenny
 */

#include "parser.hpp"
#include "simulation_engine.hpp"


//set the delimiters for the command string
const char* DELIMITER = ")( ,";

char* commands[NR_CMDS] = {
			"set_move_type", "move", "pause",
			"resume", "set_linear_move_direction", "start_continuous_linear_move_x",
			"stop_continuous_linear_move_x", "start_continuous_linear_move_y",
			"stop_continuous_linear_move_y", "start_continuous_linear_move_z",
			"stop_continuous_linear_move_z", "set_position_x", "set_position_y",
			"set_position_z", "set_velocity_1", "set_velocity_2",
			"set_velocity_selector_to_1", "set_velocity_selector_to_2",
			"set_acceleration_1", "set_acceleration_2",
			"set_acceleration_selector_to_1", "set_acceleration_selector_to_2"//22 commands
			,"set_rotation_axis","set_center_of_rotation",
			"set_angular_velocity","set_angular_acceleration" ,//26
			"set_axial_velocity", // 27
			"go_to",
			"set_axis_max", // 29
			"set_axis_min",// 30
			"set_rotation_plane", //31
			"set_arch_angle" //32
			};

// set the maximum possible command length.
const int MAX_CHARS_PER_LINE = 512;

void parse_next_command(std::string ss){

	std::cout << "\nPARSER: " << ss << "\n";
	char buffer[MAX_CHARS_PER_LINE];

	// copy the command string to a char buffer
	strcpy(buffer, ss.c_str());
	//tokenize the string removing ( , ) and empty spaces
	char* token = "";
	token = strtok(buffer, DELIMITER);
	std::string command = "";
	char* values[3];
	if (token) {
		// first token should be the command name
		command = token;
		//should obtain the value...
		token = strtok(NULL, DELIMITER);
		if (!token)
			values[0] = "_";
		else
			values[0] = token;

		token = strtok(NULL, DELIMITER);
		if (!token)
			values[1] = "_";
		else
			values[1] = token;

		token = strtok(NULL, DELIMITER);
		if (!token)
			values[2] = "_";
		else
			values[2] = token;



		std::cout << "parsed command is : " << command << "\n --> values : ["
				<< values[0] << "," << values[1] << "," <<values[2] << "]\n";
	} else {
		std::cout << "PARSER: invalid input command \n";
	}

	// find the current command
	bool command_identified = false;

	for (int i = 0; i < NR_CMDS; ++i) {
		if (!strcmp(command.c_str(), commands[i])) {
			std::cout << "\n Command index :" << i << " input command name: "
					<< command << " original command name: " << commands[i] << "\n";
			execute_command(i,values);
			command_identified = true;
			break;
		}
	}

	if(!command_identified)
		std::cout<<"\n Hmm could not recognize the command you just sent me. Please try again \n";

}

std::string normalizeStr(std::string str) {

	std::string result(str);
	std::locale loc;
	for (std::string::size_type i = 0; i < str.length(); ++i)
		result[i] = std::tolower(str[i], loc);

	return result;

}


void Console_IO_Control::loop() {

	std::cout<< "\nEntering Console IO control loop mode \n";
	std::cout<<"...\n";
	while (true) {
		std::cout<<"Waiting for your next instruction...\n";
		std::string str;
		getline(std::cin, str);
		/// normalize the command to lower case..
		str = normalizeStr(str);
		if(!str.compare("help"))
		{
			list_commands();
			continue;
		}

		if (!str.compare("end")){
			std::cout << "That will be all for now! bye bye \n";
			exit(EXIT_SUCCESS);
		}
		parse_next_command(str);
	}
}

void Socket_IO_Control::loop(){


	std::cout<< "\nEntering Socket IO control loop mode \n";
	std::cout<<"...\n";

	while (true) {

		std::string str;
		getline(std::cin, str);
		/// normalize the command to lower case..
		str = normalizeStr(str);
		if(!str.compare("help"))
		{
			list_commands();
			continue;
		}

		if (!str.compare("end")){
			std::cout << "That will be all for now! bye bye \n";
			exit(EXIT_SUCCESS);
		}

		parse_next_command(str);
	}

}


