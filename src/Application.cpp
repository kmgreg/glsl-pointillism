///
///  Application.cpp
///
///  Assignment-specific code.
///
///  Created by Warren R. Carithers on 2019/09/09.
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

#include "Types.h"
#include "ShaderSetup.h"
#include "Buffers.h"
#include "Canvas.h"
#include "Utils.h"

#include "Application.h"
#include "Viewing.h"
#include "Textures.h"
#include "Lighting.h"
#include "Shapes.h"

// .obj file loader
#include "OBJ_Loader.h"

using namespace std;

///
/// PRIVATE GLOBALS
///

///
/// Object information
///

// cube transformations
static GLfloat cube_scale[] = {  0.5f, 1.0f, 0.5f };
static GLfloat cube_xlate[] = { -1.5f, 0.0f, -1.5f };
static GLfloat cube_rotate[] = { 0.0f, 25.0f, 0.0f };

// cylinder transformations
static GLfloat cyl_scale[] = { 0.5f, 1.0f, 0.5f };
static GLfloat cyl_xlate[] = { 0.0f, 0.0f, -3.5f };
static GLfloat cyl_rotate[] = { 0.0f, 0.0f, 0.0f };

// sphere transformations
static GLfloat sph_scale[] = { 0.5f, 0.5f, 0.5f };
static GLfloat sph_xlate[] = { 1.5f, 0.0f, -0.5f };
static GLfloat sph_rotate[] = { 0.0f, 0.0f, 0.0f };

// our Canvas
static Canvas *canvas;

// We need three vertex buffers for our shapes
static BufferSet buffers[N_OBJECTS];

// shader program handle
static GLuint program;

// names of our object files
static const char *cubeFile = "cube.obj";
static const char *cylinderFile = "cylinder.obj";
static const char *sphereFile = "sphere.obj";

// object loader data
objl::Loader cubeData;
objl::Loader cylinderData;
objl::Loader sphereData;

// names of our GLSL shader files
static const char *vshader = "texture.vert";
static const char *fshader = "texture.frag";

///
// PUBLIC GLOBALS
///

///
// Drawing-related variables
///

// dimensions of the drawing window
int w_width  = 600;
int w_height = 600;

// drawing window title
const char *w_title = (char *) "Lab 2";

// GL context we're using (we assume 3.2, for GLSL 1.50)
int gl_maj = 3;
int gl_min = 2;

// our GLFWwindow
GLFWwindow *w_window;

///
// PRIVATE FUNCTIONS
///

///
/// makeShape() - load one object's data into the canvas
///
/// @param object    the object data to use
/// @param C         the Canvas object to be used
/// @param which     which model we want within that data
///
void makeShape( objl::Loader object, Canvas &C, int which ) {
    objl::Mesh mesh = object.LoadedMeshes[which];

    // we need to get the vertices in triangle order, so
    // we will iterate on the indices

    for( int i = 0; i < mesh.Indices.size(); i += 3 ) {
        int i1 = mesh.Indices[i];
        int i2 = mesh.Indices[i+1];
        int i3 = mesh.Indices[i+2];

        Vertex v1 = { mesh.Vertices[i1].Position.X,
                      mesh.Vertices[i1].Position.Y,
                      mesh.Vertices[i1].Position.Z };

        Vertex v2 = { mesh.Vertices[i2].Position.X,
                      mesh.Vertices[i2].Position.Y,
                      mesh.Vertices[i2].Position.Z };

        Vertex v3 = { mesh.Vertices[i3].Position.X,
                      mesh.Vertices[i3].Position.Y,
                      mesh.Vertices[i3].Position.Z };

#if defined(USE_CALCULATED_NORMALS)
        // let addTriangle() calculate normals
        C.addTriangle( v1, v2, v3 );
#else
        // use normals from the .obj file
        Normal n1 = { mesh.Vertices[i1].Normal.X,
                      mesh.Vertices[i1].Normal.Y,
                      mesh.Vertices[i1].Normal.Z };

        Normal n2 = { mesh.Vertices[i2].Normal.X,
                      mesh.Vertices[i2].Normal.Y,
                      mesh.Vertices[i2].Normal.Z };

        Normal n3 = { mesh.Vertices[i3].Normal.X,
                      mesh.Vertices[i3].Normal.Y,
                      mesh.Vertices[i3].Normal.Z };

        C.addTriangleWithNorms( v1, n1, v2, n2, v3, n3 );
#endif

        TexCoord t1 = { mesh.Vertices[i1].TextureCoordinate.X,
                        mesh.Vertices[i1].TextureCoordinate.Y };

        TexCoord t2 = { mesh.Vertices[i2].TextureCoordinate.X,
                        mesh.Vertices[i2].TextureCoordinate.Y };

        TexCoord t3 = { mesh.Vertices[i3].TextureCoordinate.X,
                        mesh.Vertices[i3].TextureCoordinate.Y };

        C.addTextureCoords( t1, t2, t3 );
    }
}

