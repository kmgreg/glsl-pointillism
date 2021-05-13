#shader vertex
#version 430 core
//@author Lauren Cole
layout(location = 0) in vec4 position;
layout(location = 1) in  vec3 velocity;
layout(location = 2) in float age;

out float v_particleLifeRemaining;

//model view projection matrix
uniform mat4 u_view;
uniform mat4 u_proj;
uniform mat4 u_model;
void main()
{
	v_particleLifeRemaining = age;
	gl_Position = u_proj * u_view * u_model * position;	
};

#shader fragment
#version 430 core
in float v_particleLifeRemaining;
layout(location = 0) out vec4 color;
uniform vec4 u_color;
uniform vec3 u_lerpColors[6];



void main()
{
	vec3 interp;
	vec4 final;
	float increment = 0.167f;
	if (v_particleLifeRemaining > (1.0f - increment))
	{
		interp = mix(u_lerpColors[1], u_lerpColors[0], (v_particleLifeRemaining - (5.0 * increment))/ increment);
		final = vec4(interp, 1.0f);
	}
	else if (v_particleLifeRemaining > (1.0f - (increment * 2.0f)))
	{
		interp = mix(u_lerpColors[2], u_lerpColors[1], (v_particleLifeRemaining - (4.0f * increment)) / increment);
		final = vec4(interp, 1.0f);
	}
	else if (v_particleLifeRemaining > (1.0f - (increment * 3.0f)))
	{
		interp = mix(u_lerpColors[3], u_lerpColors[2], (v_particleLifeRemaining - (3.0f * increment)) / increment);
		final = vec4(interp, 1.0f);
	}
	else if (v_particleLifeRemaining > 1.0f - (increment * 4.0f))
	{
		interp = mix(u_lerpColors[4], u_lerpColors[3], (v_particleLifeRemaining - (2.0f*increment)) / increment);
		final = vec4(interp, 1.0f);
	}
	else if (v_particleLifeRemaining > 1.0f - (increment * 5.0f))
	{
		interp = mix(u_lerpColors[5], u_lerpColors[4], (v_particleLifeRemaining - (increment)) / increment);
		final = vec4(interp, 1.0f);
	}
	else
	{
		interp = mix(vec3(0.0f, 0.0f, 0.0f), u_lerpColors[5], v_particleLifeRemaining / increment);
		final = vec4(interp, 1.0f);
	}
	final[3] = 0.9f;
	color = final;
	//color = u_color;
};