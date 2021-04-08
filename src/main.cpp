///
///  main.cpp
///
///  Main program for assignments.
///
///  Created by Warren R. Carithers on 02/27/14.
///  Updates: 2019/09/09 by wrc.
///  Based on earlier versions created by Joe Geigel and Warren R. Carithers
///  Copyright 2019 Rochester Institute of Technology. All rights reserved.
///
///  This file should not be modified by students.
///

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

#include "Application.h"

using namespace std;

///
/// Event callback routines
///
/// We define a general GLFW callback routine; all others must be
/// defined and registered in the assignment-specific code.
///

///
/// Error callback for GLFW
///
void glfwError( int code, const char *desc )
{
    cerr << "GLFW error " << code << ": " << desc << endl;
    exit( 2 );
}


///
/// Main program for this assignment
///
int main( int argc, char *argv[] )
{
    glfwSetErrorCallback( glfwError );

    if( !glfwInit() ) {
        cerr << "Can't initialize GLFW!" << endl;
        exit( 1 );
    }

    // You may need to uncomment the following four function calls
    // if you are unable to get a 3.2 rendering context on your
    // system.  This is more likely on Mac systems because of
    // Apple's restriction of OpenGL beyond 2.1 to using only
    // the core profile.

    // ensure we have at least OpenGL 3.2
    // glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 );
    // glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 2 );

    // also select the "forward compatible" core profile
    // glfwWindowHint( GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE );
    // glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );

    // w_width, w_height, and w_title come from the Application module
    w_window = glfwCreateWindow( w_width, w_height, w_title, NULL, NULL );

    if( !w_window ) {
        cerr << "GLFW window create failed!" << endl;
        glfwTerminate();
        exit( 1 );
    }

    glfwMakeContextCurrent( w_window );

#ifndef __APPLE__
    GLenum err = glewInit();
    if( err != GLEW_OK ) {
        cerr << "GLEW error: " << glewGetErrorString(err) << endl;
        glfwTerminate();
        exit( 1 );
    }

    if( !GLEW_VERSION_3_2 ) {
        cerr << "OpenGL 3.2 not available" << endl;
        if( !GLEW_VERSION_2_1 ) {
            cerr << "OpenGL 2.1 not available, either!" << endl;
            glfwTerminate();
            exit( 1 );
        }
    }
#endif

    // find out which OpenGL context version we're able to use
    gl_maj = glfwGetWindowAttrib( w_window, GLFW_CONTEXT_VERSION_MAJOR );
    gl_min = glfwGetWindowAttrib( w_window, GLFW_CONTEXT_VERSION_MINOR );

    cerr << "GLFW: using " << gl_maj << "." << gl_min << " context" << endl;

    // do all application-specific work
    application( argc, argv );

    // all done - shut everything down cleanly
    glfwDestroyWindow( w_window );
    glfwTerminate();

    return 0;
}
