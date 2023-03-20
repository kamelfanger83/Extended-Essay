#ifndef D_ANIMALS
#define D_ANIMALS

#include <vector>
#include <memory>
#include <vector>

#include "utils.h"
#include "state.h"

#endif //!D_ANIMALS

class predator;

class prey {
public:
	vec2f pos;
	prey();
	void step(State& state);
};

class predator {
public:
	vec2f pos;
	float phi_idle;
	float energy;
	double scared_frames;
	vec2f scared_step;
	predator(State state);
	predator(vec2f, State state);
	bool step(std::shared_ptr<predator> self, State& state);
};