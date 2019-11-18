#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

in vec3 Normal;

in vec3 Position;

struct Material
{
	// 漫反射  贴图
	sampler2D texture_diffuse1;

	// 镜面反射
	//vec3 specular;

	// 镜面反射 贴图 越白的地方镜面反射越强、越暗的地方镜面反射越弱
	//sampler2D specular;
	sampler2D texture_specular1;

	// 反光度
	float shininess;
};

uniform Material material;

uniform vec3 cameraPos;

uniform samplerCube skybox;

void main()
{ 
	vec3 I = Position - cameraPos;

	// 反射
	vec3 R = reflect(normalize(I), normalize(Normal));

	// 折射
	vec3 Z = refract(normalize(I), normalize(Normal), 1f / 1.52f);

    //FragColor = vec4( texture(material.texture_diffuse1, TexCoords).rgb ,1.0);
    FragColor = vec4( texture(skybox, R).rgb, 1.0 );
}	