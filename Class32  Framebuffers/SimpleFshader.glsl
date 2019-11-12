#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

struct Material
{
	// 漫反射  贴图
	sampler2D texture_diffuse1;

	// 镜面反射
	//vec3 specular;

	// 镜面反射 贴图 越白的地方镜面反射越强、越暗的地方镜面反射越弱
	//sampler2D specular;
	sampler2D texture_specular1;

	// 反光度
	float shininess;
};

uniform Material material;

void main()
{ 
    FragColor = texture(material.texture_diffuse1, TexCoords);
}