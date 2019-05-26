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

	// 将此窗口设置为上下文窗口
	glfwMakeContextCurrent(Window);


	// 将gl获取的窗口地址方法给glad库 
	// load all OpenGL function pointers
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Faild" << std::endl;

		return -1;
	}

	// 视口大小
	glViewport(0, 0, 800, 600);

	// 窗口大小改变回调
	glfwSetFramebufferSizeCallback(Window, framebuffer_size_callback);

	unsigned int shaderProgram, VAO, VBO, vertexShader, fragmentShader;


	ShaderByClass(shaderProgram, VAO, VBO, vertexShader, fragmentShader);

	// 绘制背面和正面：用线绘制
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// 默认绘制：全填充
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	// 渲染循环(Render Loop)
	while (!glfwWindowShouldClose(Window))
	{
		// 输入监测
		processInput(Window);

		// 渲染指令
		// “状态设置”函数
		glClearColor(0.2f, 0.3f, 0.3f, 10.f);

		// “状态使用”函数
		// 清除颜色缓冲之后，整个颜色缓冲都将被填充为glClearColor里设置的颜色
		glClear(GL_COLOR_BUFFER_BIT);


		glUseProgram(shaderProgram);

		// 
		glBindVertexArray(VAO);

		// 绘制图形
		//glDrawArrays(GL_TRIANGLES, 0, 3);

		glDrawArrays(GL_TRIANGLES, 0, 6);

		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);


		// 检查是否有事件触发：键盘、鼠标，更新窗口状态
		glfwPollEvents();

		//双缓冲
		//前缓冲保存着最终输出的图像，它会在屏幕上显示；而所有的的渲染指令都会在后缓冲上绘制。
		//当所有的渲染指令执行完毕后，我们交换(Swap)前缓冲和后缓冲，这样图像就立即呈显出来，之前提到的不真实感就消除了。

		// 交换颜色缓冲（它是一个储存着GLFW窗口每一个像素颜色值的大缓冲），它在这一迭代中被用来绘制，并且将会作为输出显示在屏幕上
		glfwSwapBuffers(Window);
	}

	// 清理资源
	glfwTerminate();

	//system("pause");

	return 0;
}

void Shader1(unsigned int& shaderProgram, unsigned int& VAO, unsigned int& VBO, unsigned int & vertexShader, unsigned int & fragmentShader)
{
	// 三角形
	// float vertices[] =
	// {
	// 	-0.5f, -0.5f, 0.0f,
	// 	0.5f, -0.5f, 0.0f,
	// 	0.0f, 0.5f, 0.0f
	// };

	// 正方形
	// float vertices[] = {
	// 	// 第一个三角形
	// 	0.5f, 0.5f, 0.0f,   // 右上角
	// 	0.5f, -0.5f, 0.0f,  // 右下角
	// 	-0.5f, 0.5f, 0.0f,  // 左上角
	// 	// 第二个三角形
	// 	0.5f, -0.5f, 0.0f,  // 右下角
	// 	-0.5f, -0.5f, 0.0f, // 左下角
	// 	-0.5f, 0.5f, 0.0f   // 左上角
	// };

	// 正方形
	float vertices[] = {
	0.5f, 0.5f, 0.0f,   // 右上角
	0.5f, -0.5f, 0.0f,  // 右下角
	-0.5f, -0.5f, 0.0f, // 左下角
	-0.5f, 0.5f, 0.0f   // 左上角
	};

	unsigned int indices[] =
	{
		0, 1, 3, // 第一个三角形
		1, 2, 3 // 第二个三角形
	};



	// 顶点着色器源码
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

	// 顶点着色器对象引用

	vertexShader = glCreateShader(GL_VERTEX_SHADER);

	// 绑定源码和对象
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);

	// 编译着色器
	glCompileShader(vertexShader);

	int success;

	char infoLog[512];

	// 判断编译状态
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);

		std::cout << infoLog << std::endl;
	}

	// 片段着色器源码
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

	// 着色器程序
	shaderProgram = glCreateProgram();

	// 附加顶点着色器
	glAttachShader(shaderProgram, vertexShader);


	// 附加片段着色器
	glAttachShader(shaderProgram, fragmentShader);

	// 链接
	glLinkProgram(shaderProgram);

	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);

	if (!success)
	{
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);

		std::cout << infoLog << std::endl;
	}

	// 激活程序对象
	glUseProgram(shaderProgram);


	// // 链接到程序对象之后可以删除着色器
	// glDeleteShader(vertexShader);
	//
	// glDeleteShader(fragmentShader);

	// 索引缓冲对象引用 按制定顶点顺序绘制


	// 顶点缓冲对象引用		

	glGenVertexArrays(1, &VAO);

	glBindVertexArray(VAO);



	glGenBuffers(1, &VBO);

	// 顶点缓冲对象的缓冲类型是GL_ARRAY_BUFFER
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	// glBufferData是一个专门用来把用户定义的数据复制到当前绑定缓冲的函数
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);


	unsigned int EBO;

	glGenBuffers(1, &EBO);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// 解析顶点数据方式
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);


	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//glBindVertexArray(0);

	//glDrawArrays(GL_TRIANGLES, 0, 3);


	/*
	 * 1.绑定VAO
	 * 2.绑定VBO
	 * 3.填充VBO，填充的数据也会存储在VAO
	 * 4.绑定EBO
	 * 5.填充EBO，告诉gl读取顶点的顺序
	 */
}

