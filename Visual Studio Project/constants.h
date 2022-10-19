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

	static void init() {
	    // state constants
		init_prey = 10;
		init_pred = 10;
		simulating = false;
		sim_step_per_frame = 1;
		sim_steps = 0, frame_steps = 0;

		// prey constants
		prey_size = 0.02f;
		prey_speed = 0.05;
		prey_see_range = 0.2;

		// predator constants
		pred_size = 0.02f;
		pred_speed = 0.1;
		pred_see_range = 0.1;
		idle_slow = 0.7;
		idle_dir_c = 0.05;
		energy_on = 0;
	}
};


#endif // !D_CONSTANTS