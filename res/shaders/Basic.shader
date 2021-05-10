#shader vertex
#version 430 core
//@author Lauren Cole
layout(location = 0) in vec4 position;
layout(location = 1) in vec4 velocity;
layout(location = 2) in vec4 age;
layout(location = 3) in vec4 colorIn;
out vec4 _color;
//model view projection matrix
uniform mat4 u_view;
uniform mat4 u_proj;
uniform mat4 u_model;
void main()
{
	_color = colorIn;
	gl_PointSize = mix(10.0, 1.0, age[0]);
	gl_Position = u_proj * u_view * u_model * position;
};

#shader fragment
#version 430 core
layout(location = 0) out vec4 color;
in vec4 fColor;
void main()
{
	color = fColor;
};