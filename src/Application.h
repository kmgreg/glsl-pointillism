///
///  Application.h
///
///  Assignment-specific code.
///
///  Created by Warren R. Carithers on 2019/09/07.
///  Based on earlier versions created by Joe Geigel and Warren R. Carithers
///  Copyright 2019 Rochester Institute of Technology. All rights reserved.
///
///  This file should not be modified by students.
///

#ifndef APPLICATION_H_
#define APPLICATION_H_

#include <cstdlib>
#include <iostream>

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#endif

//
// GLEW and GLFW header files also pull in the OpenGL definitions
//

#ifndef __APPLE__
#include <GL/glew.h>
#endif

#include <GLFW/glfw3.h>

///
/// PUBLIC GLOBALS
///

///
/// Drawing-related variables
///

// dimensions of the drawing window
extern int w_width;
extern int w_height;

// drawing window title
extern const char *w_title;

// GL context we're using (we assume 3.0, for GLSL 1.30)
extern int gl_maj;
extern int gl_min;

// our GLFWwindow
extern GLFWwindow *w_window;

///
/// PUBLIC FUNCTIONS
///

///
/// Assignment-specific processing
///
void application( int argc, char *argv[] );

#endif