void Shader2(unsigned int& shaderProgram, unsigned int& VAO, unsigned int& VBO, unsigned int & vertexShader, unsigned int & fragmentShader)
{
	// 三角形
	// float vertices[] =
	// {
	// 	-0.5f, -0.5f, 0.0f,
	// 	0.5f, -0.5f, 0.0f,
	// 	0.0f, 0.5f, 0.0f
	// };

	// 正方形
	// float vertices[] = {
	// 	// 第一个三角形
	// 	0.5f, 0.5f, 0.0f,   // 右上角
	// 	0.5f, -0.5f, 0.0f,  // 右下角
	// 	-0.5f, 0.5f, 0.0f,  // 左上角
	// 	// 第二个三角形
	// 	0.5f, -0.5f, 0.0f,  // 右下角
	// 	-0.5f, -0.5f, 0.0f, // 左下角
	// 	-0.5f, 0.5f, 0.0f   // 左上角
	// };

	// 正方形
	// float vertices[] = {
	// 0.5f, 0.5f, 0.0f,   // 右上角
	// 0.5f, -0.5f, 0.0f,  // 右下角
	// -0.5f, -0.5f, 0.0f, // 左下角
	// -0.5f, 0.5f, 0.0f   // 左上角
	// };
	//
	// unsigned int indices[] =
	// {
	// 	0, 1, 3, // 第一个三角形
	// 	1, 2, 3 // 第二个三角形
	// };

	float vertices[] = {
		// 位置              // 颜色
		 0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // 右下
		-0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // 左下
		 0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // 顶部
	};



	// 顶点着色器源码
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
		"out vec3 ourColor;//向片段着色器输出一个颜色\n"
		"void main()\n"
		"{\n"
		"   gl_Position = vec4(aPos, 1.0);\n"
		"	//vertexColor = vec4(0.5, 0.0, 0.0, 1.0);\n"
		"	ourColor = aColor;\n"
		"}\n\0";

	// 顶点着色器对象引用

	vertexShader = glCreateShader(GL_VERTEX_SHADER);

	// 绑定源码和对象
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);

	// 编译着色器
	glCompileShader(vertexShader);

	int success;

	char infoLog[512];

	// 判断编译状态
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);

		std::cout << "vertex\n" << infoLog << std::endl;
	}

	// 片段着色器源码
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

	// 着色器程序
	shaderProgram = glCreateProgram();

	// 附加顶点着色器
	glAttachShader(shaderProgram, vertexShader);


	// 附加片段着色器
	glAttachShader(shaderProgram, fragmentShader);

	// 链接
	glLinkProgram(shaderProgram);

	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);

	if (!success)
	{
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);

		std::cout << "shaderProgram\n" << infoLog << std::endl;
	}

	// 激活程序对象
	glUseProgram(shaderProgram);


	// // 链接到程序对象之后可以删除着色器
	// glDeleteShader(vertexShader);
	//
	// glDeleteShader(fragmentShader);

	// 索引缓冲对象引用 按制定顶点顺序绘制


	// 顶点缓冲对象引用		

	glGenVertexArrays(1, &VAO);

	glBindVertexArray(VAO);



	glGenBuffers(1, &VBO);

	// 顶点缓冲对象的缓冲类型是GL_ARRAY_BUFFER
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	// glBufferData是一个专门用来把用户定义的数据复制到当前绑定缓冲的函数
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);


	// unsigned int EBO;
	//
	// glGenBuffers(1, &EBO);
	//
	// glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	//
	// glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// 解析顶点数据方式  位置属性
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);

	glEnableVertexAttribArray(0);

	// 颜色属性
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//glBindVertexArray(0);

	//glDrawArrays(GL_TRIANGLES, 0, 3);


	/*
	 * 1.绑定VAO
	 * 2.绑定VBO
	 * 3.填充VBO，填充的数据也会存储在VAO
	 * 4.绑定EBO
	 * 5.填充EBO，告诉gl读取顶点的顺序
	 */
}

void ShaderByClass(unsigned int& shaderProgram, unsigned int& VAO, unsigned int& VBO, unsigned int & vertexShader, unsigned int & fragmentShader)
{
	float vertices[] = {
		// 位置              // 颜色
		 0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // 右下
		-0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // 左下
		 0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // 顶部
	};


	
	Shader shader("shader.vs", "shader.fs");

	shader.user();

	

	glGenVertexArrays(1, &VAO);

	glBindVertexArray(VAO);

	// 顶点缓冲对象引用		

	glGenBuffers(1, &VBO);

	// 顶点缓冲对象的缓冲类型是GL_ARRAY_BUFFER
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	// glBufferData是一个专门用来把用户定义的数据复制到当前绑定缓冲的函数
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);


	// unsigned int EBO;
	//
	// glGenBuffers(1, &EBO);
	//
	// glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	//
	// glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// 解析顶点数据方式  位置属性
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);

	glEnableVertexAttribArray(0);

	// 颜色属性
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//glBindVertexArray(0);

	//glDrawArrays(GL_TRIANGLES, 0, 3);


	/*
	 * 1.绑定VAO
	 * 2.绑定VBO
	 * 3.填充VBO，填充的数据也会存储在VAO
	 * 4.绑定EBO
	 * 5.填充EBO，告诉gl读取顶点的顺序
	 */
}
// 窗口大小改变回调
void framebuffer_size_callback(GLFWwindow * window, int width, int height)
{
	std::cout << "111" << std::endl;
	glViewport(0, 0, width, height);
}

// 输入回调
void processInput(GLFWwindow * window)
{
	// 失败返回GLFW_RELEASE
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
}