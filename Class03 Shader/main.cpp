#pragma comment(lib,"glfw3.lib")
#pragma comment(lib,"opengl32.lib")

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "Shader.h"

//glfw3.lib
//opengl32.lib

void framebuffer_size_callback(GLFWwindow * window, int width, int height);
void processInput(GLFWwindow * window);
void Shader1(unsigned int& shaderProgram, unsigned int& VAO, unsigned int& VBO, unsigned int & vertexShader, unsigned int & fragmentShader);
void Shader2(unsigned int& shaderProgram, unsigned int& VAO, unsigned int& VBO, unsigned int & vertexShader, unsigned int & fragmentShader);
void ShaderByClass(unsigned int& shaderProgram, unsigned int& VAO, unsigned int& VBO, unsigned int & vertexShader, unsigned int & fragmentShader);
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

	// ���ڴ�С�ı�ص�
	glfwSetFramebufferSizeCallback(Window, framebuffer_size_callback);

	unsigned int shaderProgram, VAO, VBO, vertexShader, fragmentShader;


	ShaderByClass(shaderProgram, VAO, VBO, vertexShader, fragmentShader);

	// ���Ʊ�������棺���߻���
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// Ĭ�ϻ��ƣ�ȫ���
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	// ��Ⱦѭ��(Render Loop)
	while (!glfwWindowShouldClose(Window))
	{
		// ������
		processInput(Window);

		// ��Ⱦָ��
		// ��״̬���á�����
		glClearColor(0.2f, 0.3f, 0.3f, 10.f);

		// ��״̬ʹ�á�����
		// �����ɫ����֮��������ɫ���嶼�������ΪglClearColor�����õ���ɫ
		glClear(GL_COLOR_BUFFER_BIT);


		glUseProgram(shaderProgram);

		// 
		glBindVertexArray(VAO);

		// ����ͼ��
		//glDrawArrays(GL_TRIANGLES, 0, 3);

		glDrawArrays(GL_TRIANGLES, 0, 6);

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

	//system("pause");

	return 0;
}

void Shader1(unsigned int& shaderProgram, unsigned int& VAO, unsigned int& VBO, unsigned int & vertexShader, unsigned int & fragmentShader)
{
	// ������
	// float vertices[] =
	// {
	// 	-0.5f, -0.5f, 0.0f,
	// 	0.5f, -0.5f, 0.0f,
	// 	0.0f, 0.5f, 0.0f
	// };

	// ������
	// float vertices[] = {
	// 	// ��һ��������
	// 	0.5f, 0.5f, 0.0f,   // ���Ͻ�
	// 	0.5f, -0.5f, 0.0f,  // ���½�
	// 	-0.5f, 0.5f, 0.0f,  // ���Ͻ�
	// 	// �ڶ���������
	// 	0.5f, -0.5f, 0.0f,  // ���½�
	// 	-0.5f, -0.5f, 0.0f, // ���½�
	// 	-0.5f, 0.5f, 0.0f   // ���Ͻ�
	// };

	// ������
	float vertices[] = {
	0.5f, 0.5f, 0.0f,   // ���Ͻ�
	0.5f, -0.5f, 0.0f,  // ���½�
	-0.5f, -0.5f, 0.0f, // ���½�
	-0.5f, 0.5f, 0.0f   // ���Ͻ�
	};

	unsigned int indices[] =
	{
		0, 1, 3, // ��һ��������
		1, 2, 3 // �ڶ���������
	};



	// ������ɫ��Դ��
	// const char * vertexShaderSource = "\
	// #version 330 core\
	// layout(location = 0) in vec3 aPos;\
	// void main()\
	// {\
	// 	gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\
	// }\
	// ";

	const char *vertexShaderSource = "#version 330 core\n"
		"layout (location = 0) in vec3 aPos;\n"
		"out vec4 vertexColor;\n"
		"void main()\n"
		"{\n"
		"   gl_Position = vec4(aPos, 1.0);\n"
		"	vertexColor = vec4(0.5f, 0.0, 0.0, 1.0);\n"
		"}\0";

	// ������ɫ����������

	vertexShader = glCreateShader(GL_VERTEX_SHADER);

	// ��Դ��Ͷ���
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);

	// ������ɫ��
	glCompileShader(vertexShader);

	int success;

	char infoLog[512];

	// �жϱ���״̬
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);

		std::cout << infoLog << std::endl;
	}

	// Ƭ����ɫ��Դ��
	// const char * fragmentShaderSource = "\
	// #version 330 core\
	// out vec4 FragColor;\
	// void main()\
	// {\
	// 	FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\
	// }\
	// ";

	const char *fragmentShaderSource = "#version 330 core\n"
		"out vec4 FragColor;\n"
		"uniform vec4 ourColor;\n"
		"//in vec4 vertexColor;\n"
		"void main()\n"
		"{\n"
		"   FragColor = ourColor;\n"
		"}\n\0";



	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);

	glCompileShader(fragmentShader);

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);

		std::cout << infoLog << std::endl;
	}

	// ��ɫ������
	shaderProgram = glCreateProgram();

	// ���Ӷ�����ɫ��
	glAttachShader(shaderProgram, vertexShader);


	// ����Ƭ����ɫ��
	glAttachShader(shaderProgram, fragmentShader);

	// ����
	glLinkProgram(shaderProgram);

	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);

	if (!success)
	{
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);

		std::cout << infoLog << std::endl;
	}

	// ����������
	glUseProgram(shaderProgram);


	// // ���ӵ��������֮�����ɾ����ɫ��
	// glDeleteShader(vertexShader);
	//
	// glDeleteShader(fragmentShader);

	// ��������������� ���ƶ�����˳�����


	// ���㻺���������		

	glGenVertexArrays(1, &VAO);

	glBindVertexArray(VAO);



	glGenBuffers(1, &VBO);

	// ���㻺�����Ļ���������GL_ARRAY_BUFFER
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	// glBufferData��һ��ר���������û���������ݸ��Ƶ���ǰ�󶨻���ĺ���
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);


	unsigned int EBO;

	glGenBuffers(1, &EBO);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// �����������ݷ�ʽ
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);


	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//glBindVertexArray(0);

	//glDrawArrays(GL_TRIANGLES, 0, 3);


	/*
	 * 1.��VAO
	 * 2.��VBO
	 * 3.���VBO����������Ҳ��洢��VAO
	 * 4.��EBO
	 * 5.���EBO������gl��ȡ�����˳��
	 */
}

