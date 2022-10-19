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

void prey::step(std::list<std::shared_ptr<predator>>& pred_vec) {
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
	
	std::pair<float, float> dvec = (*nearest)->pos - pos;

	if (sdlen(pos + norm(dvec, consts::prey_speed)) > 1) {
		std::pair<float, float> per_vec = {pos.second, -pos.first}; // make vector perpendicular to position vector to determine in which direction to flee

		if (per_vec * dvec > 0) { // calculate dot product to determine whether per_vec and direction it wants to flee is in the same direction
			dvec = per_vec;
		}
		else { // if not flee in direction of - per_vec
			dvec = per_vec * -1;
		}
	}

	if (len(dvec) < 0.0000001) return; //precaution for division

	else if (len(dvec) < consts::prey_see_range) { // move away from nearest predator, distance normalized to speed
		dvec = norm(dvec, consts::prey_speed);
		pos = pos - dvec;
		if(sdlen(pos) > 1) pos = norm(pos, 1);
	}
}


predator::predator() { // generate predators evenly within game circle
	float r = std::max((float)rand() / RAND_MAX, (float)rand() / RAND_MAX); // get PDF needed to distribute point evenly in circle
	float phi = (float)rand() / RAND_MAX * 2 * PI;
	pos = polartocart(phi, r);
	phi_idle = 3 * PI; // initialize phi_idle to 3 * PI which i define as undefined -> has to be initialized first;
	energy = 0;
}

predator::predator(vec2f pos): pos(pos) {
	energy = 0;
}

bool predator::step (std::list<std::shared_ptr<prey>>& prey_vec, std::list<std::shared_ptr<predator>>& pred_vec) {
	if (energy < -consts::energy_on) return true; // means this creature dies in this frame
	if (energy > consts::energy_on) {
		vec2f childpos = pos + polartocart((float)rand() / RAND_MAX * 2 * PI, 5*consts::pred_speed); // spawn child a little aways so they are not practically the same entity
		pred_vec.insert(pred_vec.end(), std::shared_ptr<predator>(new predator(childpos)));
	}

	if (prey_vec.size() == 0) return false;

	float min_dst_sq = 10; //distances within field cannot exceed sqrt(2)*2 (diagonals) so square has to be <= 8
	auto nearest = prey_vec.begin();

	for (auto cand_nearest = prey_vec.begin(); cand_nearest != prey_vec.end(); cand_nearest++) { //iterate through prey to find one that is closest (could be done more efficiently by using datastructure trees optimised for NN but this is likely not the bottleneck in my simulation)
		vec2f dif = (*cand_nearest)->pos - pos;
		if (sdlen(dif) < min_dst_sq) {
			min_dst_sq = sdlen(dif);
			nearest = cand_nearest;
		}
	}

	std::pair<float, float> dvec = (*nearest)->pos - pos;

	if (len(dvec) < consts::pred_speed) {
		(*nearest) = std::shared_ptr<prey>(new prey());
	}

	else if (len(dvec) < consts::pred_see_range) { // move in direction of nearest prey, distance normalized to speed
		dvec = norm(dvec, consts::pred_speed);
		pos = pos + dvec;
		phi_idle = 3 * PI; // set idle direction as unitialized again so it gets regenerated next time
	}

	else { // move in direction of phi_idle with reduced speed;
		if (phi_idle > 2 * PI) phi_idle = (float)rand() / RAND_MAX * 2 * PI;
		phi_idle += (float)rand() / RAND_MAX * 2 * consts::idle_dir_c - consts::idle_dir_c;
		if (phi_idle < 0) phi_idle += 2 * PI;
		if (phi_idle > 2 * PI) phi_idle -= 2 * PI;
		pos += polartocart(phi_idle, consts::pred_speed * consts::idle_slow);
		if (sdlen(pos) > 1) {
			pos = norm(pos, 1);
			phi_idle = (float)rand() / RAND_MAX * 2 * PI;
		}
	}
    return false;
}
