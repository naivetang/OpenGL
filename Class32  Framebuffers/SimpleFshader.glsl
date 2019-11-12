#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

struct Material
{
	// ������  ��ͼ
	sampler2D texture_diffuse1;

	// ���淴��
	//vec3 specular;

	// ���淴�� ��ͼ Խ�׵ĵط����淴��Խǿ��Խ���ĵط����淴��Խ��
	//sampler2D specular;
	sampler2D texture_specular1;

	// �����
	float shininess;
};

uniform Material material;

void main()
{ 
    FragColor = texture(material.texture_diffuse1, TexCoords);
}