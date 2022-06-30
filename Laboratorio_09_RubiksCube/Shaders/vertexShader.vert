#version 330 core

layout (location = 0) in vec4 position;

uniform mat4 u_Proj;
uniform mat4 u_View;
uniform mat4 u_Model;

void main()
{
	mat4 MVP = u_Proj * u_View * u_Model;
	gl_Position = MVP * position;
}
