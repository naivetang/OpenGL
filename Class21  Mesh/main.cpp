#pragma comment(lib,"glfw3.lib")
#pragma comment(lib,"opengl32.lib")

#pragma comment(lib,"assimpd.lib")

#define	STB_IMAGE_IMPLEMENTATION
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "Shader.h"
#include "camera.h"

#include <filesystem>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Mesh.h"
#include "Model.h"

using namespace glm;
using namespace  std;



void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void processInput(GLFWwindow *window);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

unsigned int loadTexture(char const * path);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

float lastX = SCR_WIDTH / 2, lastY = SCR_HEIGHT / 2;

float deltaTime = 0.0f;

float lastFrame = 0.0f;

Camera camera(vec3(0,0,3));



int main()
{
	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

	// glfw window creation
	// --------------------
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	glfwSetCursorPosCallback(window, mouse_callback);

	glfwSetScrollCallback(window, scroll_callback);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	

	mat4 model, view, projection;

	Shader lightingShader("Vshader.glsl", "Fshader.glsl");

	Shader lampShader("Vshader.glsl", "LightShader.glsl");

	vec3 dirLightPos = vec3(0.2f, 2.0f, 0.0f);

	vec3 dirLightDir = vec3(-0.2f, -1.0f, -0.3f);

	vec3 spotLightPos = vec3(0.0f, 0.0f, -5.0f);

	vec3 spotLightDir = vec3(0.0f, 0.8f, 1.0f);


	lightingShader.user();

	lightingShader.setVec3("objectColor", 1.0f, 1.0f, 1.0f);

	lightingShader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);

	model = mat4(1);

	// model = rotate(model, radians(-30.0f), vec3(1, 0, 0));
	//
	// model = rotate(model, radians(-30.0f), vec3(0, 0, 1));

	// 上一章的效果：
	// lightingShader.setVec3("material.ambient", 0.1, 0.1, 0.1);
	// lightingShader.setVec3("material.diffuse", 1.0, 1.0, 1.0);
	// lightingShader.setVec3("material.specular", 1.0, 1, 1);
	// lightingShader.setFloat("material.shininess", 32);

	//lightingShader.setVec3("material.ambient", 1.0f, 0.5f, 0.31f);
	//lightingShader.setVec3("material.diffuse", 1.0f, 0.5f, 0.31f);


	lightingShader.setInt("material.diffuse", 0);
	//lightingShader.setVec3("material.specular", 0.5f, 0.5f, 0.5f);
	lightingShader.setInt("material.specular", 1);
	lightingShader.setFloat("material.shininess", 32);

	// lightingShader.setVec3("light.ambient", 0.2, 0.2, 0.2);
	// lightingShader.setVec3("light.diffuse", 0.5, 0.5, 0.5);
	// lightingShader.setVec3("light.specular", 1, 1, 1);
	// lightingShader.setVec3("light.direction", -0.2f, -1.0f, -0.3f);
	//
	//
	// lightingShader.setFloat("light.constant", 1.0f);
	// lightingShader.setFloat("light.linear", 0.09f);
	// lightingShader.setFloat("light.quadratic", 0.032f);

	lampShader.user();

	model = mat4(1.0f);

	model = translate(model, dirLightPos);

	model = scale(model, vec3(0.2f));

	// model = rotate(model, radians(-30.0f), vec3(1, 0, 0));
	//
	// model = rotate(model, radians(-30.0f), vec3(0, 0, 1));

	lampShader.setMat4("model", model);


	Model Model("Model/nanosuit.obj");

	// render loop
	// -----------
	while (!glfwWindowShouldClose(window))
	{
		// input
		// -----
		processInput(window);

		// render
		// ------
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		

		lightingShader.user();

		projection = perspective(radians(camera.Zoom), (float)SCR_WIDTH / SCR_HEIGHT, 1.0f, 100.0f);

		lightingShader.setMat4("projection", projection);

		view = camera.GetViewMatrix();

		lightingShader.setMat4("view", view);

		vec3 viewPose = camera.Position;

		lightingShader.setVec3("viewPose", viewPose.x, viewPose.y, viewPose.z);

		// 聚光源

		// 动态修改光源

		//std::cout << camera.Front.x << "," << camera.Front.y << camera.Front.z << std::endl;

		lightingShader.setVec3("light.direction", camera.Front);

		lightingShader.setVec3("light.position", camera.Position);

		lightingShader.setFloat("light.inCutOff", cos(12.5f));
		lightingShader.setFloat("light.outCutOff", cos(17.5f));

		// glm::vec3 lightColor;
		// lightColor.x = sin(glfwGetTime() * 2.0f);
		// lightColor.y = sin(glfwGetTime() * 0.7f);
		// lightColor.z = sin(glfwGetTime() * 1.3f);
		//
		// glm::vec3 diffuseColor = lightColor * glm::vec3(0.5f); // 降低影响
		// glm::vec3 ambientColor = diffuseColor * glm::vec3(0.2f); // 很低的影响
		//
		//
		// lightingShader.setVec3("light.ambient", ambientColor.x, ambientColor.y, ambientColor.z);
		// lightingShader.setVec3("light.diffuse", diffuseColor.x, diffuseColor.y, diffuseColor.z);
		//
		Model.Draw(lightingShader);


	}


	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	glfwTerminate();
	return 0;
}



// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	deltaTime = glfwGetTime() - lastFrame;

	lastFrame = glfwGetTime();


	//std::cout << deltaTime << std::endl;
	
	// 摄像机位移

	if(glfwGetKey(window, GLFW_KEY_W))
	{
		camera.ProcessKeyboard(FORWARD, deltaTime);
	}

	if (glfwGetKey(window, GLFW_KEY_S))
	{
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	}

	if (glfwGetKey(window, GLFW_KEY_D))
	{
		camera.ProcessKeyboard(RIGHT, deltaTime);
	}

	if (glfwGetKey(window, GLFW_KEY_A))
	{
		camera.ProcessKeyboard(LEFT, deltaTime);

		//std::cout << normalize(cross(cameraFront, cameraUp)).x << "," << normalize(cross(cameraFront, cameraUp)).y << "," << normalize(cross(cameraFront, cameraUp)).z << std::endl;
	}

	// 摄像机转动

	// 隐藏鼠标
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);


}

bool firstMouse = true;

// 监听鼠标移动
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

glm::mat4 calculate_lookAt_matrix(glm::vec3 position, glm::vec3 target, glm::vec3 worldUp)
{
	// 1. Position = known
	// 2. Calculate cameraDirection
	glm::vec3 zaxis = glm::normalize(position - target);
	// 3. Get positive right axis vector
	glm::vec3 xaxis = glm::normalize(glm::cross(glm::normalize(worldUp), zaxis));
	// 4. Calculate camera up vector
	glm::vec3 yaxis = glm::cross(zaxis, xaxis);

	// Create translation and rotation matrix
	// In glm we access elements as mat[col][row] due to column-major layout
	glm::mat4 translation = glm::mat4(1.0f); // Identity matrix by default
	translation[3][0] = -position.x; // Third column, first row
	translation[3][1] = -position.y;
	translation[3][2] = -position.z;
	glm::mat4 rotation = glm::mat4(1.0f);
	rotation[0][0] = xaxis.x; // First column, first row
	rotation[1][0] = xaxis.y;
	rotation[2][0] = xaxis.z;
	rotation[0][1] = yaxis.x; // First column, second row
	rotation[1][1] = yaxis.y;
	rotation[2][1] = yaxis.z;
	rotation[0][2] = zaxis.x; // First column, third row
	rotation[1][2] = zaxis.y;
	rotation[2][2] = zaxis.z;

	// Return lookAt matrix as combination of translation and rotation matrix
	return rotation * translation; // Remember to read from right to left (first translation then rotation)
}

unsigned int loadTexture(char const * path)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}

	return textureID;
}