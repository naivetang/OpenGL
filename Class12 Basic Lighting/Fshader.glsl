#version 330 core
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

void main()
{

	// ���ߵ�λ����
	vec3 DirNor = normalize(lightPos - FragPos);

	// cos�нǵ�ֵ �� ֵԽ�� �� �н�ԽС �� ����ǿ��Խ��
	float cossAngle = max( dot(normalize( Normal), DirNor), 0.0);

	// �������
	vec3 diffuse = cossAngle * lightColor;

	float ambientStrength = 0.1;

	// ������
	vec3 ambient = ambientStrength * lightColor;

	FragColor = vec4((diffuse + ambient)* objectColor, 1.0);
}
