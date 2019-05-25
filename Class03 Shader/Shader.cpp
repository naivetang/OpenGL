#include "Shader.h"


Shader::Shader(const GLchar * vertexPath, const GLchar * fragmentPath)
{
	// 1. 从文件路径中获取顶点/片段着色器
	std::string vertexCode;

	std::string fragmentCode;

	std::ifstream vShaderFile;

	std::ifstream fShaderFile;

	// 保证ifstream对象可以抛出异常：
	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try
	{
		// 打开文件
		vShaderFile.open(vertexPath);

		fShaderFile.open(fragmentPath);

		std::stringstream vShaderStream, fShaderStream;
		
		// 读取文件的缓冲内容到数据流中
		vShaderStream << vShaderFile.rdbuf();

		fShaderStream << fShaderFile.rdbuf();

		// 关闭文件处理器
		vShaderFile.close();

		fShaderFile.close();

		// 转换数据流到string
		vertexCode = vShaderStream.str();

		fragmentCode = fShaderStream.str();
	}
	catch (std::ifstream::failure e)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
	}

	const char* vShaderCode = vertexCode.c_str();

	const char* fShaderCode = fragmentCode.c_str();

	// 2. 编译着色器

	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);

	glShaderSource(vertexShader, 1, &vShaderCode, NULL);

	// 编译顶点着色器
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

	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(fragmentShader, 1, &fShaderCode, NULL);

	// 编译片段着色器
	glCompileShader(fragmentShader);

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);

		std::cout << "fragment\n" << infoLog << std::endl;
	}

	// 程序着色器
	unsigned int shaderProgram = glCreateProgram();

	glAttachShader(shaderProgram, vertexShader);

	glAttachShader(shaderProgram, fragmentShader);

	glLinkProgram(shaderProgram);

	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);

	if (!success)
	{
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);

		std::cout << "shaderProgram\n" << infoLog << std::endl;
	}

	glDeleteShader(vertexShader);

	glDeleteShader(fragmentShader);

	ID = shaderProgram;
}

void Shader::user()
{
	// 激活程序对象
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
