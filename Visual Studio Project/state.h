#ifndef D_CONSTANTS
#define D_CONSTANTS

struct State {
	// state constants
	int init_prey, init_pred;
	bool simulating;
	float sim_step_per_frame;
	float sim_steps, frame_steps;
	// prey constants
	float prey_size;
	float prey_speed, prey_see_range;

	//predator constants
	float pred_size;
	float pred_speed, pred_see_range;
	float idle_slow;
	float idle_dir_c;
	float energy_on;
	float energy_mov;
	// eating always gives 1 energy
};


#endif // !D_CONSTANTS