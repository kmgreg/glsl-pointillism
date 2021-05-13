#version 330 core
layout (points) in;
layout (points, max_vertices = 256) out;
in vec4 _color[];
out vec4 fColor;
out vec4 posit;

uniform float u_pointDistance;

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

void main() {   
    fColor = _color[0];
    
    gl_Position = gl_in[0].gl_Position; 
    EmitVertex();
	gl_Position = gl_in[0].gl_Position + vec4(u_pointDistance, u_pointDistance, u_pointDistance, u_pointDistance); 
    EmitVertex();
	gl_Position = gl_in[0].gl_Position + vec4(-u_pointDistance, u_pointDistance, u_pointDistance, u_pointDistance);
	posit = gl_Position;
    EmitVertex();
	gl_Position = gl_in[0].gl_Position + vec4(u_pointDistance, -u_pointDistance, u_pointDistance, u_pointDistance); 
    EmitVertex();
	gl_Position = gl_in[0].gl_Position + vec4(u_pointDistance, u_pointDistance, -u_pointDistance, u_pointDistance); 
    EmitVertex();
	gl_Position = gl_in[0].gl_Position + vec4(u_pointDistance, -u_pointDistance, -u_pointDistance, u_pointDistance); 
    EmitVertex();
	gl_Position = gl_in[0].gl_Position + vec4(-u_pointDistance, -u_pointDistance, -u_pointDistance, u_pointDistance); 
    EmitVertex();
	gl_Position = gl_in[0].gl_Position + vec4(-u_pointDistance, -u_pointDistance, u_pointDistance, u_pointDistance); 
    EmitVertex();
	gl_Position = gl_in[0].gl_Position + vec4(-u_pointDistance, u_pointDistance, -u_pointDistance, 0.01); 
    EmitVertex();
    EndPrimitive();
}  