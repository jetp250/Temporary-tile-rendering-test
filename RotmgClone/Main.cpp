
#include <glad/glad.h>
#include <glfw3.h>

#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/vec3.hpp>

#include "GLShader.h"

#include "ImageIO.h"

GLFWwindow* createWindow() {
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_SAMPLES, 0);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	//glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE);
	glfwWindowHint(GLFW_DECORATED, GLFW_TRUE);

	GLFWmonitor* monitor = glfwGetPrimaryMonitor();

	unsigned width, height;
	{ // Limit scope
		const GLFWvidmode* videomode = glfwGetVideoMode(monitor);
		width = videomode->width;
		height = videomode->height;
		glfwWindowHint(GLFW_RED_BITS, videomode->redBits);
		glfwWindowHint(GLFW_GREEN_BITS, videomode->greenBits);
		glfwWindowHint(GLFW_BLUE_BITS, videomode->blueBits);
		glfwWindowHint(GLFW_REFRESH_RATE, videomode->refreshRate);
	}
	width = 800;
	height = 800;

	GLFWwindow* window = glfwCreateWindow(width, height, "Application", nullptr, nullptr);

	glfwMakeContextCurrent(window);
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

	glfwSwapInterval(0); // Vsync

	glViewport(0, 0, width, height);

	return window;
}

int main() {
	GLFWwindow* window = createWindow();
	glfwSwapInterval(1);

	glEnable(GL_DEPTH_TEST);

	glh::shader shader("assets/shaders/shader.vs", "assets/shaders/shader.fs");
	shader.bind();

	unsigned texID = loadTexture("assets/textures/wand.png");
	glBindTexture(GL_TEXTURE_2D, texID);
	glActiveTexture(GL_TEXTURE0);

	shader.setInt("tex", 0);

	unsigned vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	unsigned vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	GLfloat vertices[]{
		0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
		1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
		1.0f, 0.0f, 0.2f, 1.0f, 1.0f,
	};

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);
	
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), 0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glm::mat4 proj = glm::ortho(-400.0f, 400.0f, -400.0f, 400.0f, -400.0f, 400.0f);
	//glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 0.0001f, 0.5f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));

	//glm::mat4 model(1.0f);
	//model = glm::scale(model, glm::vec3(100.0f));
	//model = glm::translate(model, glm::vec3(10.0f, 10.0f, -0.01f));

	float angle = 0.0f;

	float time = (float)glfwGetTime();

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	while (!glfwWindowShouldClose(window)) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glfwPollEvents();

		float ctime = (float)glfwGetTime();
		float dt = ctime - time;
		time = ctime;

		if (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_RELEASE) {
			glfwSetWindowShouldClose(window, GLFW_TRUE);
			break;
		}

		int rot = 0;
		if (glfwGetKey(window, GLFW_KEY_Q) != GLFW_RELEASE)
			rot -= 1;
		if (glfwGetKey(window, GLFW_KEY_E) != GLFW_RELEASE)
			rot += 1;

		if (rot != 0) {
			constexpr float rotSpeed = 120.0f;
			proj = glm::rotate(proj, glm::radians(rot * rotSpeed * dt), glm::vec3(0.0f, 0.0f, 1.0f));

			angle += rot * rotSpeed * dt;
		}

		int velX = 0;
		int velY = 0;
		if (glfwGetKey(window, GLFW_KEY_A) != GLFW_RELEASE)
			velX -= 1;
		if (glfwGetKey(window, GLFW_KEY_D) != GLFW_RELEASE)
			velX += 1;
		if (glfwGetKey(window, GLFW_KEY_W) != GLFW_RELEASE)
			velY += 1;
		if (glfwGetKey(window, GLFW_KEY_S) != GLFW_RELEASE)
			velY -= 1;

		if (velX || velY) {
			glm::vec3 vel(velX, velY, 0);
			glm::mat4 rot = glm::rotate(glm::mat4(1.0f), glm::radians(angle + 180.0f), glm::vec3(0.0f, 0.0f, 1.0f));
			vel = vel * glm::mat3(rot);

			constexpr float movSpeed = 250.0f;
			proj = glm::translate(proj, (movSpeed * dt) * vel);
		}

		for (int i = 0; i < 2; ++i) {
			for (int j = 0; j < 2; ++j) {
				glm::mat4 model(1.0f);
				model = glm::scale(model, glm::vec3(100.0f));
				model = glm::translate(model, glm::vec3((i-1)*2.0f, (j-1)*2.0f, 0.0f));

				glm::mat4 pvm = proj * model;
				shader.setMat4("pvm", pvm);
				glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

				model = glm::mat4(1.0f);
				model = glm::scale(model, glm::vec3(100.0f));
				model = glm::translate(model, glm::vec3((i-1)*2.0f + 1.0f, (j-1)*2.0f + 1.0f, -2.0f));

				pvm = proj * model;
				shader.setMat4("pvm", pvm);
				glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
			}
		}

		glfwSwapBuffers(window);
	}
	glfwTerminate();
}