#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

out vec3 FragPos;
out vec3 Normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	FragPos = vec3( model * vec4( aPos ,1.0));

	// 局部空间的法向量
	// Normal = aNormal;

	// 世界空间的法向量 法线矩阵：模型矩阵左上角的逆矩阵的转置矩阵
	// 逆矩阵开销大，正式环境建议法线矩阵由CPU计算
	Normal = mat3(transpose(inverse(model))) * aNormal;

	gl_Position = projection * view * model * vec4(aPos, 1.0);
	
	
}