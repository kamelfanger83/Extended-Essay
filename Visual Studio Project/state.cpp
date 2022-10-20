#include "state.h"

namespace State{
    // state constants
    int init_prey = 10;
    int init_pred = 10;
    bool simulating = false;
    float sim_step_per_frame = 1;
    float sim_steps = 0, frame_steps = 0;

    // prey constants
    float prey_size = 0.02f;
    float prey_speed = 0.01;
    float prey_see_range = 0.2;

    // predator constants
    float pred_size = 0.02f;
    float pred_speed = 0.02;
    float pred_see_range = 0.1;
    float idle_slow = 0.7;
    float idle_dir_c = 0.05;
    float energy_on = 0;
    float energy_mov = 0.01;
}