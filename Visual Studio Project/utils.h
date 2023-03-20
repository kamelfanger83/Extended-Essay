#pragma once
 
#include <utility>

#define PI 3.1415926538979323f

#define vec2f std::pair<float, float>

const double dinf = 1e100;

float clamp(float, float, float);

float len(vec2f);

float sdlen(vec2f);

vec2f norm(vec2f, float);

vec2f polartocart(float, float);

vec2f operator+ (vec2f, vec2f);

vec2f operator+= (vec2f&, vec2f);

vec2f operator- (vec2f, vec2f);

float operator* (vec2f, vec2f);

vec2f operator* (vec2f, float);