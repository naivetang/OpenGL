#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
out vec3 ourColor;//��Ƭ����ɫ�����һ����ɫ
void main()
{
	gl_Position = vec4(aPos, 1.0);
	//vertexColor = vec4(0.5, 0.0, 0.0, 1.0);
	ourColor = aColor;
};