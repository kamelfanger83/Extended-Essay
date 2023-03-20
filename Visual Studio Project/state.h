#ifndef D_CONSTANTS
#define D_CONSTANTS

#include <string>
#include <functional>
#include <vector>
#include <memory>

struct prey;
struct predator;

struct State {
	// state constants
	int init_prey, init_pred;
	bool simulating;
	float sim_step_per_frame;
	double sim_steps, frame_steps;
	float dt;

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
	float territorial_range;
	int init_scared;
	// eating always gives 1 energy

	//sampling constants
	int pre_sample_ticks;
	int sample_wait;
	int sample_interval;
	int sample_total;
	int sample_step;
	bool sampling;
	bool wants_sampling;
	bool wants_auto_sampling;
	bool sample_done;
	int tick;
	int prey_eaten;

	// simulation variables
	std::vector<std::shared_ptr<prey>> prey_vec;
	std::vector<std::shared_ptr<predator>> pred_vec;
	std::vector<std::shared_ptr<predator>> npred_vec;

	// tracking variables
	std::shared_ptr<predator> tracking;

	// visual variables
	float* zoom;
};

struct attribute {
	std::string attribute_name;
	std::function<float()> get;
	attribute(std::string attribute_name, std::function<float()> get) : attribute_name(attribute_name), get(get) {}
};

struct experiment {
	std::string fileName;
	State start;
	std::function<float()> step;
	experiment(std::string fileName, State start, std::function<float()> step) : fileName(fileName), start(start), step(step) {}
};

#endif // !D_CONSTANTS