void Shader2(unsigned int& shaderProgram, unsigned int& VAO, unsigned int& VBO, unsigned int & vertexShader, unsigned int & fragmentShader)
{
	// ������
	// float vertices[] =
	// {
	// 	-0.5f, -0.5f, 0.0f,
	// 	0.5f, -0.5f, 0.0f,
	// 	0.0f, 0.5f, 0.0f
	// };

	// ������
	// float vertices[] = {
	// 	// ��һ��������
	// 	0.5f, 0.5f, 0.0f,   // ���Ͻ�
	// 	0.5f, -0.5f, 0.0f,  // ���½�
	// 	-0.5f, 0.5f, 0.0f,  // ���Ͻ�
	// 	// �ڶ���������
	// 	0.5f, -0.5f, 0.0f,  // ���½�
	// 	-0.5f, -0.5f, 0.0f, // ���½�
	// 	-0.5f, 0.5f, 0.0f   // ���Ͻ�
	// };

	// ������
	// float vertices[] = {
	// 0.5f, 0.5f, 0.0f,   // ���Ͻ�
	// 0.5f, -0.5f, 0.0f,  // ���½�
	// -0.5f, -0.5f, 0.0f, // ���½�
	// -0.5f, 0.5f, 0.0f   // ���Ͻ�
	// };
	//
	// unsigned int indices[] =
	// {
	// 	0, 1, 3, // ��һ��������
	// 	1, 2, 3 // �ڶ���������
	// };

	float vertices[] = {
		// λ��              // ��ɫ
		 0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // ����
		-0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // ����
		 0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // ����
	};



	// ������ɫ��Դ��
	// const char * vertexShaderSource = "\
	// #version 330 core\
	// layout(location = 0) in vec3 aPos;\
	// void main()\
	// {\
	// 	gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\
	// }\
	// ";

	const char *vertexShaderSource = "#version 330 core\n"
		"layout (location = 0) in vec3 aPos;\n"
		"layout (location = 1) in vec3 aColor;\n"
		"out vec3 ourColor;//��Ƭ����ɫ�����һ����ɫ\n"
		"void main()\n"
		"{\n"
		"   gl_Position = vec4(aPos, 1.0);\n"
		"	//vertexColor = vec4(0.5, 0.0, 0.0, 1.0);\n"
		"	ourColor = aColor;\n"
		"}\n\0";

	// ������ɫ����������

	vertexShader = glCreateShader(GL_VERTEX_SHADER);

	// ��Դ��Ͷ���
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);

	// ������ɫ��
	glCompileShader(vertexShader);

	int success;

	char infoLog[512];

	// �жϱ���״̬
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);

		std::cout << "vertex\n" << infoLog << std::endl;
	}

	// Ƭ����ɫ��Դ��
	// const char * fragmentShaderSource = "\
	// #version 330 core\
	// out vec4 FragColor;\
	// void main()\
	// {\
	// 	FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\
	// }\
	// ";

	const char *fragmentShaderSource = "#version 330 core\n"
		"out vec4 FragColor;\n"
		"in vec3 ourColor;\n"
		"//in vec4 vertexColor;\n"
		"void main()\n"
		"{\n"
		"   FragColor = vec4(ourColor, 1.0);\n"
		"}\n\0";



	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);

	glCompileShader(fragmentShader);

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);

		std::cout << "fragment\n" << infoLog << std::endl;
	}

	// ��ɫ������
	shaderProgram = glCreateProgram();

	// ���Ӷ�����ɫ��
	glAttachShader(shaderProgram, vertexShader);


	// ����Ƭ����ɫ��
	glAttachShader(shaderProgram, fragmentShader);

	// ����
	glLinkProgram(shaderProgram);

	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);

	if (!success)
	{
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);

		std::cout << "shaderProgram\n" << infoLog << std::endl;
	}

	// ����������
	glUseProgram(shaderProgram);


	// // ���ӵ��������֮�����ɾ����ɫ��
	// glDeleteShader(vertexShader);
	//
	// glDeleteShader(fragmentShader);

	// ��������������� ���ƶ�����˳�����


	// ���㻺���������		

	glGenVertexArrays(1, &VAO);

	glBindVertexArray(VAO);



	glGenBuffers(1, &VBO);

	// ���㻺�����Ļ���������GL_ARRAY_BUFFER
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	// glBufferData��һ��ר���������û���������ݸ��Ƶ���ǰ�󶨻���ĺ���
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);


	// unsigned int EBO;
	//
	// glGenBuffers(1, &EBO);
	//
	// glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	//
	// glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// �����������ݷ�ʽ  λ������
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);

	glEnableVertexAttribArray(0);

	// ��ɫ����
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//glBindVertexArray(0);

	//glDrawArrays(GL_TRIANGLES, 0, 3);


	/*
	 * 1.��VAO
	 * 2.��VBO
	 * 3.���VBO����������Ҳ��洢��VAO
	 * 4.��EBO
	 * 5.���EBO������gl��ȡ�����˳��
	 */
}

