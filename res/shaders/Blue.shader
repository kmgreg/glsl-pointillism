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
    gl_PointSize = 1.0f;
	_color = vec4(0.1, 0.7, 1.0, 1.0);
	gl_Position = u_proj * u_view * u_model * position;
};

#shader fragment
#version 430 core
layout(location = 0) out vec4 color;
in vec4 fColor;
in vec4 posit;
// From https://thebookofshaders.com/11/
vec2 skew (vec2 st) {
    vec2 r = vec2(0.0);
    r.x = 1.1547*st.x;
    r.y = st.y+0.5*r.x;
    return r;
}

vec3 simplexGrid (vec2 st) {
    vec3 xyz = vec3(0.0);

    vec2 p = fract(skew(st));
    if (p.x > p.y) {
        xyz.xy = 1.0-vec2(p.x,p.y-p.x);
        xyz.z = p.y;
    } else {
        xyz.yz = 1.0-vec2(p.x-p.y,p.y);
        xyz.x = p.x;
    }

    return fract(xyz);
}
void main()
{
	color = fColor + 0.2 * vec4(simplexGrid(100 * tan(posit.x) * posit.xy), 1)	; // Lightly modulate
};