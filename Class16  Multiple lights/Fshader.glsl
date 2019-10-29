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

///////////////////////////////////////////////////////////////////////////

// �����Դ
struct DirLight
{
	vec3 direction;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

uniform DirLight dirLight;

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);

/////////////////////////////////////////////////////////////////////////////

// ���Դ
struct PointLight
{
	vec3 position;

	float constant;
	float linear;
	float quadratic;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

#define NR_POINT_LIGHTS 4
uniform PointLight pointLights[NR_POINT_LIGHTS];

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

/////////////////////////////////////////////////////////////////////////////////

// �۹�Դ
struct SpotLight
{
	vec3 position;

	vec3 direction;
	float inCutOff;
	float outerCutOff;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

uniform SpotLight spotLight;

vec3 CalcSpotLight(SpotLight light, vec3 normal,vec3 fragPos,vec3 viewDir);
/////////////////////////////////////////////////////////////////////////////////

//������ɫ ������ + ������� + �������

void main()
{
	vec3 normal = normalize(Normal);

	vec3 viewDir = normalize(viewPose - FragPos);

	vec3 result;

	// ��һ�׶Σ��������
	 result += CalcDirLight(dirLight, normal, viewDir);

	// �ڶ��׶Σ����Դ
	for(int i = 0; i < NR_POINT_LIGHTS; i++)
		result += CalcPointLight(pointLights[i], normal, FragPos, viewDir);

	// �����׶Σ��۹�Դ
	result += CalcSpotLight(spotLight, normal, FragPos, viewDir);

	FragColor = vec4(result, 1.0f);

	return;		
}

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
	// ������
	vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));



	
	// ���ߵ�λ����
	vec3 lightDir  = normalize(-light.direction);		

	// cos�нǵ�ֵ �� ֵԽ�� �� �н�ԽС �� ����ǿ��Խ��
	float cossAngle = max( dot(normalize( normal), lightDir ), 0.0);

	// �������
	vec3 diffuse = light.diffuse * cossAngle * vec3 ( texture(material.diffuse, TexCoords));




	// ������գ��߹⣩
	//vec3 viewDir = normalize(viewPose - FragPos);

	// �������
	vec3 reflectDir = reflect(-lightDir , normal);

	// �ȼ������߷����뷴�䷽��ĵ�ˣ���ȷ�������Ǹ�ֵ����Ȼ��ȡ����32���ݡ����32�Ǹ߹�ķ����(Shininess)
	// һ������ķ����Խ�ߣ�����������Խǿ��ɢ���Խ�٣��߹��ͻ�ԽС
	
	float spec = pow( max(dot(normalize(viewDir), normalize(reflectDir)), 0.0) , material.shininess );

	vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));

	return (ambient + diffuse + specular);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{	

	// ������
	vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
	
	// ���ߵ�λ����
	vec3 DirNor = normalize(light.position - fragPos);		

	//vec3 DirNor = normalize(-light.direction);	

	// cos�нǵ�ֵ �� ֵԽ�� �� �н�ԽС �� ����ǿ��Խ��
	float cossAngle = max( dot(normalize( normal), DirNor), 0.0);

	// �������
	vec3 diffuse = light.diffuse * cossAngle * vec3 ( texture(material.diffuse, TexCoords));

	// ������գ��߹⣩
	//vec3 viewDir = normalize(viewPose - fragPos);

	// �������
	vec3 reflectDir = reflect(-DirNor, normal);

	// �ȼ������߷����뷴�䷽��ĵ�ˣ���ȷ�������Ǹ�ֵ����Ȼ��ȡ����32���ݡ����32�Ǹ߹�ķ����(Shininess)
	// һ������ķ����Խ�ߣ�����������Խǿ��ɢ���Խ�٣��߹��ͻ�ԽС
	
	float spec = pow( max(dot(normalize(viewDir), normalize(reflectDir)), 0.0) , material.shininess );

	// �����
	vec3 specular = spec * light.specular * vec3(texture(material.specular, TexCoords));

	// Ƭ�ξ�����Դ��λ��
	float dis = distance(light.position, fragPos);

	float intensity = 1.0 / (light.constant + light.linear * dis + light.quadratic * dis * dis);

	return (ambient + diffuse + specular) * intensity;
}

vec3 CalcSpotLight(SpotLight light, vec3 normal,vec3 fragPos,vec3 viewDir)
{
		// ������
	vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
	
	// ���ߵ�λ����
	vec3 DirNor = normalize(-light.direction);	

	// cos�нǵ�ֵ �� ֵԽ�� �� �н�ԽС �� ����ǿ��Խ��
	float cossAngle = max( dot(normalize( normal), DirNor), 0.0);

	// �������
	vec3 diffuse = light.diffuse * cossAngle * vec3 ( texture(material.diffuse, TexCoords));

	// ������գ��߹⣩
	//vec3 viewDir = normalize(viewPose - fragPos);

	// �������
	vec3 reflectDir = reflect(-DirNor, normal);

	// �ȼ������߷����뷴�䷽��ĵ�ˣ���ȷ�������Ǹ�ֵ����Ȼ��ȡ����32���ݡ����32�Ǹ߹�ķ����(Shininess)
	// һ������ķ����Խ�ߣ�����������Խǿ��ɢ���Խ�٣��߹��ͻ�ԽС
	
	float spec = pow( max(dot(normalize(viewDir), normalize(reflectDir)), 0.0) , material.shininess );

	// �����
	vec3 specular = spec * light.specular * vec3(texture(material.specular, TexCoords));
	

	// �۹�Դ
	float theta = dot(normalize(light.direction) ,normalize(fragPos - light.position) );

	float intensity =clamp( (theta - light.outerCutOff)  / (light.inCutOff - light.outerCutOff) , 0.0, 1.0) ;

	return (ambient + diffuse + specular) * intensity;
}