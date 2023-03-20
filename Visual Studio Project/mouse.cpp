#include "state.h"
#include "utils.h"
#include "animals.h"

#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include <iostream>

State* statep;

namespace Mouse {
    void init(State& state){
        statep = &state;
    }

    void mouse_button_callback(GLFWwindow* window, int button, int action, int mods){
        if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {

            double posx, posy; // screen position
            glfwGetCursorPos(window, &posx, &posy);

            int width, height;
            glfwGetWindowSize(window, &width, &height);

            double glposx, glposy; // OpenGL position
            glposx = (posx - width / 2.0f) / width * 2;
            glposy = (posy - height / 2.0f) / height * 2;

            double sposx, sposy; // simulation position
            sposx = glposx * statep->zoom[2] + statep->zoom[0];
            sposy = - (glposy * statep->zoom[2] + statep->zoom[1]);

            int closest_i = -1; double closest_d = dinf; int i = 0;
            for (auto predator : statep->pred_vec) {
                double dx = predator->pos.first - sposx;
                double dy = predator->pos.second - sposy;
                double dst = sqrt(dx * dx + dy * dy);
                if (dst < closest_d) {
                    closest_d = dst;
                    closest_i = i;
                }
                i++;
            }
            if (closest_d < 0.1) statep->tracking = statep->pred_vec[closest_i];
            else statep->tracking = NULL;
        }
    }
}
