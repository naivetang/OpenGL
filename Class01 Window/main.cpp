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

	// ���˴�������Ϊ�����Ĵ���
	glfwMakeContextCurrent(Window);


	// ��gl��ȡ�Ĵ��ڵ�ַ������glad�� 
	// load all OpenGL function pointers
	if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Faild" << std::endl;

		return -1;
	}

	// �ӿڴ�С
	glViewport(0, 0, 800, 600);

	// ���ڴ�С�ı�ص�
	glfwSetFramebufferSizeCallback(Window, framebuffer_size_callback);


	// ���Ʊ�������棺���߻���
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// Ĭ�ϻ��ƣ�ȫ���
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	// ��Ⱦѭ��(Render Loop)
	while(!glfwWindowShouldClose(Window))
	{
		// ������
		processInput(Window);

		// ��Ⱦָ��
		// ��״̬���á�����
		glClearColor(0.2f, 0.3f, 0.3f, 10.f);

		// ��״̬ʹ�á�����
		// �����ɫ����֮��������ɫ���嶼�������ΪglClearColor�����õ���ɫ
		glClear(GL_COLOR_BUFFER_BIT);



		glDrawArrays(GL_TRIANGLES, 0, 3);


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

// ���ڴ�С�ı�ص�
void framebuffer_size_callback(GLFWwindow * window,int width, int height)
{
	std::cout << "111" << std::endl;
	glViewport(0, 0, width, height);
}

// ����ص�
void processInput(GLFWwindow * window)
{
	// ʧ�ܷ���GLFW_RELEASE
	if(glfwGetKey(window,GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
}