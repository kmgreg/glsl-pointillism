#shader vertex
#version 430 core
//@author Lauren Cole
layout(location = 0) in vec4 position;

out vec4 _color;
//model view projection matrix
uniform mat4 u_view;
uniform mat4 u_proj;
uniform mat4 u_model;
void main()
{
	_color = vec4(0.1, 0.7, 1.0, 1.0);
	gl_PointSize = 2.0;
	gl_Position = u_proj * u_view * u_model * position;
};

#shader fragment
#version 430 core
layout(location = 0) out vec4 color;
in vec4 _color;
void main()
{
	color = _color;
};