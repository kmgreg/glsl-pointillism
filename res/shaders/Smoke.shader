#shader vertex
#version 430 core
//@author Lauren Cole
layout(location = 0) in vec4 position;
layout(location = 1) in vec4 velocity;
layout(location = 2) in vec4 age;
layout(location = 3) in vec4 colorIn;
out vec4 _color;
out vec4 _age;
out float _pointSize;
//model view projection matrix
uniform mat4 u_view;
uniform mat4 u_proj;
uniform mat4 u_model;
void main()
{
	_color = colorIn;
	_age = age;
	_pointSize = mix(60.0, 10.0f, age[0]);
	
	//_pointSize = 128.0f;
	gl_PointSize = _pointSize;
	gl_Position = u_proj * u_view * u_model * position;
};

#shader fragment
#version 430 core
layout(location = 0) out vec4 color;
in vec4 _color;
in vec4 _age;
in float _pointSize;
uniform sampler2D u_text;
void main()
{
	color = _color;
};