#pragma once

#include <vector>
#include <memory>
#include <list>

#include "utils.h"
#include "constants.h"

class predator;

class prey {
public:
	vec2f pos;
	prey();
	void step(std::list<std::shared_ptr<predator>>& pred_vec);
};

class predator {
public:
	vec2f pos;
	float phi_idle;
	float energy;
	predator();
	predator(vec2f);
	bool step(std::list<std::shared_ptr<prey>>& prey_vec, std::list<std::shared_ptr<predator>>& pred_vec);
};