#ifndef D_CONSTANTS
#define D_CONSTANTS

#include <string>

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
	float terretorial_range;
	// eating always gives 1 energy

	//tracking constants
	int sample_interval;
	int tick;
	int prey_eaten;
};

struct sampled_attribute {
	std::string attribute_name;
	float value;
	sampled_attribute(std::string attribute_name, float value) : attribute_name(attribute_name), value(value) {}
};

#endif // !D_CONSTANTS