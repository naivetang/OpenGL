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

	Shader ourShader("ModelVShader.glsl", "ModelFShader.glsl");

	Shader lightingShader("Vshader.glsl", "Fshader.glsl");

	Shader lampShader("Vshader.glsl", "LightShader.glsl");

	vec3 dirLightPos = vec3(0.2f, 2.0f, 0.0f);

	vec3 dirLightDir = vec3(0, 1, 0);

	vec3 spotLightPos = vec3(0.0f, 0.0f, -5.0f);

	vec3 spotLightDir = vec3(0.0f, 0.8f, 1.0f);


	lightingShader.user();

	lightingShader.setVec3("objectColor", 1.0f, 1.0f, 1.0f);

	lightingShader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);

	model = mat4(1);

	lightingShader.setFloat("material.shininess", 32);

	lampShader.user();

	model = mat4(1.0f);

	model = translate(model, dirLightPos);

	model = scale(model, vec3(0.2f));



	lampShader.setMat4("model", model);


	Model ourModel("D:/Github/OpenGL/Class21  Mesh/Model/nanosuit.obj");

	// 立方体顶点数据
	float vertices[] = {
		// positions          // normals           // texture coords
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
	};

	glm::vec3 cubePositions[] = {
glm::vec3(0.0f,  0.0f,  0.0f),
glm::vec3(2.0f,  5.0f, -15.0f),
glm::vec3(-1.5f, -2.2f, -2.5f),
glm::vec3(-3.8f, -2.0f, -12.3f),
glm::vec3(2.4f, -0.4f, -3.5f),
glm::vec3(-1.7f,  3.0f, -7.5f),
glm::vec3(1.3f, -2.0f, -2.5f),
glm::vec3(1.5f,  2.0f, -2.5f),
glm::vec3(1.5f,  0.2f, -1.5f),
glm::vec3(-1.3f,  1.0f, -1.5f)
	};

	// 点光源位置数据
	glm::vec3 pointLightPositions[] = {
	glm::vec3(0.7f,  0.2f,  2.0f),
	glm::vec3(2.3f, -3.3f, -4.0f),
	glm::vec3(-4.0f,  2.0f, -12.0f),
	glm::vec3(0.0f,  0.0f, -3.0f)
	};



	// Cube的顶点数据
	unsigned int VBO, cubeVAO;
	glGenVertexArrays(1, &cubeVAO);
	glBindVertexArray(cubeVAO);


	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);


	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// normal attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// texture coords attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	// Light的顶点数据
	unsigned int lightVAO;
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);

	// we only need to bind to the VBO (to link it with glVertexAttribPointer), no need to fill it; the VBO's data already contains all we need (it's already bound, but we do it again for educational purposes)
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glEnable(GL_DEPTH_TEST);

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

		// ourShader.user();
		//
		// // view/projection transformations
		// glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		// glm::mat4 view = camera.GetViewMatrix();
		// ourShader.setMat4("projection", projection);
		// ourShader.setMat4("view", view);
		//
		// // render the loaded model
		// glm::mat4 model = glm::mat4(1.0f);
		// model = glm::translate(model, glm::vec3(0.0f, -1.75f, 0.0f)); // translate it down so it's at the center of the scene
		// model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));	// it's a bit too big for our scene, so scale it down
		// ourShader.setMat4("model", model);
		// ourModel.Draw(ourShader);
		//
		//
		// // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// // -------------------------------------------------------------------------------
		// glfwSwapBuffers(window);
		// glfwPollEvents();
		
		//continue;

		

		lightingShader.user();

		model = mat4(1);

		model = glm::translate(model, glm::vec3(0.0f, -1.75f, 0.0f)); // translate it down so it's at the center of the scene
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));	// it's a bit too big for our scene, so scale it down
		
		lightingShader.setMat4("model", model);

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
		for (unsigned int i = 0; i < 10; i++)
		{
			model = mat4(1);

			// 设置定向光源

			lightingShader.setVec3("dirLight.direction", dirLightDir);
			lightingShader.setVec3("dirLight.ambient", 0.2, 0.2, 0.2);
			lightingShader.setVec3("dirLight.diffuse", 0.5, 0.5, 0.5);
			lightingShader.setVec3("dirLight.specular", 1, 1, 1);

			// 设置点光源

			for (int i = 0; i < 4; i++)
			{
				std::string str = "pointLights[";
				str += std::to_string(i);
				str += "]";

				lightingShader.setVec3(str + ".position", pointLightPositions[i]);
				lightingShader.setVec3(str + ".ambient", 0.2, 0.2, 0.2);
				lightingShader.setVec3(str + ".diffuse", 0.5, 0.5, 0.5);
				lightingShader.setVec3(str + ".specular", 1, 1, 1);

				lightingShader.setFloat(str + ".constant", 1.0f);
				lightingShader.setFloat(str + ".linear", 0.09f);
				lightingShader.setFloat(str + ".quadratic", 0.032f);
			}

			// 设置聚光源

			lightingShader.setVec3("spotLight.position", spotLightPos);

			lightingShader.setVec3("spotLight.direction", spotLightDir);
			lightingShader.setFloat("spotLight.inCutOff", cos(12.5f));
			lightingShader.setFloat("spotLight.outerCutOff", cos(17.5f));


			lightingShader.setVec3("spotLight.ambient", 0.2, 0.2, 0.2);
			lightingShader.setVec3("spotLight.diffuse", 0.5, 0.5, 0.5);
			lightingShader.setVec3("spotLight.specular", 1, 1, 1);

		}
		ourModel.Draw(lightingShader);

		// 绘制光源

		// 绘制 定向光源

		// 白色

		lampShader.user();

		model = mat4(1.0f);

		model = translate(model, dirLightPos);

		model = scale(model, vec3(0.5f));

		lampShader.setMat4("model", model);

		lampShader.setMat4("projection", projection);

		lampShader.setMat4("view", view);

		lampShader.setVec3("color", 1, 1, 1);

		glBindVertexArray(lightVAO);

		glDrawArrays(GL_TRIANGLES, 0, 36);

		// 绘制 点光源		

		// 绿色

		for (int i = 0; i < 4; i++)
		{
			model = mat4(1);

			model = translate(model, pointLightPositions[i]);

			model = scale(model, vec3(0.2f));

			lampShader.setMat4("model", model);

			lampShader.setVec3("color", 0, 1, 0);

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		// 绘制聚光源

		// 红色

		model = mat4(1);

		model = translate(model, spotLightPos);

		model = scale(model, vec3(0.1f));

		lampShader.setMat4("model", model);

		lampShader.setVec3("color", 1, 0, 0);

		glDrawArrays(GL_TRIANGLES, 0, 36);

		 glfwSwapBuffers(window);
		 glfwPollEvents();
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