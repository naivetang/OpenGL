#version 330 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aOffset;

out vec3 fColor;

//uniform vec2 offect[100];

void main()
{
	int index = gl_InstanceID;

	gl_Position = vec4( aPos + aOffset, 0.0, 1.0);

	//gl_Position = vec4( aPos, 0.0, 1.0);

	fColor = aColor;
}