#pragma once

#ifndef D_CONSTANTS
#define D_CONSTANTS

namespace constant {
	// state constants
	int init_prey = 10, init_pred = 10;
	bool simulating = false;
	float sim_step_per_frame = 1;
	float sim_steps, frame_steps;

	// prey constants
	float prey_size = 0.02f;
	float prey_speed = 0.05, prey_see_range = 0.2;

	//predator constants
	float pred_size = 0.02f;
	float pred_speed = 0.1, pred_see_range = 0.1;
	float idle_slow = 0.7; float idle_dir_c = 0.05;
	float energy_on;
}


#endif // !D_CONSTANTS