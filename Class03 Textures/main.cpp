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

	// 宽度、高度、颜色通道个数
	int width, height, nrChannels;

	unsigned char *data = stbi_load("container.jpg", &width, &height, &nrChannels, 0);

	unsigned int texture;

	// 第一个参数是生成纹理的数量
	glGenTextures(1, &texture);

	// 绑定texture对象到GL_TEXTURE_2D，让后面对GL_TEXTURE_2D的指令都绑定到texture纹理上
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	if(data)
	{
		// 根据前面载入的图片生成纹理
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	// 释放图像
	stbi_image_free(data);

	float vertices[] = {
		//     ---- 位置 ----       ---- 颜色 ----     - 纹理坐标 -
			 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // 右上
			 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // 右下
			-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // 左下
			-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // 左上
	};

	unsigned int indices[] =
	{
		0, 1, 3, // 第一个三角形
		1, 2, 3 // 第二个三角形
	};

	glBindTexture(GL_TEXTURE_2D, texture);

	Shader shadeProgram("shader.vs", "shader.fs");

	shadeProgram.user();

	// 顶点数组对象
	unsigned int VAO;

	glGenVertexArrays(1, &VAO);

	glBindVertexArray(VAO);

	// 顶点缓冲对象引用	

	unsigned int VBO;

	glGenBuffers(1, &VBO);

	// 顶点缓冲对象的缓冲类型是GL_ARRAY_BUFFER
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	// glBufferData是一个专门用来把用户定义的数据复制到当前绑定缓冲的函数
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
	
	// 渲染循环(Render Loop)
	while (!glfwWindowShouldClose(Window))
	{

		// 渲染指令
		// “状态设置”函数
		glClearColor(0.2f, 0.3f, 0.3f, 10.f);

		// “状态使用”函数
		// 清除颜色缓冲之后，整个颜色缓冲都将被填充为glClearColor里设置的颜色
		glClear(GL_COLOR_BUFFER_BIT);

		//glBindTexture(GL_TEXTURE_2D, texture);

		// 
		//glBindVertexArray(VAO);

		// 绘制图形
		//glDrawArrays(GL_TRIANGLES, 0, 3);

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

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

	return 0;
	
}

