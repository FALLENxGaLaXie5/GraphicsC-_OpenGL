//
// Display a color cube
//
// Colors are assigned to each vertex and then the rasterizer interpolates
//   those colors across the triangles.  We us an orthographic projection
//   as the default projetion.

#include <unistd.h>

#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <iostream>

#include <GLUT/glut.h>

#include "Angel.h"
#include "simplemodels.h"


// Array of rotation angles (in degrees) for each coordinate axis
enum { Xaxis = 0, Yaxis = 1, Zaxis = 2, NumAxes = 3 };
int      Axis = Zaxis;
GLfloat  Theta1[NumAxes] = { 0.0, 0.0, 0.0 };
GLfloat  Theta2[NumAxes] = { 0.0, 0.0, 0.0 };

GLfloat TranslateCube[NumAxes] = {-0.8,0.8,0.0};
GLfloat TranslateCyl[NumAxes] = {0.0,0.0,0.0};
GLfloat TranslateSphere[NumAxes] = {0.8,0.5,0.0};

size_t CUBE_OFFSET;
size_t CYLINDER_OFFSET;
size_t SPHERE_OFFSET;

//----------------------------------------------------------------------------

GLuint program;
GLuint vPosition;
GLuint vColor;

void
init()
{
    // Load shaders and use the resulting shader program
    program = InitShader( "vshader21.glsl", "fshader21.glsl" );
    glUseProgram( program );
    
    //---------------------------------------------------------------------
    colorcube();
    colortube();
    colorsphere();
    //---------------------------------------------------------------------

    
    //----set offset variables
    
    CUBE_OFFSET = 0;
    CYLINDER_OFFSET = sizeof(points_cube) + sizeof(colors);
    //SPHERE_OFFSET = CYLINDER_OFFSET + sizeof(points_cylinder);
    SPHERE_OFFSET = sizeof(points_cube) + sizeof(colors);
    
    
    // Create a vertex array object
    GLuint vao;
    glGenVertexArrays( 1, &vao );  // removed 'APPLE' suffix for 3.2
    glBindVertexArray( vao );
    
    // Create and initialize a buffer object
    GLuint buffer;
    glGenBuffers( 1, &buffer );
    glBindBuffer( GL_ARRAY_BUFFER, buffer );
    
    /**
    glBufferData( GL_ARRAY_BUFFER, sizeof(points_cube) + sizeof(colors) + sizeof(points_cylinder) + sizeof(points_sphere), NULL, GL_STATIC_DRAW );
    glBufferSubData( GL_ARRAY_BUFFER, CUBE_OFFSET, sizeof(points_cube), points_cube );
    glBufferSubData( GL_ARRAY_BUFFER, sizeof(points_cube), sizeof(colors), colors );
    glBufferSubData( GL_ARRAY_BUFFER, CYLINDER_OFFSET, sizeof(points_cylinder), points_cylinder );
    glBufferSubData( GL_ARRAY_BUFFER, SPHERE_OFFSET, sizeof(points_sphere), points_sphere );
     */
    
    
    glBufferData( GL_ARRAY_BUFFER, sizeof(points_cube) + sizeof(colors) + sizeof(points_cylinder) + sizeof(points_sphere), NULL, GL_STATIC_DRAW );
    glBufferSubData( GL_ARRAY_BUFFER, CUBE_OFFSET, sizeof(points_cube), points_cube );
    glBufferSubData( GL_ARRAY_BUFFER, sizeof(points_cube), sizeof(colors), colors );
    glBufferSubData( GL_ARRAY_BUFFER, SPHERE_OFFSET, sizeof(points_sphere), points_sphere );

    //---------------------------------------------------------------------
    
    // set up vertex arrays
    vPosition = glGetAttribLocation( program, "vPosition" );
    glEnableVertexAttribArray( vPosition );
    glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );
    
    vColor = glGetAttribLocation( program, "vColor" );
    glEnableVertexAttribArray( vColor );
    glVertexAttribPointer( vColor, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(points_cube)) );
    //---------------------------------------------------------------------
    
    glEnable( GL_DEPTH_TEST );
    glClearColor( 1.0, 1.0, 1.0, 1.0 );
}

//----------------------------------------------------------------------------




