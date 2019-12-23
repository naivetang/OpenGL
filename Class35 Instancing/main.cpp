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
void Render(GLFWwindow* window, Shader lightingShader, mat4 model, mat4 projection, mat4 view, vec3 dirLightDir, vec3 *pointLightPositions, vec3 spotLightPos, vec3 spotLightDir, Model ourModel);

unsigned int loadTexture(char const * path);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

float lastX = SCR_WIDTH / 2, lastY = SCR_HEIGHT / 2;

float deltaTime = 0.0f;

float lastFrame = 0.0f;

Camera camera(vec3(0,0,6));

unsigned int loadCubemap(vector<string> faces)
{
	unsigned int textureID;

	glGenTextures(1, &textureID);

	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	int width, height, nrChannels;

	for(unsigned int i = 0; i < 6; ++i)
	{
		unsigned char * data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);

		if(data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		}
		else
		{
			cout << "load error : " << faces[i].c_str() << endl;			
		}

		stbi_image_free(data);
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	return textureID;
}

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


	float quadVertices[] = {
		// λ��          // ��ɫ
		-0.05f,  0.05f,  1.0f, 0.0f, 0.0f,	// ����
		 0.05f, -0.05f,  0.0f, 1.0f, 0.0f,	// ����
		-0.05f, -0.05f,  0.0f, 0.0f, 1.0f,	// ����

		-0.05f,  0.05f,  1.0f, 0.0f, 0.0f,	// ����
		 0.05f, -0.05f,  0.0f, 1.0f, 0.0f,	// ����
		 0.05f,  0.05f,  0.0f, 1.0f, 1.0f	// ����
	};

	unsigned int quadVAO, quadVBO;

	glGenVertexArrays(1, &quadVAO);
	glBindVertexArray(quadVAO);

	glGenBuffers(1, &quadVBO);
	glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GL_FLOAT), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GL_FLOAT), (void*)(2 * sizeof(GL_FLOAT)) );
	glEnableVertexAttribArray(1);

	

	vec2 offsets[100];

	int index = 0;

	float offset1 = 0.1f;

	for(int y = -10; y < 10; y +=2)
	{
		for (int x = -10; x < 10; x += 2)
		{
			vec2 t;

			t.x = (float)x / 10.0f + offset1;

			t.y = (float)y / 10.0f + offset1;

			offsets[index++] = t;
		}
	}

	Shader InstanceShader("InstanceVShader.glsl", "InstanceFShader.glsl");

	InstanceShader.user();

	// for (unsigned int i = 0; i < 100; i++)
	// {
	// 	stringstream ss;
	// 	string index;
	// 	ss << i;
	// 	index = ss.str();
	// 	InstanceShader.setVec2(("offect[" + index + "]").c_str(), offsets[i]);
	// }


	unsigned int instanceVBO;
	glGenBuffers(1, &instanceVBO);
	glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vec2) * 100, &offsets[0], GL_STATIC_DRAW);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(2);

	/*�������������OpenGL��ʲôʱ����¶������Ե���������һ�����ݡ�
	 *���ĵ�һ����������Ҫ�Ķ������ԣ��ڶ������������Գ���(Attribute Divisor)��
	 *Ĭ������£����Գ�����0������OpenGL������Ҫ�ڶ�����ɫ����ÿ�ε���ʱ���¶������ԡ�
	 *��������Ϊ1ʱ�����Ǹ���OpenGL����ϣ������Ⱦһ����ʵ����ʱ����¶������ԡ�
	 *������Ϊ2ʱ������ϣ��ÿ2��ʵ������һ�����ԣ�
	 *�Դ����ơ����ǽ����Գ�������Ϊ1�����ڸ���OpenGL������λ��ֵ2�Ķ���������һ��ʵ�������顣*/
	glVertexAttribDivisor(2, 1);


	//TODO::����
	unsigned int amount = 100000;
	mat4 * modelMatrices = new mat4[amount];
	srand(glfwGetTime());
	float radius = 50.0f;
	float offset = 2.5f;
	for (unsigned int i = 0; i < amount; i++)
	{
		glm::mat4 model;
		// 1. λ�ƣ��ֲ��ڰ뾶Ϊ 'radius' ��Բ���ϣ�ƫ�Ƶķ�Χ�� [-offset, offset]
		float angle = (float)i / (float)amount * 360.0f;
		float displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
		float x = sin(angle) * radius + displacement;
		displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
		float y = displacement * 0.4f; // �����Ǵ��ĸ߶ȱ�x��z�Ŀ��ҪС
		displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
		float z = cos(angle) * radius + displacement;
		model = glm::translate(model, glm::vec3(x, y, z));

		// 2. ���ţ��� 0.05 �� 0.25f ֮������
		float scale = (rand() % 20) / 100.0f + 0.05;
		model = glm::scale(model, glm::vec3(scale));

		// 3. ��ת������һ�����룩���ѡ�����ת�����������������ת
		float rotAngle = (rand() % 360);
		model = glm::rotate(model, rotAngle, glm::vec3(0.4f, 0.6f, 0.8f));

		// 4. ��ӵ������������
		modelMatrices[i] = model;
	}

	Model rock("D:/Github/OpenGL/Class35 Instancing/Rock/rock.obj");

	Model planet("D:/Github/OpenGL/Class35 Instancing/Planet/planet.obj");

	Shader rockShader("InstanceModelVShader.glsl", "ModelFShader.glsl");
	Shader planetShader("ModelVShader.glsl", "ModelFShader.glsl");

	// ���㻺�����
	unsigned int buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, amount * sizeof(glm::mat4), &modelMatrices[0], GL_STATIC_DRAW);

	for (unsigned int i = 0; i < rock.meshes.size(); i++)
	{
		unsigned int VAO = rock.meshes[i].VAO;
		glBindVertexArray(VAO);
		// ��������
		GLsizei vec4Size = sizeof(glm::vec4);

		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)0);

		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(vec4Size));

		glEnableVertexAttribArray(5);
		glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(2 * vec4Size));

		glEnableVertexAttribArray(6);
		glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(3 * vec4Size));

		glVertexAttribDivisor(3, 1);
		glVertexAttribDivisor(4, 1);
		glVertexAttribDivisor(5, 1);
		glVertexAttribDivisor(6, 1);

		glBindVertexArray(0);

		cout << "update:" << i << endl;
	}

	

	glEnable(GL_DEPTH_TEST);

	while (!glfwWindowShouldClose(window))
	{
		//glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
		
		//glBindFramebuffer(GL_FRAMEBUFFER, 0);
		//float time = glfwGetTime();
		processInput(window);

		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// ��������
		
		glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 1000.0f);
		glm::mat4 view = camera.GetViewMatrix();;
		glm::mat4 model;
		model = glm::translate(model, glm::vec3(0.0f, -3.0f, 0.0f));
		model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));


		planetShader.user();

		planetShader.setMat4("model", model);
		planetShader.setMat4("projection", projection);
		planetShader.setMat4("view", view);

		planet.Draw(planetShader);


		//shader.setMat4("model", model);
		//planet.Draw(shader);

		// ����С����
		// for (unsigned int i = 0; i < amount; i++)
		// {
		// 	//shader.setMat4("model", modelMatrices[i]);
		// 	rock.Draw(shader);
		// }

		rockShader.user();

		rockShader.setMat4("projection", projection);
		rockShader.setMat4("view", view);

		for(unsigned int i = 0; i < rock.meshes.size(); ++i)
		{
			glBindVertexArray(rock.meshes[i].VAO);

			glDrawElementsInstanced(GL_TRIANGLES, rock.meshes[i].indices.size(), GL_UNSIGNED_INT, 0, amount);
		}


		//glBindVertexArray(quadVAO);

		//glDrawArraysInstanced(GL_TRIANGLES, 0, 6, 100);
		//glDrawArrays(GL_TRIANGLES, 0, 6);
		
		glfwSwapBuffers(window);
		glfwPollEvents();

	}


	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	glfwTerminate();

	return 0;
}

