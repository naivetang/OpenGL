#version 330 core

struct Material
{
	// 漫反射  贴图
	sampler2D diffuse;

	// 镜面反射
	//vec3 specular;

	// 镜面反射
	sampler2D specular;

	// 反光度
	float shininess;
};

uniform Material material;

out vec4 FragColor;

// 片段法向量
in vec3 Normal;

// 片段位置
in vec3 FragPos;

// 自己颜色
uniform vec3 objectColor;

// 光源颜色
uniform vec3 lightColor;

// 摄像机位置
uniform vec3 viewPose;

// 光照贴图位置
in vec2 TexCoords;

///////////////////////////////////////////////////////////////////////////

// 定向光源
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

// 点光源
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

// 聚光源
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

//冯氏着色 环境光 + 漫反射光 + 镜面光照

void main()
{
	vec3 normal = normalize(Normal);

	vec3 viewDir = normalize(viewPose - FragPos);

	vec3 result;

	// 第一阶段：定向光照
	 result += CalcDirLight(dirLight, normal, viewDir);

	// 第二阶段：点光源
	for(int i = 0; i < NR_POINT_LIGHTS; i++)
		result += CalcPointLight(pointLights[i], normal, FragPos, viewDir);

	// 第三阶段：聚光源
	result += CalcSpotLight(spotLight, normal, FragPos, viewDir);

	FragColor = vec4(result, 1.0f);

	return;		
}

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
	// 环境光
	vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));



	
	// 光线单位向量
	vec3 lightDir  = normalize(-light.direction);		

	// cos夹角的值 ， 值越大 ， 夹角越小 ， 光照强度越大
	float cossAngle = max( dot(normalize( normal), lightDir ), 0.0);

	// 漫反射光
	vec3 diffuse = light.diffuse * cossAngle * vec3 ( texture(material.diffuse, TexCoords));




	// 镜面光照（高光）
	//vec3 viewDir = normalize(viewPose - FragPos);

	// 反射光线
	vec3 reflectDir = reflect(-lightDir , normal);

	// 先计算视线方向与反射方向的点乘（并确保它不是负值），然后取它的32次幂。这个32是高光的反光度(Shininess)
	// 一个物体的反光度越高，反射光的能力越强，散射得越少，高光点就会越小
	
	float spec = pow( max(dot(normalize(viewDir), normalize(reflectDir)), 0.0) , material.shininess );

	vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));

	return (ambient + diffuse + specular);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{	

	// 环境光
	vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
	
	// 光线单位向量
	vec3 DirNor = normalize(light.position - fragPos);		

	//vec3 DirNor = normalize(-light.direction);	

	// cos夹角的值 ， 值越大 ， 夹角越小 ， 光照强度越大
	float cossAngle = max( dot(normalize( normal), DirNor), 0.0);

	// 漫反射光
	vec3 diffuse = light.diffuse * cossAngle * vec3 ( texture(material.diffuse, TexCoords));

	// 镜面光照（高光）
	//vec3 viewDir = normalize(viewPose - fragPos);

	// 反射光线
	vec3 reflectDir = reflect(-DirNor, normal);

	// 先计算视线方向与反射方向的点乘（并确保它不是负值），然后取它的32次幂。这个32是高光的反光度(Shininess)
	// 一个物体的反光度越高，反射光的能力越强，散射得越少，高光点就会越小
	
	float spec = pow( max(dot(normalize(viewDir), normalize(reflectDir)), 0.0) , material.shininess );

	// 反射光
	vec3 specular = spec * light.specular * vec3(texture(material.specular, TexCoords));

	// 片段距离点光源的位置
	float dis = distance(light.position, fragPos);

	float intensity = 1.0 / (light.constant + light.linear * dis + light.quadratic * dis * dis);

	return (ambient + diffuse + specular) * intensity;
}

vec3 CalcSpotLight(SpotLight light, vec3 normal,vec3 fragPos,vec3 viewDir)
{
		// 环境光
	vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
	
	// 光线单位向量
	vec3 DirNor = normalize(-light.direction);	

	// cos夹角的值 ， 值越大 ， 夹角越小 ， 光照强度越大
	float cossAngle = max( dot(normalize( normal), DirNor), 0.0);

	// 漫反射光
	vec3 diffuse = light.diffuse * cossAngle * vec3 ( texture(material.diffuse, TexCoords));

	// 镜面光照（高光）
	//vec3 viewDir = normalize(viewPose - fragPos);

	// 反射光线
	vec3 reflectDir = reflect(-DirNor, normal);

	// 先计算视线方向与反射方向的点乘（并确保它不是负值），然后取它的32次幂。这个32是高光的反光度(Shininess)
	// 一个物体的反光度越高，反射光的能力越强，散射得越少，高光点就会越小
	
	float spec = pow( max(dot(normalize(viewDir), normalize(reflectDir)), 0.0) , material.shininess );

	// 反射光
	vec3 specular = spec * light.specular * vec3(texture(material.specular, TexCoords));
	

	// 聚光源
	float theta = dot(normalize(light.direction) ,normalize(fragPos - light.position) );

	float intensity =clamp( (theta - light.outerCutOff)  / (light.inCutOff - light.outerCutOff) , 0.0, 1.0) ;

	return (ambient + diffuse + specular) * intensity;
}