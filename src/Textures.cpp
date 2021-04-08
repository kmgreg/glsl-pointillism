///
///  Textures
///
///  Simple class for setting up texture mapping parameters.
///
///  Created by Warren R. Carithers 2016/11/22.
///  Updates: 2019/11/23 by wrc.
///  Based on code created by Joe Geigel on 1/23/13.
///  Copyright 2019 Rochester Institute of Technology.  All rights reserved.
///
///  Contributor:  YOUR_NAME_HERE
///

#include <iostream>

#include "Textures.h"
#include "Shapes.h"
#include "Utils.h"

// this is here in case you are using SOIL;
// if you're not, it can be deleted.
#include <SOIL/SOIL.h>

using namespace std;

// These variables are used in both Phong shading and texture mapping
GLfloat cyl_specExp  = 20.0f;
GLfloat cube_specExp = 50.0f;
GLfloat sph_specExp  = 80.0f;
GLfloat k[3]         = { 0.5f, 0.7f, 1.0f };

// These variables are used in the framework only when doing Phong
// shading; they will be unused once you have implemented texture mapping.
GLfloat cyl_ambient[4]      = { 0.50f, 0.10f, 0.90f, 1.00f };
GLfloat cyl_diffuse[4]      = { 0.89f, 0.00f, 0.00f, 1.00f };
GLfloat cube_ambdiffuse[4]  = { 0.10f, 0.85f, 0.20f, 1.00f };
GLfloat sph_ambient[4]      = { 0.50f, 0.50f, 0.50f, 1.00f };
GLfloat sph_diffuse[4]      = { 0.49f, 0.49f, 0.80f, 1.00f };
GLfloat specular[4]         = { 1.00f, 1.00f, 1.00f, 1.00f };

// Add any global definitions and/or variables you need here.

///
/// This function initializes all texture-related data structures for
/// the program.  This is where texture buffers should be created, where
/// texture images should be loaded, etc.
///
/// You will need to write this function, and maintain all of the values
/// needed to be sent to the various shaders.
///
void initTextures( void )
{
    //
    // Add your code here.
    //

}

///
/// This function sets up the parameters for texture use.
///
/// You will need to modify this function, and maintain all of the values
/// needed to be sent to the various shaders.
///
/// @param program The ID of an OpenGL (GLSL) shader program to which
///    parameter values are to be sent
/// @param obj     The object type of the object being drawn
///
void setTextures( GLuint program, int obj )
{
    //
    // Add your code here to implement texture mapping.
    //
}