///
/// createShapes() - create vertex and element buffers for our shapes
///
static void createShapes( void )
{
    // clear any previous shape
    canvas->clear();

    // make the cube and its buffers
    makeShape( cubeData, *canvas, 0 );
    buffers[OBJ_CUBE].createBuffers( *canvas );

    // ditto for the others
    canvas->clear();
    makeShape( cylinderData, *canvas, 0 );
    buffers[OBJ_CYLINDER].createBuffers( *canvas );

    canvas->clear();
    makeShape( sphereData, *canvas, 0 );
    buffers[OBJ_SPHERE].createBuffers( *canvas );
}

///
/// Event callback routines for this assignment
///

///
/// Handle keyboard input
///
static void keyboard( GLFWwindow *window, int key, int scan,
                      int action, int mods )
{
    if( action != GLFW_PRESS ) {
        return;
    }

    switch( key ) {
    case GLFW_KEY_H:  // help message
        cout << "Key(s)       Action" << endl;
        cout << "==========   =======================" << endl;
        cout << " ESC, q, Q   Terminate the program" << endl;
        break;

    case GLFW_KEY_ESCAPE:  // terminate the program
    case GLFW_KEY_Q:
        glfwSetWindowShouldClose( window, 1 );
        break;
    }
}

///
// Display the current image
///
static void display( void )
{
    // clear the frame buffer
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    // select the shader program
    glUseProgram( program );

    // set up projection parameters
    setFrustum( program );

    // set up lighting for the scene
    setLighting( program );

    // set up the view transformation
    setCamera( program );

    checkErrors( "display common" );

    // draw the individual objects
    for( int obj = 0; obj < N_OBJECTS; ++obj ) {

        // set up material property data
        setMaterials( program, obj );

        // set up texture/shading information
        setTextures( program, obj );

        checkErrors( "display object 1" );

        // send all the transformation data
        switch( obj ) {
        case OBJ_CUBE:
            setTransforms( program, cube_scale, cube_rotate, cube_xlate );
            break;
        case OBJ_SPHERE:
            setTransforms( program, sph_scale, sph_rotate, sph_xlate );
            break;
        case OBJ_CYLINDER:
            setTransforms( program, cyl_scale, cyl_rotate, cyl_xlate );
            break;
        }

        checkErrors( "display object 2" );

        // draw it
        buffers[obj].selectBuffers( program,
            "vPosition", NULL, "vNormal", "vTexCoord" );

        checkErrors( "display object 3" );

        glDrawElements( GL_TRIANGLES, buffers[obj].numElements,
            GL_UNSIGNED_INT, (void *)0 );

        checkErrors( "display object 4" );
    }
}

///
/// OpenGL initialization
///
static bool init( void )
{
    // Create our Canvas "object"
    canvas = new Canvas( w_width, w_height );

    if( canvas == NULL ) {
        cerr << "error - cannot create Canvas" << endl;
        return( false );
    }

    // Check the OpenGL version - we need at least OpenGL 3.2
    // in order to get GLSL 1.50
    if( gl_maj < 3 || (gl_maj == 3 && gl_min < 2) ) {
        cerr << "Cannot use GLSL 1.50 - shaders won't compile!" << endl;
        cerr << "Bailing out!" << endl;
        return( false );
    }

    // Load objects
    if( !cubeData.LoadFile( cubeFile ) ) {
        cerr << "Error loading " << cubeFile << endl;
        return( false );
    }
    if( !cylinderData.LoadFile( cylinderFile ) ) {
        cerr << "Error loading " << cylinderFile << endl;
        return( false );
    }
    if( !sphereData.LoadFile( sphereFile ) ) {
        cerr << "Error loading " << sphereFile << endl;
        return( false );
    }

    // Load shaders and use the resulting shader program
    ShaderError error;
    program = shaderSetup( vshader, fshader, &error );
    if( !program ) {
        cerr << "Error setting up shaders - "
             << errorString(error) << endl;
        return( false );
    }

#if 0
    // this code can be enabled if you need to dump out the
    // shader variables for the three shaders

    cout << "Shader actives" << endl;
    cout << "----------" << endl << "Flat shader " << flat << endl;
    dumpActives( flat );
    cout << "----------" << endl << "Gouraud shader " << gouraud << endl;
    dumpActives( gouraud );
    cout << "----------" << endl << "Phong shader " << phong << endl;
    dumpActives( phong );
#endif

    glUseProgram( program );

    checkErrors( "init 1" );

    // Create all our objects
    createShapes();

    // OpenGL state initialization
    glEnable( GL_DEPTH_TEST );
    glEnable( GL_CULL_FACE );
    glCullFace( GL_BACK );
    glClearColor( 0.0, 0.0, 0.0, 0.0 );
    glDepthFunc( GL_LEQUAL );
    glClearDepth( 1.0f );

    checkErrors( "init 2" );

    // initialize all texture-related things
    initTextures();

    checkErrors( "init 3" );

    // register our callbacks
    glfwSetKeyCallback( w_window, keyboard );

    checkErrors( "init 4" );

    return( true );
}

///
// PUBLIC FUNCTIONS
///

///
// Assignment-specific processing
///
void application( int argc, char *argv[] )
{
    (void) argc;
    (void) argv;

    if( !init() ) {
        return;
    }

    checkErrors( "pre-draw" );

    // loop until it's time to quit
    while( !glfwWindowShouldClose(w_window) ) {
        display();
        glfwSwapBuffers( w_window );
        checkErrors( "draw" );
        glfwWaitEvents();
    }
}
