#version 150

//
// Fragment shader for Lab 2
//
// @author  RIT CS Department
// @author  YOUR_NAME_HERE
//

// Light position
in vec3 lPos;

// Vertex position (in clip space)
in vec3 vPos;

// Vertex normal
in vec3 vNorm;

// Texture coordinates
in vec2 texCoord;

// Light color
uniform vec4 lightColor;
uniform vec4 ambientLight;

// Material properties
uniform vec4 diffuseColor;
uniform vec4 ambientColor;
uniform vec4 specularColor;
uniform float specExp;
uniform vec3 kCoeff;

// Incoming data from the application

// Textures
uniform sampler2D material;

// OUTGOING DATA

// The final fragment color
out vec4 fragmentColor;

void main()
{
    // calculate lighting vectors
    vec3 L = normalize( lPos - vPos );
    vec3 N = normalize( vNorm );
    vec3 R = normalize( reflect(-L, N) );
    vec3 V = normalize( -(vPos) );

    vec4 ambient  = vec4(0.0);  // ambient color component
    vec4 diffuse  = vec4(0.0);  // diffuse color component
    vec4 specular = vec4(0.0);  // specular color component
    float specDot;  // specular dot(R,V) ^ specExp value

    //
    // The following code implements Phong shading.  It must be
    // modified to perform texture mapping using Phong.
    //

    ambient  = ambientLight * ambientColor;
    diffuse  = lightColor * diffuseColor * max(dot(N,L),0.0);
    specDot  = pow( max(dot(R,V),0.0), specExp );
    specular = lightColor * specularColor * specDot;

    ambient  = ambientLight * ambientColor;
    diffuse  = lightColor * diffuseColor * max(dot(N,L),0.0);
    specDot  = pow( max(dot(R,V),0.0), specExp );
    specular = lightColor * specularColor * specDot;

    vec4 color = (kCoeff.x * ambient) +
                 (kCoeff.y * diffuse) +
                 (kCoeff.z * specular);

    // fragmentColor = color * vec4( texCoord, 1.0, 1.0 );
    fragmentColor = color;
}
