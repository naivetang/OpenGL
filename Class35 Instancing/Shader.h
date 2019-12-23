#pragma once

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
	
	// ��������ȡ��������ɫ��
	Shader(const GLchar * vertexPath,const GLchar* fragmentPath, const GLchar * geometryPath = nullptr);
	
	// ʹ��/�������
	void user();
	
	//uniform���ߺ���
	void setBool(const std::string &name, bool value) const;
	void setInt(const std::string &name, int value) const;
	void setFloat(const std::string &name, float value) const;
	void setMat4(const std::string& name, glm::mat4 value) const;
	void setVec3(const std::string& name, float x, float y, float z) const;
	void setVec3(const std::string& name, glm::vec3 dir) const;
	void setVec2(const std::string& name, glm::vec2 dir) const;
};

#endif

