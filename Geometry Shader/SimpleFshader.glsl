#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

in vec3 Normal;

in vec3 Position;

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

uniform vec3 cameraPos;

uniform samplerCube skybox;

void main()
{ 
	vec3 I = Position - cameraPos;

	// ����
	vec3 R = reflect(normalize(I), normalize(Normal));

	// ����
	vec3 Z = refract(normalize(I), normalize(Normal), 1f / 1.52f);

    //FragColor = vec4( texture(material.texture_diffuse1, TexCoords).rgb ,1.0);
    FragColor = vec4( texture(skybox, R).rgb, 1.0 );
}	