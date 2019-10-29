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
	// �����
	vec3 direction;

	vec3 position;
	

	vec3 ambient;

	vec3 diffuse;

	vec3 specular;


	// ���Դ
	// ǿ��˥����ʽ ��1 / ��contant  + linear * d + quadratic * d * d ��

	// �������ù�ʽ����ֵ������1
	float constant;

	// һ����ϵ��
	float linear;

	// ������ϵ��
	float quadratic;

	// �۹�Դ
	//vec3 direction;
	//vec3 position;
	float inCutOff;

	float outerCutOff;

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
	vec3 DirNor = normalize(light.position - FragPos);	

	// Ƭ�ξ�����Դ��λ��
	float dis = distance(light.position, FragPos);

	//vec3 DirNor = normalize(-light.direction);	

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

	// ���Դ
	//FragColor = vec4((specular + diffuse + ambient) * objectColor * (1 / (light.constant + light.linear * dis + light.quadratic * dis * dis)), 1.0);

	// �۹�Դ
	float theta = dot(normalize(light.direction) ,normalize(FragPos - light.position) );

	float intensity =clamp( (theta - light.outerCutOff)  / (light.inCutOff - light.outerCutOff) , 0.0, 1.0) ;

	FragColor = vec4((specular + diffuse + ambient) * intensity * objectColor, 1.0);

	return;

	if(theta > light.inCutOff)
		FragColor = vec4((specular + diffuse + ambient) * objectColor, 1.0);
	else
		FragColor = vec4(ambient * objectColor, 1.0);
		
}
