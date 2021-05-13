#version 330
layout (points) in;
layout (points, max_vertices = 220) out;
in vec4 _color[];

in vec3 lPos[];
in vec3 vPos[];
in vec3 vNorm[];

// Light position
out vec3 plPos;

// Vertex position (in clip space)
out vec3 pvPos;

// Vertex normal
out vec3 pvNorm;

// in vec4 fColor;
out vec4 posit;

void main() {
	plPos = lPos[0];
	pvPos = vPos[0];
	pvNorm = vNorm[0];
    gl_Position = gl_in[0].gl_Position; 
    EmitVertex();
	gl_Position = gl_in[0].gl_Position + vec4(0.01, 0.01, 0.01, 0.01); 
    EmitVertex();
	gl_Position = gl_in[0].gl_Position + vec4(-0.01, 0.01, 0.01, 0.01);
	posit = gl_Position;
    EmitVertex();
	gl_Position = gl_in[0].gl_Position + vec4(0.01, -0.01, 0.01, 0.01); 
    EmitVertex();
	gl_Position = gl_in[0].gl_Position + vec4(0.01, 0.01, -0.01, 0.01); 
    EmitVertex();
	gl_Position = gl_in[0].gl_Position + vec4(0.01, -0.01, -0.01, 0.01); 
    EmitVertex();
	gl_Position = gl_in[0].gl_Position + vec4(-0.01, -0.01, -0.01, 0.01); 
    EmitVertex();
	gl_Position = gl_in[0].gl_Position + vec4(-0.01, -0.01, 0.01, 0.01); 
    EmitVertex();
	gl_Position = gl_in[0].gl_Position + vec4(-0.01, 0.01, -0.01, 0.01); 
    EmitVertex();
    EndPrimitive();
}