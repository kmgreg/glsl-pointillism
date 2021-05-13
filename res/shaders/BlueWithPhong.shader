#shader vertex
#version 330 compatibility	
//@author Lauren Cole
//@authot Kurt Gregorek
//Phong based on Lab 2
layout(location = 0) in vec4 position;
layout(location = 1) in vec3 normal;

out vec4 _color;
//model view projection matrix
uniform mat4 u_view;
uniform mat4 u_proj;
uniform mat4 u_model;
uniform vec4 lightPos;

out vec3 lPos;
out vec3 vPos;
out vec3 vNorm;

void main()
{
    gl_PointSize = 1.0f;
	_color = vec4(0.1, 0.7, 1.0, 1.0);
	gl_Position = u_proj * u_view * u_model * position;
	lPos = (u_view * lightPos).xyz;
	
	mat4 mvMat = u_view * u_model;
	
	vPos = (mvMat * position).xyz;
	
	mat3 normMat = inverse( transpose( mat3(mvMat) ) );
	vNorm = (normMat * normal).xyz;
};

#shader fragment
#version 330 compatibility
layout(location = 0) out vec4 color;
// Light color
uniform vec4 lightColor;
uniform vec4 ambientLight;

// Material properties
uniform vec4 diffuseColor;
uniform vec4 ambientColor;
uniform vec4 specularColor;
uniform float specExp;
uniform vec3 kCoeff;

// Light position
in vec3 plPos;

// Vertex position (in clip space)
in vec3 pvPos;

// Vertex normal
in vec3 pvNorm;

// in vec4 fColor;
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
	// calculate lighting vectors
    vec3 L = normalize( plPos - pvPos );
    vec3 N = normalize( pvNorm );
    vec3 R = normalize( reflect(-L, N) );
    vec3 V = normalize( -(pvPos) );

    vec4 ambient  = vec4(0.0);  // ambient color component
    vec4 diffuse  = vec4(0.0);  // diffuse color component
    vec4 specular = vec4(0.0);  // specular color component
    float specDot;

    // From Lab 2

    ambient  = ambientLight * ambientColor;
    diffuse  = lightColor * diffuseColor * max(dot(N,L),0.0);
    specDot  = pow( max(dot(R,V),0.0), specExp );
    specular = lightColor * specularColor * specDot;

    vec4 phongResult = (kCoeff.x * ambient) +
                 (kCoeff.y * diffuse) +
                 (kCoeff.z * specular);

	color = phongResult + 0.3 * vec4(simplexGrid(100 * tan(posit.x) * posit.xy), 1)	; // Lightly modulate
};