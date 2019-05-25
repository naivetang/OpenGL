#pragma comment(lib,"glfw3.lib")
#pragma comment(lib,"opengl32.lib")

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>


void framebuffer_size_callback(GLFWwindow * window, int width, int height);
void processInput(GLFWwindow * window);

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

	if(Window == nullptr)
	{
		std::cout << "Faild" << std::endl;
		
		glfwTerminate();
		
		return -1;
	}

	// 将此窗口设置为上下文窗口
	glfwMakeContextCurrent(Window);


	// 将gl获取的窗口地址方法给glad库 
	// load all OpenGL function pointers
	if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Faild" << std::endl;

		return -1;
	}

	// 视口大小
	glViewport(0, 0, 800, 600);

	// 窗口大小改变回调
	glfwSetFramebufferSizeCallback(Window, framebuffer_size_callback);


	// 绘制背面和正面：用线绘制
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// 默认绘制：全填充
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	// 渲染循环(Render Loop)
	while(!glfwWindowShouldClose(Window))
	{
		// 输入监测
		processInput(Window);

		// 渲染指令
		// “状态设置”函数
		glClearColor(0.2f, 0.3f, 0.3f, 10.f);

		// “状态使用”函数
		// 清除颜色缓冲之后，整个颜色缓冲都将被填充为glClearColor里设置的颜色
		glClear(GL_COLOR_BUFFER_BIT);



		glDrawArrays(GL_TRIANGLES, 0, 3);


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

// 窗口大小改变回调
void framebuffer_size_callback(GLFWwindow * window,int width, int height)
{
	std::cout << "111" << std::endl;
	glViewport(0, 0, width, height);
}

// 输入回调
void processInput(GLFWwindow * window)
{
	// 失败返回GLFW_RELEASE
	if(glfwGetKey(window,GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
}