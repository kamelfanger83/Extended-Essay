#pragma once

struct State;
struct GLFWwindow;

namespace Mouse {
	void init(State&);
	void mouse_button_callback(GLFWwindow*, int, int, int);
}