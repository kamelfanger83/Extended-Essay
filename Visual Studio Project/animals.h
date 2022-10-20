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
	void step(std::list<std::shared_ptr<predator>>& pred_vec, State state);
};

class predator {
public:
	vec2f pos;
	float phi_idle;
	float energy;
	predator();
	predator(vec2f);
	bool step(std::list<std::shared_ptr<prey>>& prey_vec, std::list<std::shared_ptr<predator>>& pred_vec, State state);
};