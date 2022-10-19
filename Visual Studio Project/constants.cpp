#include "constants.h"

void consts::init() {
    // state constants
    init_prey = 10;
    init_pred = 10;
    simulating = false;
    sim_step_per_frame = 1;
    sim_steps = 0, frame_steps = 0;

    // prey constants
    prey_size = 0.02f;
    prey_speed = 0.05;
    prey_see_range = 0.2;

    // predator constants
    pred_size = 0.02f;
    pred_speed = 0.1;
    pred_see_range = 0.1;
    idle_slow = 0.7;
    idle_dir_c = 0.05;
    energy_on = 0;
}