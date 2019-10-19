#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

uniform sampler2D texture1;
uniform sampler2D texture2;

void main()
{
	// 采样纹理颜色。ourTexture：纹理采样器  TexCoord：纹理坐标
	FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), 0.2);
}
