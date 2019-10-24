#version 330 core

struct Material
{
	// 环境光
	vec3 ambient;

	// 漫反射
	vec3 diffuse;

	// 镜面反射
	vec3 specular;

	// 反光度
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

// 片段法向量
in vec3 Normal;

// 片段位置
in vec3 FragPos;

// 自己颜色
uniform vec3 objectColor;

// 光源颜色
uniform vec3 lightColor;

// 光源位置
uniform vec3 lightPos;

// 摄像机位置
uniform vec3 viewPose;

//冯氏着色 环境光 + 漫反射光 + 镜面光照

void main()
{
	// 环境光
	vec3 ambient = light.ambient * material.ambient;
	
	
	// 光线单位向量
	vec3 DirNor = normalize(lightPos - FragPos);	

	// cos夹角的值 ， 值越大 ， 夹角越小 ， 光照强度越大
	float cossAngle = max( dot(normalize( Normal), DirNor), 0.0);

	// 漫反射光
	vec3 diffuse = light.diffuse * (cossAngle * material.diffuse);

	// 镜面光照（高光）
	vec3 viewDir = normalize(viewPose - FragPos);

	// 反射光线
	vec3 reflectDir = reflect(-DirNor, Normal);

	// 先计算视线方向与反射方向的点乘（并确保它不是负值），然后取它的32次幂。这个32是高光的反光度(Shininess)
	// 一个物体的反光度越高，反射光的能力越强，散射得越少，高光点就会越小
	float spec = pow( max(dot(normalize(viewDir), normalize(reflectDir)), 0.0) , material.shininess );

	vec3 specular = spec * material.specular * light.specular;

	FragColor = vec4((specular + diffuse + ambient)* objectColor, 1.0);
}
