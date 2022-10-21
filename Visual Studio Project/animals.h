#ifndef D_ANIMALS
#define D_ANIMALS

#include <vector>
#include <memory>
#include <list>

#include "utils.h"
#include "state.h"

#endif //!D_ANIMALS

class predator;

class prey {
public:
	vec2f pos;
	prey();
	void step(std::list<std::shared_ptr<predator>>& pred_vec, State& state);
};

class predator {
public:
	vec2f pos;
	float phi_idle;
	float energy;
	int scared_frames;
	vec2f scared_step;
	predator(State state);
	predator(vec2f, State state);
	bool step(std::list<std::shared_ptr<prey>>& prey_vec, std::list<std::shared_ptr<predator>>& pred_vec, std::list<std::shared_ptr<predator>>::iterator self, State& state);
};