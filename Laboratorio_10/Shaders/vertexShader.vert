#version 330 core

layout (location = 0) in vec4 position;
layout (location = 1) in vec2 texCoord;

out vec2 v_TexCoord;

uniform mat4 u_Proj;
uniform mat4 u_View;

uniform mat4 u_Trans;
uniform mat4 u_Rot;
uniform mat4 u_Scale;

void main()
{
	mat4 TRS = u_Trans * u_Rot * u_Scale;
	mat4 MVP = u_Proj * u_View * TRS;
	gl_Position = MVP * position;
	v_TexCoord = texCoord;
}
