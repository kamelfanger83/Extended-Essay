#pragma once

#ifndef D_CONSTANTS
#define D_CONSTANTS

class consts{
public:
	// state constants
	static int init_prey, init_pred;
	static bool simulating;
	static float sim_step_per_frame;
	static float sim_steps, frame_steps;
	// prey constants
	static float prey_size;
	static float prey_speed, prey_see_range;

	//predator constants
	static float pred_size;
	static float pred_speed, pred_see_range;
	static float idle_slow;
	static float idle_dir_c;
	static float energy_on;

	static void init();
};


#endif // !D_CONSTANTS