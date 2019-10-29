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

struct Light
{
	// 定向光
	vec3 direction;

	vec3 position;
	

	vec3 ambient;

	vec3 diffuse;

	vec3 specular;


	// 点光源
	// 强度衰减公式 ：1 / （contant  + linear * d + quadratic * d * d ）

	// 常量，让公式最终值不大于1
	float constant;

	// 一次项系数
	float linear;

	// 二次项系数
	float quadratic;

	// 聚光源
	//vec3 direction;
	//vec3 position;
	float inCutOff;

	float outerCutOff;

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

// 摄像机位置
uniform vec3 viewPose;

// 光照贴图位置
in vec2 TexCoords;

//冯氏着色 环境光 + 漫反射光 + 镜面光照

void main()
{
	// 环境光
	vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
	
	// 光线单位向量
	vec3 DirNor = normalize(light.position - FragPos);	

	// 片段距离点光源的位置
	float dis = distance(light.position, FragPos);

	//vec3 DirNor = normalize(-light.direction);	

	// cos夹角的值 ， 值越大 ， 夹角越小 ， 光照强度越大
	float cossAngle = max( dot(normalize( Normal), DirNor), 0.0);

	// 漫反射光
	vec3 diffuse = light.diffuse * cossAngle * vec3 ( texture(material.diffuse, TexCoords));

	// 镜面光照（高光）
	vec3 viewDir = normalize(viewPose - FragPos);

	// 反射光线
	vec3 reflectDir = reflect(-DirNor, Normal);

	// 先计算视线方向与反射方向的点乘（并确保它不是负值），然后取它的32次幂。这个32是高光的反光度(Shininess)
	// 一个物体的反光度越高，反射光的能力越强，散射得越少，高光点就会越小
	
	float spec = pow( max(dot(normalize(viewDir), normalize(reflectDir)), 0.0) , material.shininess );

	vec3 specular = spec * light.specular * vec3(texture(material.specular, TexCoords));

	// 点光源
	//FragColor = vec4((specular + diffuse + ambient) * objectColor * (1 / (light.constant + light.linear * dis + light.quadratic * dis * dis)), 1.0);

	// 聚光源
	float theta = dot(normalize(light.direction) ,normalize(FragPos - light.position) );

	float intensity =clamp( (theta - light.outerCutOff)  / (light.inCutOff - light.outerCutOff) , 0.0, 1.0) ;

	FragColor = vec4((specular + diffuse + ambient) * intensity * objectColor, 1.0);

	return;

	if(theta > light.inCutOff)
		FragColor = vec4((specular + diffuse + ambient) * objectColor, 1.0);
	else
		FragColor = vec4(ambient * objectColor, 1.0);
		
}
