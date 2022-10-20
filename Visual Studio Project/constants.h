#ifndef D_CONSTANTS
#define D_CONSTANTS

namespace consts {
	// state constants
	extern int init_prey, init_pred;
	extern bool simulating;
	extern float sim_step_per_frame;
	extern float sim_steps, frame_steps;
	// prey constants
	extern float prey_size;
	extern float prey_speed, prey_see_range;

	//predator constants
	extern float pred_size;
	extern float pred_speed, pred_see_range;
	extern float idle_slow;
	extern float idle_dir_c;
	extern float energy_on;
};


#endif // !D_CONSTANTS