#ifndef SHADER_H
#define SHADER_H

#pragma comment(lib,"glfw3.lib")
#pragma comment(lib,"opengl32.lib")

#include <glad/glad.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <glm/mat4x2.hpp>

class Shader
{
public:
	unsigned int ID;
	
	// 构造器读取并构建着色器
	Shader(const GLchar * vertexPath,const GLchar* fragmentPath);
	
	// 使用/激活程序
	void user();
	
	//uniform工具函数
	void setBool(const std::string &name, bool value) const;
	void setInt(const std::string &name, int value) const;
	void setFloat(const std::string &name, float value) const;
	void setMat4(const std::string& name, glm::mat4 value) const;
};

#endif