void Render(GLFWwindow* window, Shader lightingShader, mat4 model, mat4 projection, mat4 view, vec3 dirLightDir, vec3 *pointLightPositions, vec3 spotLightPos, vec3 spotLightDir, Model ourModel)
{
	// input
// -----
	processInput(window);

	// render
	// ------
	

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

	

	lightingShader.setMat4("model", model);

	projection = perspective(radians(camera.Zoom), (float)SCR_WIDTH / SCR_HEIGHT, 1.0f, 100.0f);

	lightingShader.setMat4("projection", projection);

	view = camera.GetViewMatrix();

	lightingShader.setMat4("view", view);

	vec3 viewPose = camera.Position;

	lightingShader.setVec3("viewPose", viewPose.x, viewPose.y, viewPose.z);

	// �۹�Դ

	// ��̬�޸Ĺ�Դ

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
	// glm::vec3 diffuseColor = lightColor * glm::vec3(0.5f); // ����Ӱ��
	// glm::vec3 ambientColor = diffuseColor * glm::vec3(0.2f); // �ܵ͵�Ӱ��
	//
	//
	// lightingShader.setVec3("light.ambient", ambientColor.x, ambientColor.y, ambientColor.z);
	// lightingShader.setVec3("light.diffuse", diffuseColor.x, diffuseColor.y, diffuseColor.z);
	//
	for (unsigned int i = 0; i < 10; i++)
	{
		model = mat4(1);

		// ���ö����Դ

		lightingShader.setVec3("dirLight.direction", dirLightDir);
		lightingShader.setVec3("dirLight.ambient", 0.2, 0.2, 0.2);
		lightingShader.setVec3("dirLight.diffuse", 0.5, 0.5, 0.5);
		lightingShader.setVec3("dirLight.specular", 1, 1, 1);

		// ���õ��Դ

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

		// ���þ۹�Դ

		lightingShader.setVec3("spotLight.position", spotLightPos);

		lightingShader.setVec3("spotLight.direction", spotLightDir);
		lightingShader.setFloat("spotLight.inCutOff", cos(12.5f));
		lightingShader.setFloat("spotLight.outerCutOff", cos(17.5f));


		lightingShader.setVec3("spotLight.ambient", 0.2, 0.2, 0.2);
		lightingShader.setVec3("spotLight.diffuse", 0.5, 0.5, 0.5);
		lightingShader.setVec3("spotLight.specular", 1, 1, 1);

	}
	ourModel.Draw(lightingShader);
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
	
	// �����λ��

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

	// �����ת��

	// �������
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);


}

bool firstMouse = true;

// ��������ƶ�
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