#version 330 core
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

void main()
{

	// 光线单位向量
	vec3 DirNor = normalize(lightPos - FragPos);

	// cos夹角的值 ， 值越大 ， 夹角越小 ， 光照强度越大
	float cossAngle = max( dot(normalize( Normal), DirNor), 0.0);

	// 漫反射光
	vec3 diffuse = cossAngle * lightColor;

	float ambientStrength = 0.1;

	// 环境光
	vec3 ambient = ambientStrength * lightColor;

	FragColor = vec4((diffuse + ambient)* objectColor, 1.0);
}
