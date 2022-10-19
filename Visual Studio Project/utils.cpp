#include "utils.h"

#include <utility>


float clamp(float x, float min, float max) {
	if (x < min) return x;
	if (x > max) return max;
	return x;
}

float len(vec2f a) {
	return sqrt(a.first * a.first + a.second * a.second);
}

float sdlen(vec2f a) {
	return a.first * a.first + a.second * a.second;
}

vec2f norm(vec2f a, float l) {
	float cur_l = len(a);
	if (cur_l < 0.000001) return { 0,0 }; // avoid division problems
	return { a.first / cur_l * l, a.second / cur_l * l };
}

vec2f polartocart(float phi, float r) {
	return { r * cos(phi), r * sin(phi) };
}

vec2f operator+(vec2f a, vec2f b) {
	return { a.first + b.first, a.second + b.second };
}

vec2f operator+= (vec2f &a, vec2f b) {
	return a = a + b;
}

vec2f operator-(vec2f a, vec2f b) {
	return { a.first - b.first, a.second - b.second };
}

float operator*(vec2f a, vec2f b) {
	return a.first * b.first - a.second * b.second;
}

vec2f operator*(vec2f a, float f) {
	return { a.first * f, a.second * f };
}