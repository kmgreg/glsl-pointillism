#version 330 core
layout (points) in;
layout (points, max_vertices = 220) out;
in vec4 _color[];
out vec4 fColor;
void main() {   
    fColor = _color[0];
    gl_Position = gl_in[0].gl_Position; 
    EmitVertex();
    EndPrimitive();
}  