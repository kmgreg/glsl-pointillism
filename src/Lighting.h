///
///  Lighting.h
///
///  Simple module for setting up the parameters for lighting and shading
///  for this assignment.
///
///  Created by Warren R. Carithers 2019/11/18.
///  Copyright 2019 Rochester Institute of Technology.  All rights reserved.
///

#ifndef LIGHTING_H_
#define LIGHTING_H_

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#endif

#ifndef __APPLE__
#include <GL/glew.h>
#endif

#include <GLFW/glfw3.h>

// get the object identifiers
#include "Shapes.h"

///
// This function sets up the lighting parameters for the Phong shader.
//
// @param program  The ID of an OpenGL (GLSL) shader program to which
//                 parameter values are to be sent
///
void setLighting( GLuint program );

///
// This function sets up the material and shading parameters
// for the Phong shader.
//
// @param program  The ID of an OpenGL (GLSL) shader program to which
//                 parameter values are to be sent
// @param object   The ID of the object being shaded
///
void setMaterials( GLuint program, int object );

#endif
