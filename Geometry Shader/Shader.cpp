#include "Shader.h"
#include <glm/gtc/type_ptr.hpp>

Shader::Shader(const GLchar * vertexPath, const GLchar * fragmentPath, const GLchar * geometryPath)
{
	// 1. ���ļ�·���л�ȡ����/Ƭ����ɫ��
	std::string vertexCode;

	std::string fragmentCode;

	std::string geometryCode;

	std::ifstream vShaderFile;

	std::ifstream fShaderFile;

	std::ifstream gShaderFile;

	// ��֤ifstream��������׳��쳣��
	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	gShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try
	{
		// ���ļ�
		vShaderFile.open(vertexPath);

		fShaderFile.open(fragmentPath);

		if(geometryPath != nullptr)
		{
			gShaderFile.open(geometryPath);
		}

		std::stringstream vShaderStream, fShaderStream, gShaderStream;
		
		// ��ȡ�ļ��Ļ������ݵ���������
		vShaderStream << vShaderFile.rdbuf();

		fShaderStream << fShaderFile.rdbuf();

		if (geometryPath != nullptr)
		{
			gShaderStream << gShaderFile.rdbuf();
		}

		// �ر��ļ�������
		vShaderFile.close();

		fShaderFile.close();

		if (geometryPath != nullptr)
		{
			gShaderFile.close();
		}

		// ת����������string
		vertexCode = vShaderStream.str();

		fragmentCode = fShaderStream.str();

		if (geometryPath != nullptr)
		{
			geometryCode = gShaderStream.str();
		}
	}
	catch (std::ifstream::failure e)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
	}

	const char* vShaderCode = vertexCode.c_str();

	const char* fShaderCode = fragmentCode.c_str();

	const char* gShaderCode;

	if (geometryPath != nullptr)
		gShaderCode = geometryCode.c_str();

	// 2. ������ɫ��

	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);

	

	glShaderSource(vertexShader, 1, &vShaderCode, NULL);

	// ���붥����ɫ��
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

	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(fragmentShader, 1, &fShaderCode, NULL);

	// ����Ƭ����ɫ��
	glCompileShader(fragmentShader);

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);

		std::cout << "fragment\n" << infoLog << std::endl;
	}

	// ���뼸����ɫ��

	unsigned int geometryShader;

	if(geometryPath != nullptr)
	{
		geometryShader = glCreateShader(GL_GEOMETRY_SHADER);

		glShaderSource(geometryShader, 1, &gShaderCode, NULL);

		// ����Ƭ����ɫ��
		glCompileShader(geometryShader);

		glGetShaderiv(geometryShader, GL_COMPILE_STATUS, &success);

		if (!success)
		{
			glGetShaderInfoLog(geometryShader, 512, nullptr, infoLog);

			std::cout << "geometry\n" << infoLog << std::endl;
		}
	}


	// ������ɫ��
	unsigned int shaderProgram = glCreateProgram();

	glAttachShader(shaderProgram, vertexShader);

	glAttachShader(shaderProgram, fragmentShader);

	if(geometryPath != nullptr)
	{
		glAttachShader(shaderProgram, geometryShader);
	}

	glLinkProgram(shaderProgram);

	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);

	if (!success)
	{
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);

		std::cout << "shaderProgram\n" << infoLog << std::endl;
	}

	glDeleteShader(vertexShader);

	glDeleteShader(fragmentShader);

	if (geometryPath != nullptr)
	{
		glDeleteShader(geometryShader);
	}

	ID = shaderProgram;
}

void Shader::user()
{
	// ����������
	glUseProgram(ID);
}

void Shader::setBool(const std::string & name, bool value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}

void Shader::setInt(const std::string & name, int value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setFloat(const std::string & name, float value) const
{
	glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setMat4(const std::string & name,glm::mat4 value) const
{
	glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::setVec3(const std::string& name, float x, float y, float z) const
{
	glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z);

}
void Shader::setVec3(const std::string& name, glm::vec3 dir) const
{
	setVec3(name, dir.x, dir.y, dir.z);
}