void
display( void )
{
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    
    /**
    //FOR CYLINDER
    mat4  rot1 = ( RotateX( Theta1[Xaxis] ) * RotateY( Theta1[Yaxis] ) * RotateZ( Theta1[Zaxis] ) );
    mat4 transform = Translate( 0.5, 0.0, 0.0 ) * rot1 * Scale(0.25, 1.0, 0.25);
    
    glUniformMatrix4fv( glGetUniformLocation( program, "model" ), 1, GL_TRUE, transform );
    
    glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(CYLINDER_OFFSET) );
    
    glUniform1i( glGetUniformLocation(program, "obj_color_on"), true );
    glUniform4fv( glGetUniformLocation(program, "obj_color"), 1, vec4(0.8, 0.0, 0.0, 1.0) );
    
    glDrawArrays( GL_TRIANGLES, 0, NumVerticesCylinder );
    */
    
    //FOR CUBE
    
    
    mat4  rot2 = ( RotateX( Theta2[Xaxis] ) * RotateY( Theta2[Yaxis] ) * RotateZ( Theta2[Zaxis] ) );
    mat4 transform = Translate( TranslateCube[Xaxis], TranslateCube[Yaxis], TranslateCube[Zaxis] ) * rot2 * Scale(0.25, 1.0, 0.25);
    
    glUniformMatrix4fv( glGetUniformLocation( program, "model" ), 1, GL_TRUE, transform );
    
    glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(CUBE_OFFSET) );
    
    glUniform1i( glGetUniformLocation(program, "obj_color_on"), false );
    
    glDrawArrays( GL_TRIANGLES, 0, NumVerticesCube );
    
    glutSwapBuffers();
    
    
    
    //FOR SPHERE
    mat4  rot3 = ( RotateX( Theta2[Xaxis] ) * RotateY( Theta2[Yaxis] ) * RotateZ( Theta2[Zaxis] ) );
    transform = Translate( TranslateSphere[Xaxis], TranslateSphere[Yaxis], TranslateSphere[Zaxis] ) * rot3 * Scale(0.25, 1.0, 0.25);
    
    glUniformMatrix4fv( glGetUniformLocation( program, "model" ), 1, GL_TRUE, transform );
    
    glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(SPHERE_OFFSET) );
    
    glUniform1i( glGetUniformLocation(program, "obj_color_on"), false );
    
    glDrawArrays( GL_TRIANGLES, 0, NumVerticesSphere );
    
    glutSwapBuffers();
    
}

//----------------------------------------------------------------------------

void
keyboard( unsigned char key, int x, int y )
{
    switch( key ) {
        case 033: // Escape Key
        case 'q': case 'Q':
            exit( EXIT_SUCCESS );
            break;
        case 'x':  Axis = Xaxis;  break;
        case 'y':  Axis = Yaxis;  break;
        case 'z':  Axis = Zaxis;  break;
            
    }
}

//----------------------------------------------------------------------------

void
mouse( int button, int state, int x, int y )
{
    if ( state == GLUT_DOWN ) {
        switch( button ) {
            case GLUT_LEFT_BUTTON:    Axis = Xaxis;  break;
            case GLUT_MIDDLE_BUTTON:  Axis = Yaxis;  break;
            case GLUT_RIGHT_BUTTON:   Axis = Zaxis;  break;
        }
    }
}

//----------------------------------------------------------------------------
int speed1 = 1;
int speed2 = 3;
float posX = -0.8;
float posY = 1.0;
float posZ = 0.0;

void
idle( void )
{
    if (posY >= -0.8 && posX <= -0.8 && posX >= -1.0)
    {
        posY-=0.05;
        TranslateCube[Yaxis] = posY;
    }
    else if (posY < -0.8 && posX < 0.8)
    {
        posX+=0.05;
        TranslateCube[Xaxis] = posX;
    }
    else if (posY < 0.8 && posX >= 0.8)
    {
        posY+=0.01;
        TranslateCube[Yaxis] = posY;
    }
    else if (posY >= 0.8 && posX > -0.8)
    {
        posX-=0.01;
        TranslateCube[Xaxis] = posX;
    }
    Theta1[Axis] = fmod(Theta1[Axis]+speed1, 360.0);
    Theta2[Axis] = fmod(Theta2[Axis]+speed2, 360.0);
    
    Axis = Yaxis;
    Theta1[Axis] = fmod(Theta1[Axis]+speed1, 360.0);
    Theta2[Axis] = fmod(Theta2[Axis]+speed2, 360.0);
    
    Axis = Xaxis;
    Theta1[Axis] = fmod(Theta1[Axis]+speed1, 360.0);
    Theta2[Axis] = fmod(Theta2[Axis]+speed2, 360.0);

    glutPostRedisplay();
}

//----------------------------------------------------------------------------

void
reshape( int w, int h )
{
    glViewport(0,0,w,h);
}

//----------------------------------------------------------------------------

int
main( int argc, char **argv )
{
    glutInit( &argc, argv );
    glutInitDisplayMode(GLUT_3_2_CORE_PROFILE | GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH );
    glutInitWindowSize( 800, 800 );
    glutCreateWindow( "Color Cube" );
    
    init();
    
    glutDisplayFunc( display );
    glutReshapeFunc( reshape );
    glutKeyboardFunc( keyboard );
    glutMouseFunc( mouse );
    glutIdleFunc( idle );
    
    glutMainLoop();
    return 0;
}







