#include "animals.h"
#include "utils.h"

#include <vector>
#include <memory>
#include <list>

#include <iostream>

prey::prey() { // generate prey evenly within game circle
	float r = std::max((float)rand() / RAND_MAX, (float)rand() / RAND_MAX); // get PDF needed to distribute point evenly in circle
	float phi = (float)rand() / RAND_MAX * 2 * PI;
	pos = polartocart(phi, r);
}

void prey::step(std::list<std::shared_ptr<predator>>& pred_vec, State& state) {
	if (pred_vec.size() == 0) return;

	float min_dst_sq = 10; //distances within field cannot exceed sqrt(2)*2 (diagonals) so square has to be <= 8
	auto nearest = pred_vec.begin();

	for (auto cand_nearest = pred_vec.begin(); cand_nearest != pred_vec.end(); cand_nearest++) { //iterate through prey to find one that is closest (could be done more efficiently by using datastructure trees optimised for NN but this is likely not the bottleneck in my simulation)
		vec2f dif = (*cand_nearest)->pos - pos;
		if (sdlen(dif) < min_dst_sq) {
			min_dst_sq = sdlen(dif);
			nearest = cand_nearest;
		}
	}

	vec2f dvec = (*nearest)->pos - pos;

	if (sdlen(pos + norm(dvec, state.prey_speed)) > 1) {
		std::pair<float, float> per_vec = { pos.second, -pos.first }; // make vector perpendicular to position vector to determine in which direction to flee

		if (per_vec * dvec > 0) { // calculate dot product to determine whether per_vec and direction it wants to flee is in the same direction
			dvec = per_vec;
		}
		else { // if not flee in direction of - per_vec
			dvec = per_vec * -1;
		}
	}

	if (len(dvec) < 0.0000001) return; //precaution for division

	else if (len(dvec) < state.prey_see_range) { // move away from nearest predator, distance normalized to speed
		dvec = norm(dvec, state.prey_speed);
		pos = pos - dvec;
		if (sdlen(pos) > 1) pos = norm(pos, 1);
	}
}


predator::predator(State state) { // generate predators evenly within game circle
	float r = std::max((float)rand() / RAND_MAX, (float)rand() / RAND_MAX); // get PDF needed to distribute point evenly in circle
	float phi = (float)rand() / RAND_MAX * 2 * PI;
	pos = polartocart(phi, r);
	phi_idle = 3 * PI; // initialize phi_idle to 3 * PI which i define as undefined -> has to be initialized first;
	energy = state.energy_on;
	scared_frames = 0;
}

predator::predator(vec2f pos, State state) : pos(pos) {
	energy = state.energy_on;
	scared_frames = 0;
}

bool predator::step(std::list<std::shared_ptr<prey>>& prey_vec, std::list<std::shared_ptr<predator>>& pred_vec, std::list<std::shared_ptr<predator>>::iterator self, State& state) {
	if (energy < 0) return true; // means this creature dies in this frame
	if (energy >  2 * state.energy_on) {
		vec2f childpos = pos + polartocart((float)rand() / RAND_MAX * 2 * PI, 5 * state.pred_speed); // spawn child a little aways so they are not practically the same entity
		pred_vec.insert(pred_vec.end(), std::shared_ptr<predator>(new predator(childpos, state)));
		energy -= state.energy_on;
	}

	if (prey_vec.size() == 0) return false;

	if (pred_vec.size() > 1) {
		float pred_min_dst_sq = 10; //distances within field cannot exceed sqrt(2)*2 (diagonals) so square has to be <= 8
		auto pred_nearest = pred_vec.begin();

		for (auto cand_nearest = pred_vec.begin(); cand_nearest != pred_vec.end(); cand_nearest++) { //iterate through prey to find one that is closest (could be done more efficiently by using datastructure trees optimised for NN but this is likely not the bottleneck in my simulation)
			if (cand_nearest == self) continue;
			vec2f dif = (*cand_nearest)->pos - pos;
			if (sdlen(dif) < pred_min_dst_sq) {
				pred_min_dst_sq = sdlen(dif);
				pred_nearest = cand_nearest;
			}
		}

		vec2f dvec = (*pred_nearest)->pos - pos;

		if (len(dvec) < state.terretorial_range) {
			energy -= state.energy_mov;
			dvec = norm(dvec, state.pred_speed);
			pos = pos - dvec;
			phi_idle = 3 * PI; // set idle direction as unitialized again so it gets regenerated next time
			scared_frames = state.init_scared;
			scared_step = dvec;

			(*pred_nearest)->scared_frames = state.init_scared; // set other predator also scared because my step would go out of his scared range otherwise
			(*pred_nearest)->scared_step = dvec * -1;
			(*pred_nearest)->phi_idle = 3 * PI;

			return false;
		}
	}

	if (scared_frames > 0) {
		energy -= state.energy_mov;
		scared_frames--;
		pos = pos - scared_step;
		if (sdlen(pos) > 1) pos = norm(pos, 1);
		return false;
	}

	float prey_min_dst_sq = 10; //distances within field cannot exceed sqrt(2)*2 (diagonals) so square has to be <= 8
	auto prey_nearest = prey_vec.begin();

	for (auto cand_nearest = prey_vec.begin(); cand_nearest != prey_vec.end(); cand_nearest++) { //iterate through prey to find one that is closest (could be done more efficiently by using datastructure trees optimised for NN but this is likely not the bottleneck in my simulation)
		vec2f dif = (*cand_nearest)->pos - pos;
		if (sdlen(dif) < prey_min_dst_sq) {
			prey_min_dst_sq = sdlen(dif);
			prey_nearest = cand_nearest;
		}
	}

	vec2f dvec = (*prey_nearest)->pos - pos;

	if (len(dvec) < state.pred_speed) {
		(*prey_nearest) = std::shared_ptr<prey>(new prey());
		energy += 1;
		state.prey_eaten++;
	}

	else if (len(dvec) < state.pred_see_range) { // move in direction of nearest prey, distance normalized to speed
		energy -= state.energy_mov;
		dvec = norm(dvec, state.pred_speed);
		pos = pos + dvec;
		phi_idle = 3 * PI; // set idle direction as unitialized again so it gets regenerated next time
	}

	else { // move in direction of phi_idle with reduced speed;
		energy -= state.energy_mov * state.idle_slow * state.idle_slow; //subtract a reduced amount of energy (~square of v bc E = 1/2 m v^2) 
		if (phi_idle > 2 * PI) phi_idle = (float)rand() / RAND_MAX * 2 * PI;
		phi_idle += (float)rand() / RAND_MAX * 2 * state.idle_dir_c - state.idle_dir_c;
		if (phi_idle < 0) phi_idle += 2 * PI;
		if (phi_idle > 2 * PI) phi_idle -= 2 * PI;
		pos += polartocart(phi_idle, state.pred_speed * state.idle_slow);
		if (sdlen(pos) > 1) {
			pos = norm(pos, 1);
			phi_idle = (float)rand() / RAND_MAX * 2 * PI;
		}
	}
	return false;
}
