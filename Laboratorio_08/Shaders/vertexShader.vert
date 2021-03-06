#version 330 core

layout (location = 0) in vec4 position;
layout (location = 1) in vec2 texCoord;

out vec2 v_TexCoord;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Proj;

void main()
{
	mat4 MVP = u_Proj * u_View * u_Model;
	gl_Position = MVP * position;
	v_TexCoord = texCoord;
}
