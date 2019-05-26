#pragma comment(lib,"glfw3.lib")
#pragma comment(lib,"opengl32.lib")

#define	STB_IMAGE_IMPLEMENTATION

#include  "stb_image.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "Shader.h"

int main()
{

	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);

	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

	auto Window = glfwCreateWindow(800, 600, "LearnOpenGL", nullptr, nullptr);

	if (Window == nullptr)
	{
		std::cout << "Faild" << std::endl;

		glfwTerminate();

		return -1;
	}

	// ���˴�������Ϊ�����Ĵ���
	glfwMakeContextCurrent(Window);


	// ��gl��ȡ�Ĵ��ڵ�ַ������glad�� 
	// load all OpenGL function pointers
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Faild" << std::endl;

		return -1;
	}

	// �ӿڴ�С
	glViewport(0, 0, 800, 600);

	// ��ȡ��߶ȡ���ɫͨ������
	int width, height, nrChannels;

	unsigned char *data = stbi_load("container.jpg", &width, &height, &nrChannels, 0);

	unsigned int texture;

	// ��һ���������������������
	glGenTextures(1, &texture);

	// ��texture����GL_TEXTURE_2D���ú����GL_TEXTURE_2D��ָ��󶨵�texture������
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	if(data)
	{
		// ����ǰ�������ͼƬ��������
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	// �ͷ�ͼ��
	stbi_image_free(data);

	float vertices[] = {
		//     ---- λ�� ----       ---- ��ɫ ----     - �������� -
			 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // ����
			 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // ����
			-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // ����
			-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // ����
	};

	unsigned int indices[] =
	{
		0, 1, 3, // ��һ��������
		1, 2, 3 // �ڶ���������
	};

	glBindTexture(GL_TEXTURE_2D, texture);

	Shader shadeProgram("shader.vs", "shader.fs");

	shadeProgram.user();

	// �����������
	unsigned int VAO;

	glGenVertexArrays(1, &VAO);

	glBindVertexArray(VAO);

	// ���㻺���������	

	unsigned int VBO;

	glGenBuffers(1, &VBO);

	// ���㻺�����Ļ���������GL_ARRAY_BUFFER
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	// glBufferData��һ��ר���������û���������ݸ��Ƶ���ǰ�󶨻���ĺ���
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// 

	unsigned int EBO;

	glGenBuffers(1, &EBO);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// texture coord attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
	
	// ��Ⱦѭ��(Render Loop)
	while (!glfwWindowShouldClose(Window))
	{

		// ��Ⱦָ��
		// ��״̬���á�����
		glClearColor(0.2f, 0.3f, 0.3f, 10.f);

		// ��״̬ʹ�á�����
		// �����ɫ����֮��������ɫ���嶼�������ΪglClearColor�����õ���ɫ
		glClear(GL_COLOR_BUFFER_BIT);

		//glBindTexture(GL_TEXTURE_2D, texture);

		// 
		//glBindVertexArray(VAO);

		// ����ͼ��
		//glDrawArrays(GL_TRIANGLES, 0, 3);

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);


		// ����Ƿ����¼����������̡���꣬���´���״̬
		glfwPollEvents();

		//˫����
		//ǰ���屣�������������ͼ����������Ļ����ʾ�������еĵ���Ⱦָ����ں󻺳��ϻ��ơ�
		//�����е���Ⱦָ��ִ����Ϻ����ǽ���(Swap)ǰ����ͺ󻺳壬����ͼ����������Գ�����֮ǰ�ᵽ�Ĳ���ʵ�о������ˡ�

		// ������ɫ���壨����һ��������GLFW����ÿһ��������ɫֵ�Ĵ󻺳壩��������һ�����б��������ƣ����ҽ�����Ϊ�����ʾ����Ļ��
		glfwSwapBuffers(Window);
	}

	// ������Դ
	glfwTerminate();

	return 0;
	
}

