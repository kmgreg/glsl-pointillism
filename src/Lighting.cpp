///
///  Lighting
///
///  Simple module for setting up the parameters for lighting and shading
///  for this assignment.
///
///  Created by Warren R. Carithers 2019/11/18.
///  Copyright 2019 Rochester Institute of Technology.  All rights reserved.
///

#include <iostream>

#include "Lighting.h"
#include "Shapes.h"
#include "Utils.h"

using namespace std;

// Add any global variables you need here.

// lighting data
GLfloat lightcolor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat lightpos[4]   = { 4.0f, 1.0f, 2.0f, 1.0f };
GLfloat amblight[4]   = { 0.5f, 0.5f, 0.5f, 1.0f };

// cube properties
GLfloat amb_cube[4]   = { 0.0f, 1.0f, 0.7f, 1.0f };
GLfloat diff_cube[4]  = { 0.0f, 1.0f, 0.7f, 1.0f };
GLfloat spec_cube[4]  = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat exp_cube      = 50.0f;
GLfloat k_cube[3]     = { 0.5f, 0.7f, 1.0f };

// cylinder properties
GLfloat amb_cyl[4]    = { 1.0f, 0.3f, 0.0f, 1.0f };
GLfloat diff_cyl[4]   = { 1.0f, 0.3f, 0.0f, 1.0f };
GLfloat spec_cyl[4]   = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat exp_cyl       = 30.0f;
GLfloat k_cyl[3]      = { 0.5f, 0.7f, 1.0f };

// sphere properties
GLfloat amb_sph[4]    = { 1.0f, 0.3f, 0.7f, 1.0f };
GLfloat diff_sph[4]   = { 1.0f, 0.3f, 0.7f, 1.0f };
GLfloat spec_sph[4]   = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat exp_sph       = 70.0f;
GLfloat k_sph[3]      = { 0.5f, 0.7f, 1.0f };

///
// This function sets up the lighting parameters
//
// @param program The ID of an OpenGL (GLSL) shader program to which
//                parameter values are to be sent
///
void setLighting( GLuint program )
{
    GLint loc;

    // Lighting parameters
    loc = getUniformLoc( program, "lightPosition" );
    glUniform4fv( loc, 1, lightpos );

    loc = getUniformLoc( program, "lightColor" );
    glUniform4fv( loc, 1, lightcolor );

    loc = getUniformLoc( program, "ambientLight" );
    glUniform4fv( loc, 1, amblight );
}

///
// This function sets up the material and shading parameters
// for the shaders.
//
// @param program The ID of an OpenGL (GLSL) shader program to which
//                parameter values are to be sent
// @param object   The ID of the object being shaded
///
void setMaterials( GLuint program, int object )
{
    // Object properties
    GLint dcolor = getUniformLoc( program, "diffuseColor" );
    GLint acolor = getUniformLoc( program, "ambientColor" );
    GLint coeff  = getUniformLoc( program, "kCoeff" );
    GLint scolor = getUniformLoc( program, "specularColor" );
    GLint expon  = getUniformLoc( program, "specExp" );

    switch( object ) {
    case OBJ_CUBE:
        glUniform4fv( dcolor, 1, diff_cube );
        glUniform4fv( acolor, 1, amb_cube );
        glUniform3fv( coeff,  1, k_cube );
        glUniform4fv( scolor, 1, spec_cube );
        glUniform1f(  expon,  exp_cube );
        break;

    case OBJ_CYLINDER:
        glUniform4fv( dcolor, 1, diff_cyl );
        glUniform4fv( acolor, 1, amb_cyl );
        glUniform3fv( coeff,  1, k_cyl );
        glUniform4fv( scolor, 1, spec_cyl );
        glUniform1f(  expon,  exp_cyl );
        break;

    case OBJ_SPHERE:
        glUniform4fv( dcolor, 1, diff_sph );
        glUniform4fv( acolor, 1, amb_sph );
        glUniform3fv( coeff,  1, k_sph );
        glUniform4fv( scolor, 1, spec_sph );
        glUniform1f(  expon,  exp_sph );
        break;

    default:
        cerr << "ERROR unknown object " << object << " - ignoring" << endl;
    }
}
