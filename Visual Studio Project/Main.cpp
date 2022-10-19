#include"imgui.h"
#include"imgui_impl_glfw.h"
#include"imgui_impl_opengl3.h"

#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>

#include <vector>
#include <memory>
#include <list>

#include "constants.h"

#include "animals.h"
#include "utils.h"

using namespace constant;


// Vertex Shader for animals source code
const char* vertexShaderAnimalSource = "#version 330 core\n"
"layout (location = 0) in vec2 aPos;\n"
"uniform vec3 zoom;\n" //0,1 : x,y of center, 2: side/2
"void main()\n"
"{\n"
"	float x = (aPos.x - zoom.x)/zoom.z;\n"
"	float y = (aPos.y - zoom.y)/zoom.z;\n"
"   gl_Position = vec4(x, y, 0, 1.0);\n"
"}\0";

//Fragment Shader for prey source code
const char* fragmentShaderPreySource = "#version 330 core\n"
"out vec4 FragColor;\n"
"uniform vec4 prey_color;\n"
"void main()\n"
"{\n"
"   FragColor = prey_color;\n"
"}\n\0";

//Fragment Shader for predators source code
const char* fragmentShaderPredSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"uniform vec4 pred_color;\n"
"void main()\n"
"{\n"
"   FragColor = pred_color;\n"
"}\n\0";

// Vertex Shader for background source code
const char* vertexShaderBGSource = "#version 330 core\n"
"layout (location = 0) in vec2 aPos;\n"
"uniform vec3 zoom;\n" //0,1 : x,y of center, 2: side/2
"void main()\n"
"{\n"
"	float x = (aPos.x - zoom.x)/zoom.z;\n"
"	float y = (aPos.y - zoom.y)/zoom.z;\n"
"   gl_Position = vec4(x, y, 0, 1.0);\n"
"}\0";

//Fragment Shader for background source code
const char* fragmentShaderBGSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"uniform vec4 bg_color;\n"
"void main()\n"
"{\n"
"   FragColor = bg_color;\n"
"}\n\0";

