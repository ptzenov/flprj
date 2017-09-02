/*
 * command_executioner.hpp
 *
 *  Created on: 26 May 2014
 *      Author: kenny
 */

#ifndef COMMAND_EXECUTIONER_HPP_
#define COMMAND_EXECUTIONER_HPP_

#include "linsolver.hpp"

float normalize_vector(float _n[3]);
void vector_product(const float _n[3],const float _t[3], float _b[3]);
float scalar_product(float _x[3], float _y[3]);

void execute_command(const int command_idx,char* command_values[3]);
void reinit_vector_triplet();
void move();
void execute_linear_motion();
void go_to_linear_motion();
void execute_sequential_motion();
void go_to_sequential_motion();
void execute_circular_motion();
void go_to_circular_motion();
void execute_helical_motion();
void go_to_helical_motion();
void stop_motion();

bool check_bdry_reached(float _x[3]);
bool check_target_reached(float _x[3],float _x_new[3], float _target[3]);
void rotate_x (float angle, float* vect);
void rotate_y (float angle, float* vect);
void rotate_z (float angle, float*vect);

#endif /* COMMAND_EXECUTIONER_HPP_ */