void ShaderByClass(unsigned int& shaderProgram, unsigned int& VAO, unsigned int& VBO, unsigned int & vertexShader, unsigned int & fragmentShader)
{
	float vertices[] = {
		// λ��              // ��ɫ
		 0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // ����
		-0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // ����
		 0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // ����
	};


	
	Shader shader("shader.vs", "shader.fs");

	shader.user();

	

	glGenVertexArrays(1, &VAO);

	glBindVertexArray(VAO);

	// ���㻺���������		

	glGenBuffers(1, &VBO);

	// ���㻺�����Ļ���������GL_ARRAY_BUFFER
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	// glBufferData��һ��ר���������û���������ݸ��Ƶ���ǰ�󶨻���ĺ���
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);


	// unsigned int EBO;
	//
	// glGenBuffers(1, &EBO);
	//
	// glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	//
	// glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// �����������ݷ�ʽ  λ������
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);

	glEnableVertexAttribArray(0);

	// ��ɫ����
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//glBindVertexArray(0);

	//glDrawArrays(GL_TRIANGLES, 0, 3);


	/*
	 * 1.��VAO
	 * 2.��VBO
	 * 3.���VBO����������Ҳ��洢��VAO
	 * 4.��EBO
	 * 5.���EBO������gl��ȡ�����˳��
	 */
}
// ���ڴ�С�ı�ص�
void framebuffer_size_callback(GLFWwindow * window, int width, int height)
{
	std::cout << "111" << std::endl;
	glViewport(0, 0, width, height);
}

// ����ص�
void processInput(GLFWwindow * window)
{
	// ʧ�ܷ���GLFW_RELEASE
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
}