void drawBG(unsigned int shaderProgramBG, float* out_color) {

	// Specify the color of the background
	glClearColor(out_color[0], out_color[1], out_color[2], out_color[3]);
	// Clean the back buffer and assign the new color to it
	glClear(GL_COLOR_BUFFER_BIT);

#define corn_circ 100

	float bg_vertices [2 * corn_circ];

	for (int circler = 0; circler < corn_circ; circler++) {
		float ang = 2.0f * PI / corn_circ * circler;
		bg_vertices[circler * 2] = cos(ang);
		bg_vertices[circler * 2 + 1] = sin(ang);
	}

	unsigned int VBOBG, VAOBG;
	glGenBuffers(1, &VBOBG);
	glGenVertexArrays(1, &VAOBG);

	glBindVertexArray(VAOBG);

	glBindBuffer(GL_ARRAY_BUFFER, VBOBG);

	glBufferData(GL_ARRAY_BUFFER, sizeof(bg_vertices), bg_vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glUseProgram(shaderProgramBG);

	glDrawArrays(GL_TRIANGLE_FAN, 0, corn_circ);

	glDeleteBuffers(1, &VBOBG);
	glDeleteVertexArrays(1, &VAOBG);
}

int main(){

	// Initialize GLFW
	glfwInit();

	// Tell GLFW what version of OpenGL we are using 
	// In this case we are using OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Tell GLFW we are using the CORE profile
	// So that means we only have the modern functions
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Create a GLFWwindow object of 800 by 800 pixels, naming it "YoutubeOpenGL"
	GLFWwindow* window = glfwCreateWindow(800, 800, "Simulation window", NULL, NULL);
	// Error check if the window fails to create
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	// Introduce the window into the current context
	glfwMakeContextCurrent(window);

	//Load GLAD so it configures OpenGL
	gladLoadGL();
	// Specify the viewport of OpenGL in the Window
	// In this case the viewport goes from x = 0, y = 0, to x = 800, y = 800
	glViewport(0, 0, 800, 800);

	auto checkShaderSuccess = [&](int id) { // helper lambda to check for compilation error after compiling a shader
		int  success;
		char infoLog[512];
		glGetShaderiv(id, GL_COMPILE_STATUS, &success);

		if (!success) {
			glGetShaderInfoLog(id, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
		}
	};

	auto createProgram = [&](const char* sourceVertex, const char* sourceFragment) -> unsigned int{ // helper lambda to create shader programs froum source code of shaders
		// Create Vertex Shader Object and get its reference
		unsigned int localVertex = glCreateShader(GL_VERTEX_SHADER);
		// Attach Vertex Shader source to the Vertex Shader Object
		glShaderSource(localVertex, 1, &sourceVertex, NULL);
		// Compile the Vertex Shader into machine code
		glCompileShader(localVertex);

		checkShaderSuccess(localVertex);

		// Create Fragment Shader Object and get its reference
		unsigned int localFragment = glCreateShader(GL_FRAGMENT_SHADER);
		// Attach Fragment Shader source to the Vertex Shader Object
		glShaderSource(localFragment, 1, &sourceFragment, NULL);
		// Compile the Fragment Shader into machine code
		glCompileShader(localFragment);

		checkShaderSuccess(localFragment);

		// create program an attach shaders to it
		unsigned int localProgram = glCreateProgram();
		glAttachShader(localProgram, localVertex);
		glAttachShader(localProgram, localFragment);

		glLinkProgram(localProgram);

		glDeleteShader(localVertex);
		glDeleteShader(localFragment);

		return localProgram;
	};

	// Create Shader Programs
	unsigned int prey_shaderProgram = createProgram(vertexShaderAnimalSource, fragmentShaderPreySource);
	unsigned int pred_shaderProgram = createProgram(vertexShaderAnimalSource, fragmentShaderPredSource);
	unsigned int shaderProgramBG = createProgram(vertexShaderBGSource, fragmentShaderBGSource);

	// Get locations of uniforms in linked shader programs
	unsigned int prey_zoomLocation = glGetUniformLocation(prey_shaderProgram, "zoom");
	unsigned int pred_zoomLocation = glGetUniformLocation(pred_shaderProgram, "zoom");
	unsigned int zoomLocationBG = glGetUniformLocation(shaderProgramBG, "zoom");

	unsigned int prey_colorLocation = glGetUniformLocation(prey_shaderProgram, "prey_color");
	unsigned int pred_colorLocation = glGetUniformLocation(pred_shaderProgram, "pred_color");
	unsigned int bgcolorLocationBG = glGetUniformLocation(shaderProgramBG, "bg_color");

	// Initialize ImGUI
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");

	// Variables to be changed in the ImGUI window
	bool drawTriangle = true;
	float zoom[3] = { 0.0f, 0.0f, 1.0f };
	float prey_color[4] = { 0.0f, 1.0f, 0.0f, 1.0f };
	float pred_color[4] = { 1.0f, 0.0f, 0.0f, 1.0f };
	float bg_color[4] = { 0.07f, 0.13f, 0.17f, 1.0f };
	float out_color[4] = { 0.4f, 0.13f, 0.17f, 1.0f };

	auto sendUniform = [&]() {
		glUseProgram(prey_shaderProgram);
		glUniform3f(prey_zoomLocation, zoom[0], zoom[1], zoom[2]);
		glUniform4f(prey_colorLocation, prey_color[0], prey_color[1], prey_color[2], prey_color[3]);
		glUseProgram(pred_shaderProgram);
		glUniform3f(pred_zoomLocation, zoom[0], zoom[1], zoom[2]);
		glUniform4f(pred_colorLocation, pred_color[0], pred_color[1], pred_color[2], pred_color[3]);
		glUseProgram(shaderProgramBG);
		glUniform3f(zoomLocationBG, zoom[0], zoom[1], zoom[2]);
		glUniform4f(bgcolorLocationBG, bg_color[0], bg_color[1], bg_color[2], bg_color[3]);
	};

	sendUniform();

	std::list<std::shared_ptr<prey>> prey_vec;
	std::list<std::shared_ptr<predator>> pred_vec;

	// Main while loop
	while (!glfwWindowShouldClose(window)){

		// Tell OpenGL a new frame is about to begin
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		//SIMULATE

		if (simulating) {
			frame_steps += sim_step_per_frame;
			while (sim_steps < frame_steps) {
				sim_steps += 1;
				for (std::shared_ptr<predator> simulater : pred_vec) {
					simulater->step(prey_vec, pred_vec);
				}

				for (std::shared_ptr<prey> simulater : prey_vec) {
					simulater->step(pred_vec);
				}
			}
		}

		//RENDER
		
		drawBG(shaderProgramBG, out_color);

		if (simulating) { // draw out predators and prey if simulation is running

			// generate vertex array for prey
			int prey_corn = 8; // corners prey body should have
			float* prey_vertices = new float[prey_vec.size() * 2 * prey_corn];
			auto cur_prey = prey_vec.begin();
			for (int oct = 0; oct < prey_vec.size(); oct++){ // generatation of vertices: iterate through prey and for each draw polygon centered at x,y
				for (int i = 0; i < prey_corn; i++){
					prey_vertices[oct * 2 * prey_corn + i * 2] = (*cur_prey)->pos.first + prey_size * cos(2 * PI / prey_corn * i);
					prey_vertices[oct * 2 * prey_corn + i * 2 + 1] = (*cur_prey)->pos.second + prey_size * sin(2 * PI / prey_corn * i);
				}
				cur_prey++;
			}

			// generate index array for prey
			unsigned int* prey_indices = new unsigned int[prey_vec.size() * 3 * (prey_corn - 2)];
			for (int oct = 0; oct < prey_vec.size(); oct++) { // generation of indices: loop through polygons and for each connect first vertex to all pairs of consecutive vertices afterwards to form shape by triangles.
				for (int lower = 1; lower < prey_corn - 1; lower++){
					prey_indices[oct * 3 * (prey_corn - 2) + (lower - 1) * 3] = oct * prey_corn;
					prey_indices[oct * 3 * (prey_corn - 2) + (lower - 1) * 3 + 1] = oct * prey_corn + lower;
					prey_indices[oct * 3 * (prey_corn - 2) + (lower - 1) * 3 + 2] = oct * prey_corn + lower + 1;
				}
			}

			// generate vertex and index array for predator the same way as for prey ==> TODO: make lambda and only write code once
			int pred_corn = 6;
			float* pred_vertices = new float[pred_vec.size() * 2 * pred_corn];
			auto cur_pred = pred_vec.begin();
			for (int hex = 0; hex < pred_vec.size(); hex++) {
				for (int i = 0; i < pred_corn; i++) {
					pred_vertices[hex * 2 * pred_corn + i * 2] = (*cur_pred)->pos.first + pred_size * cos(2 * PI / pred_corn * i);
					pred_vertices[hex * 2 * pred_corn + i * 2 + 1] = (*cur_pred)->pos.second + pred_size * sin(2 * PI / pred_corn * i);
				}
				cur_pred++;
			}

			unsigned int* pred_indices = new unsigned int[pred_vec.size() * 3 * (pred_corn - 2)];
			for (int hex = 0; hex < pred_vec.size(); hex++) {
				for (int lower = 1; lower < pred_corn - 1; lower++) {
					pred_indices[hex * 3 * (pred_corn - 2) + (lower - 1) * 3] = hex * pred_corn;
					pred_indices[hex * 3 * (pred_corn - 2) + (lower - 1) * 3 + 1] = hex * pred_corn + lower;
					pred_indices[hex * 3 * (pred_corn - 2) + (lower - 1) * 3 + 2] = hex * pred_corn + lower + 1;
				}
			}


			// create vertex buffer, vertex array and element buffer from arrays created above to then use to be rendered
			unsigned int prey_VBO, prey_VAO, prey_EBO;
			unsigned int pred_VBO, pred_VAO, pred_EBO;

			glGenBuffers(1, &prey_VBO);
			glGenBuffers(1, &prey_EBO);
			glGenVertexArrays(1, &prey_VAO);

			glGenBuffers(1, &pred_VBO);
			glGenBuffers(1, &pred_EBO);
			glGenVertexArrays(1, &pred_VAO);


			glBindVertexArray(prey_VAO);

			glBindBuffer(GL_ARRAY_BUFFER, prey_VBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 2 * prey_corn * prey_vec.size(), prey_vertices, GL_STATIC_DRAW);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, prey_EBO);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(float) * 3 * (prey_corn - 2) * prey_vec.size(), prey_indices, GL_STATIC_DRAW);

			glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(0);

			glBindVertexArray(pred_VAO);

			glBindBuffer(GL_ARRAY_BUFFER, pred_VBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 2 * pred_corn * pred_vec.size(), pred_vertices, GL_STATIC_DRAW);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, pred_EBO);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(float) * 3 * (pred_corn - 2) * pred_vec.size(), pred_indices, GL_STATIC_DRAW);

			glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(0);

			// render out vertex array using respective shader program and element buffer
			glUseProgram(prey_shaderProgram);
			glBindVertexArray(prey_VAO);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, prey_EBO);
			glDrawElements(GL_TRIANGLES, 3 * (prey_corn - 2) * prey_vec.size(), GL_UNSIGNED_INT, 0);

			glUseProgram(pred_shaderProgram);
			glBindVertexArray(pred_VAO);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, pred_EBO);
			glDrawElements(GL_TRIANGLES, 3 * (pred_corn - 2) * pred_vec.size(), GL_UNSIGNED_INT, 0);

			// delete buffers and arrays as they will be created again next frame and no longer needed
			glDeleteBuffers(1, &prey_VBO);
			glDeleteBuffers(1, &prey_EBO);
			glDeleteVertexArrays(1, &prey_VAO);
			delete [] prey_vertices;
			delete [] prey_indices;

			glDeleteBuffers(1, &pred_VBO);
			glDeleteBuffers(1, &pred_EBO);
			glDeleteVertexArrays(1, &pred_VAO);
			delete [] pred_vertices;
			delete [] pred_indices;
		}

		// Info window
		ImGui::Begin("Info");

		ImGui::TextWrapped("This is an individual based predator simulation.");
		ImGui::TextWrapped("You can choose settings in the respective tabs. Tweak them until you are happy and then press \"Start Simulation\" to start a simulation.");
		ImGui::TextWrapped("Please also explore what you can do with these windows. You can for example merge them by hovering with one over the other and putting it on one of the blue indicators. You can then also adjust how much place each window gets.");
		ImGui::TextWrapped("Some sliders are not linear in case you are confused.");
		ImGui::TextWrapped("The fps are limited to your screens max fps so if the simulation runs slowly for you you might want to increase the simulation steps per frame");

		ImGui::TextWrapped("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		

		// Visual options menu
		ImGui::Begin("Visual options");

		ImGui::ColorEdit4("Predator color", pred_color);
		ImGui::ColorEdit4("Prey color", prey_color);
		ImGui::ColorEdit4("Border color", out_color);
		ImGui::ColorEdit4("Background color", bg_color);

		ImGui::SliderFloat("Prey size", &prey_size, 0.0f, 0.1f);
		ImGui::SliderFloat("Predator size", &pred_size, 0.0f, 0.1f);

		ImGui::Text("Hello there adventurer!");
		// Checkbox that appears in the window
		ImGui::Checkbox("Draw Triangle", &drawTriangle);
		// Ends the window
		ImGui::End();

		// Pre-Simulation menu
		ImGui::Begin("Simulation options");
		ImGui::SliderInt("Initial prey", &init_prey, 0, 100);
		ImGui::SliderInt("Initial predators", &init_pred, 0, 100);
		if (ImGui::Button("Start Simulation")) { 

			// generate fresh prey and predators
			prey_vec.clear();
			pred_vec.clear();
			for (int prey_gen = 0; prey_gen < init_prey; prey_gen++) prey_vec.insert(prey_vec.end(), std::shared_ptr<prey> (new prey()));
			for (int pred_gen = 0; pred_gen < init_pred; pred_gen++) pred_vec.insert(pred_vec.end(), std::shared_ptr<predator>(new predator()));

			simulating = true;
			sim_steps = 0;
			frame_steps = 0;
		}

		ImGui::SliderFloat("Predator speed", &pred_speed, 0.0f, 1.0f);
		ImGui::SliderFloat("Predator vision range", &pred_see_range, 0.0f, 1.0f);

		ImGui::SliderFloat("Prey speed", &prey_speed, 0.0f, 1.0f);
		ImGui::SliderFloat("Prey vision range", &prey_see_range, 0.0f, 1.0f);

		ImGui::SliderFloat("Predator idle speed %", &idle_slow, 0.0f, 1.0f);
		ImGui::SliderFloat("Predator idle direction change", &idle_dir_c, 0.0f, 1.0f, "%.3f", ImGuiSliderFlags_Logarithmic);

		float o_sim_step_per_frame = sim_step_per_frame;
		ImGui::SliderFloat("Simulation steps per frame", &sim_step_per_frame, 0.0f, 500.0f, "%.3f", ImGuiSliderFlags_Logarithmic);
		if (o_sim_step_per_frame != sim_step_per_frame) {
			sim_steps = 0; frame_steps = 0;
		}

		ImGui::End();

		sendUniform();

		// Renders the ImGUI elements
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			GLFWwindow* backup_current_context = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(backup_current_context);
		}

		// Swap the back buffer with the front buffer
		glfwSwapBuffers(window);
		// Take care of all GLFW events
		glfwPollEvents();
	}

	// Deletes all ImGUI instances
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	// Delete all the objects we've created
	glDeleteProgram(prey_shaderProgram);
	glDeleteProgram(pred_shaderProgram);
	glDeleteProgram(shaderProgramBG);
	// Delete window before ending the program
	glfwDestroyWindow(window);
	// Terminate GLFW before ending the program
	glfwTerminate();

	return 0;
}