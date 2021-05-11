#version 330 core
layout (points) in;
layout (points, max_vertices = 220) out;
in vec4 _color[];
out vec4 fColor;
out vec4 posit;
void main() {   
    fColor = _color[0];
    gl_Position = gl_in[0].gl_Position; 
	posit = gl_Position;
    EmitVertex();
	gl_Position = gl_in[0].gl_Position + vec4(0.01, 0.01, 0.01, 0.01); 
    EmitVertex();
	gl_Position = gl_in[0].gl_Position + vec4(-0.01, 0.01, 0.01, 0.01); 
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