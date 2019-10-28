#version 330 core

struct Material
{
	// ������  ��ͼ
	sampler2D diffuse;

	// ���淴��
	//vec3 specular;

	// ���淴��
	sampler2D specular;

	// �����
	float shininess;
};

uniform Material material;

struct Light
{
	vec3 position;

	vec3 ambient;

	vec3 diffuse;

	vec3 specular;
};

uniform Light light;


out vec4 FragColor;

// Ƭ�η�����
in vec3 Normal;

// Ƭ��λ��
in vec3 FragPos;

// �Լ���ɫ
uniform vec3 objectColor;

// ��Դ��ɫ
uniform vec3 lightColor;

// ��Դλ��
uniform vec3 lightPos;

// �����λ��
uniform vec3 viewPose;

// ������ͼλ��
in vec2 TexCoords;

//������ɫ ������ + ������� + �������

void main()
{
	// ������
	vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
	
	// ���ߵ�λ����
	vec3 DirNor = normalize(lightPos - FragPos);	

	// cos�нǵ�ֵ �� ֵԽ�� �� �н�ԽС �� ����ǿ��Խ��
	float cossAngle = max( dot(normalize( Normal), DirNor), 0.0);

	// �������
	vec3 diffuse = light.diffuse * cossAngle * vec3 ( texture(material.diffuse, TexCoords));

	// ������գ��߹⣩
	vec3 viewDir = normalize(viewPose - FragPos);

	// �������
	vec3 reflectDir = reflect(-DirNor, Normal);

	// �ȼ������߷����뷴�䷽��ĵ�ˣ���ȷ�������Ǹ�ֵ����Ȼ��ȡ����32���ݡ����32�Ǹ߹�ķ����(Shininess)
	// һ������ķ����Խ�ߣ�����������Խǿ��ɢ���Խ�٣��߹��ͻ�ԽС
	
	float spec = pow( max(dot(normalize(viewDir), normalize(reflectDir)), 0.0) , material.shininess );

	vec3 specular = spec * light.specular * vec3(texture(material.specular, TexCoords));

	FragColor = vec4((specular + diffuse + ambient)* objectColor, 1.0);
}
