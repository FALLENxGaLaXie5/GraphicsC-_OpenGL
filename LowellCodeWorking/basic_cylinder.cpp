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
using namespace std;
#include <GLUT/glut.h>

#include "Angel.h"
#include "simplemodels.h"

float xRotated = 90.0, yRotated = 0.0, zRotated = 0.0;

//x = (rsin@ cost t)
//z = (rsin@) sin t
//y = r cos@

// Array of rotation angles (in degrees) for each coordinate axis
enum { Xaxis = 0, Yaxis = 1, Zaxis = 2, NumAxes = 3 };
int      Axis = Yaxis;
GLfloat  Theta1[NumAxes] = { 0.0, 0.0, 0.0 };
GLfloat  Theta2[NumAxes] = { 0.0, 0.0, 0.0 };

//for saturn rotation
GLfloat  sat_rot_angle;
GLfloat  sat_new_angle;


size_t CUBE_OFFSET;
size_t CYLINDER_OFFSET;
size_t SPHERE_OFFSET;
size_t TORUS_OFFSET;
bool sphericalMode;


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
    colordonut();
    sphericalMode = false;
    //---------------------------------------------------------------------

    
    //----set offset variables
    
    CUBE_OFFSET = 0;
    CYLINDER_OFFSET = sizeof(points_cube) + sizeof(colors);
    SPHERE_OFFSET = CYLINDER_OFFSET + sizeof(points_cylinder);
    TORUS_OFFSET = SPHERE_OFFSET + sizeof(points_sphere);

    // Create a vertex array object
    GLuint vao;
    glGenVertexArrays( 1, &vao );  // removed 'APPLE' suffix for 3.2
    glBindVertexArray( vao );
    
    // Create and initialize a buffer object
    GLuint buffer;
    glGenBuffers( 1, &buffer );
    glBindBuffer( GL_ARRAY_BUFFER, buffer );
    glBufferData( GL_ARRAY_BUFFER, sizeof(points_cube) + sizeof(colors) + sizeof(points_cylinder) + sizeof(points_sphere) + sizeof(points_torus), NULL, GL_STATIC_DRAW );
    glBufferSubData( GL_ARRAY_BUFFER, CUBE_OFFSET, sizeof(points_cube), points_cube );
    glBufferSubData( GL_ARRAY_BUFFER, sizeof(points_cube), sizeof(colors), colors );
    glBufferSubData( GL_ARRAY_BUFFER, CYLINDER_OFFSET, sizeof(points_cylinder), points_cylinder );
    glBufferSubData( GL_ARRAY_BUFFER, SPHERE_OFFSET , sizeof(points_sphere), points_sphere );
    glBufferSubData( GL_ARRAY_BUFFER, TORUS_OFFSET , sizeof(points_torus), points_torus );
    
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
    glClearColor( 0, 0, 0, 0 );
}

//----------------------------------------------------------------------------

vec4 eye = vec4(0, 0.1, 2.0, 1.0);
vec4 at = vec4(0.0, 0.0, 0.0, 1.0);
vec4 up = vec4(0.0, 1.0, 0.0, 0.0);
mat4 view_matrix;
float r = 0.2;
float td_y =0.0;
float td_z = 0.0;
mat4 proj_matrix;

//----------------------------------------------------------------------------
//O R I G I N A L    D I S P L A Y
//-----------------------------------------------------------------
/*
void
display( void )
{
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glMatrixMode(GL_MODELVIEW);
    
    //-------- intrinsic parameters
    float left = -0.05;
    float right = 0.05;
    float bottom = -0.05;
    float top = 0.05;
    float zNear = 0.1;
    float zFar = 1000.0;
    
    proj_matrix = Frustum( left, right, bottom, top, zNear, zFar);
    
    glUniformMatrix4fv( glGetUniformLocation( program, "projection" ), 1, GL_TRUE, proj_matrix );
    
    
    
    float tr_y = td_y * M_PI/180;
    float tr_z = td_z * M_PI/180;
    eye.z = r * (cos(tr_z)) * cos(tr_y);
    eye.x = r * (cos(tr_z)) * sin(tr_y);
    eye.y = r * sin(tr_z);
    
    
     vec4 n = normalize(eye - at);
     vec4 u = vec4(normalize(cross(up, n)), 0.0);
     vec4 v = vec4(normalize(cross(n, u)), 0.0);
     vec4 t = vec4(0.0,0.0,0.0,1.0);
     mat4 c = mat4(u,v,n,t);
     
     view_matrix = c * Translate(-eye.x, -eye.y, -eye.z);
     
    
    //activates spherical camera
    if(sphericalMode == true)
    {
    view_matrix = LookAt(eye, at, up);
    }
    
    glUniformMatrix4fv( glGetUniformLocation( program, "view" ), 1, GL_TRUE, view_matrix );
    
    
    mat4  rot1 = ( RotateX( Theta1[Xaxis] ) * RotateY( Theta1[Yaxis] ) * RotateZ( Theta1[Zaxis] ) );
    mat4 transform = Translate( 0.5, -0.5, 0.0 ) * rot1 * Scale(0.25, 0.5, 0.25);

    
    glUniformMatrix4fv( glGetUniformLocation( program, "model" ), 1, GL_TRUE, transform );
    
    glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(CYLINDER_OFFSET) );
    
    glUniform1i( glGetUniformLocation(program, "obj_color_on"), true );
    glUniform4fv( glGetUniformLocation(program, "obj_color"), 1, vec4(0.8, 0.0, 0.0, 1.0) );
    
    glDrawArrays( GL_TRIANGLES, 0, NumVerticesCylinder );
    
    
    mat4  rot2 = ( RotateX( Theta2[Xaxis] ) * RotateY( Theta2[Yaxis] ) * RotateZ( Theta2[Zaxis] ) );
    transform = Translate( -0.5, -0.5, 0.0 ) * rot2 * Scale(0.25, 0.5, 0.25);
    
    glUniformMatrix4fv( glGetUniformLocation( program, "model" ), 1, GL_TRUE, transform );
    
    glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(CUBE_OFFSET) );
    
    glUniform1i( glGetUniformLocation(program, "obj_color_on"), false );
    
    glDrawArrays( GL_TRIANGLES, 0, NumVerticesCube );
    
    //----------------------------------------------------------------------
    // for loop for cylinder stack/ sphere
    /*
    mat4  rot3 = ( RotateX( Theta1[Xaxis] ) * RotateY( Theta1[Yaxis] ) * RotateZ( Theta1[Zaxis] ) );
    
    for(int i = 0; i < 5; i++){
    transform = Translate( 0.5, 0.0, 0.0 ) * rot1 * Scale(1.0/i, 1.0, 1.0);
    glUniformMatrix4fv( glGetUniformLocation( program, "model" ), 1, GL_TRUE, transform );
    
    glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(CYLINDER_OFFSET) );
    
    glUniform1i( glGetUniformLocation(program, "obj_color_on"), true );
    glUniform4fv( glGetUniformLocation(program, "obj_color"), 1, vec4(0.8, 0.0, 0.0, 1.0) );
    
    glDrawArrays( GL_TRIANGLES, 0, NumVerticesCylinder );
    }
 
    //for sphere-------------------------------------------------------
    transform = Translate( -0.5, 0.5, 0.0 ) * rot1 * Scale(0.5, 0.5, 0.5);
    glUniformMatrix4fv( glGetUniformLocation( program, "model" ), 1, GL_TRUE, transform );
    
    glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(SPHERE_OFFSET) );
    
    glUniform1i( glGetUniformLocation(program, "obj_color_on"), false);
    glUniform4fv( glGetUniformLocation(program, "obj_color"), 1, vec4(0.8, 0.0, 0.0, 1.0) );
    
    glDrawArrays( GL_TRIANGLES, 0, NumVerticesSphere );
    
    //draw donut-------------------------------------------------------------
    mat4 rot4 =( RotateX( Theta2[Xaxis] ) * RotateY( Theta2[Yaxis] ) * RotateZ( Theta2[Zaxis] ) );
    transform = Translate( 0.5, 0.5, 0.0 ) * rot4 * Scale(0.5, 0.5, 0.5);
    glUniformMatrix4fv( glGetUniformLocation( program, "model" ), 1, GL_TRUE, transform );
    
    glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(TORUS_OFFSET) );
    
    glUniform1i( glGetUniformLocation(program, "obj_color_on"), false);
    glUniform4fv( glGetUniformLocation(program, "obj_color"), 1, vec4(0.8, 0.0, 0.0, 1.0) );
    
    glDrawArrays( GL_TRIANGLES, 0, NumVerticesTorus );
    
    
    
    glutSwapBuffers();
    glutSolidSphere(1.0, 50, 50);
}

*/
//----------------------------------------------------------------------------
//-----END OF ORIGINAL DISPLAY
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
//N E W     D I S P L A Y
//----------------------------------------------------------------------------

void
display( void )
{
    
    //-------- intrinsic parameters
    float left = -0.05;
    float right = 0.05;
    float bottom = -0.05;
    float top = 0.05;
    float zNear = 0.1;
    float zFar = 1000.0;
    
    proj_matrix = Frustum( left, right, bottom, top, zNear, zFar);
    
    glUniformMatrix4fv( glGetUniformLocation( program, "projection" ), 1, GL_TRUE, proj_matrix );
    
    
    
    float tr_y = td_y * M_PI/180;
    float tr_z = td_z * M_PI/180;
    eye.z = r * (cos(tr_z)) * cos(tr_y);
    eye.x = r * (cos(tr_z)) * sin(tr_y);
    eye.y = r * sin(tr_z);
    
    
    //activates spherical camera
    if(sphericalMode == true)
    {
        view_matrix = LookAt(eye, at, up);
    }
    
    glUniformMatrix4fv( glGetUniformLocation( program, "view" ), 1, GL_TRUE, view_matrix );
    
    
    // FIRST SPHERE
    mat4  rot1 = ( RotateX( Theta1[Xaxis] ) * RotateY( Theta1[Yaxis] ) * RotateZ( Theta1[Zaxis] ) );
    mat4 transform = Translate( 0.5, -0.5, 0.0 ) * rot1 * Scale(0.25, 0.5, 0.25);
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glMatrixMode(GL_MODELVIEW);
    
    transform = Translate( 0.0, 0.0, 0.0 ) * rot1 * Scale(0.5, 0.5, 0.5);
    glUniformMatrix4fv( glGetUniformLocation( program, "model" ), 1, GL_TRUE, transform );
    
    glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(SPHERE_OFFSET) );
    
    glUniform1i( glGetUniformLocation(program, "obj_color_on"), false);
    glUniform4fv( glGetUniformLocation(program, "obj_color"), 1, vec4(0.8, 0.0, 0.0, 1.0) );
    
    glDrawArrays( GL_TRIANGLES, 0, NumVerticesSphere );
    
    //SECOND SPHERE
    
    mat4 rev_saturn;
    transform = RotateY(sat_new_angle) * Translate( 0.8, 0.0, 0.0 ) * RotateY(sat_rot_angle) * Scale(0.2, 0.2, 0.2);
    glUniformMatrix4fv( glGetUniformLocation( program, "model" ), 1, GL_TRUE, transform );
    
    glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(SPHERE_OFFSET) );
    
    glUniform1i( glGetUniformLocation(program, "obj_color_on"), false);
    glUniform4fv( glGetUniformLocation(program, "obj_color"), 1, vec4(0.8, 0.0, 0.0, 1.0) );
    
    glDrawArrays( GL_TRIANGLES, 0, NumVerticesSphere );
    
    //CREATE RING OF SATURN
    mat4 rot4 =( RotateX( Theta2[Xaxis] ) * RotateY( Theta2[Yaxis] ) * RotateZ( Theta2[Zaxis] ) );
    //shoot through the hoop transform
    //transform = Translate( 0.5, 0, 0.0 ) * RotateY(sat_rot_angle) * RotateY(sat_rot_angle) * Scale(0.5, 0.5, 0.05);
    transform = RotateY(sat_new_angle) * Translate(0.8, 0, 0) * RotateY(sat_rot_angle) * RotateX(90.0) * Scale(0.4, 0.4, 0.05);
    glUniformMatrix4fv( glGetUniformLocation( program, "model" ), 1, GL_TRUE, transform );
    
    glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(TORUS_OFFSET) );
    
    glUniform1i( glGetUniformLocation(program, "obj_color_on"), false);
    glUniform4fv( glGetUniformLocation(program, "obj_color"), 1, vec4(0.8, 0.0, 0.0, 1.0) );
    
    glDrawArrays( GL_TRIANGLES, 0, NumVerticesTorus );
    
    glutSwapBuffers();
    glutSolidSphere(1.0, 50, 50);
}

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
        //default location
        case 'e':
            eye = vec4(0.0, 0.0, 2.0, 1.0);
            at = vec4(0.0, 0.0, 0.0, 1.0);
            up = vec4(0.0, 1.0, 0.0, 0.0);
            view_matrix = LookAt(eye, at, up);
            glutPostRedisplay();
            break;
        //activates spherical mode
        case 'o':
            sphericalMode = !sphericalMode;
        //new movement
        case 'w':
            if(sphericalMode == true){
            td_z += 5.0;
            td_z = fmod(td_z, 360);
            }
            else
            {
            view_matrix = Translate(0, 0.0, 0.2) * view_matrix;
            }
            glutPostRedisplay();
            break;
        case 's':
            if(sphericalMode == true){
            td_z -= 5.0;
            td_z = fmod(td_z, 360);
            }
            else{
            view_matrix = Translate(0, 0.0, -0.2) * view_matrix;
            }
            glutPostRedisplay();
            break;
        case 'a':
            if(sphericalMode == true){
            td_y -= 5.0;
            td_y = fmod(td_y, 360);
            }
            else
            {
            view_matrix = Translate(0.2, 0.0, 0.0) * view_matrix;
            }
            glutPostRedisplay();
            break;
        case 'd':
            if(sphericalMode == true){
            td_y += 5.0;
            td_y = fmod(td_y, 360);
            }
            else{
            view_matrix = Translate(-0.2, 0.0, 0.0) * view_matrix;
            }
            glutPostRedisplay();
            break;
            case 'f':
                view_matrix = Translate(0.0, -0.2, 0.0) * view_matrix;
                glutPostRedisplay();
                break;
            case 'c':
            view_matrix = Translate(0.0, 0.2, 0.0) * view_matrix;
            glutPostRedisplay();
            break;
            
            
        //----capital letters does super man camera
            /*
        case 'W':
            view_matrix = Translate(0, 0.0, 0.2) * view_matrix;
            glutPostRedisplay();
            break;
        case 'S':
            view_matrix = Translate(0, 0.0, -0.2) * view_matrix;
            glutPostRedisplay();
            break;
        case 'A':
            view_matrix = Translate(0.2, 0.0, 0.0) * view_matrix;
            glutPostRedisplay();
            break;
        case 'D':
            view_matrix = Translate(-0.2, 0.0, 0.0) * view_matrix;
            glutPostRedisplay();
            break;
        case 'F':
            view_matrix = Translate(0.0, -0.2, 0.0) * view_matrix;
            glutPostRedisplay();
            break;
        case 'C':
            view_matrix = Translate(0.0, 0.2, 0.0) * view_matrix;
            glutPostRedisplay();
            break;
            
            */
        
            
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

void
idle( void )
{/*
    Theta1[Axis] = fmod(Theta1[Axis]+1, 360.0);
    Theta2[Axis] = fmod(Theta2[Axis]+1, 360.0);
  
    Axis = Zaxis;
    
    Theta1[Axis] = fmod(Theta1[Axis]+1, 360.0);
    Theta2[Axis] = fmod(Theta2[Axis]+1, 360.0);
    
    Axis = Xaxis;
    
    Theta1[Axis] = fmod(Theta1[Axis]+1, 360.0);
    Theta2[Axis] = fmod(Theta2[Axis]+1, 360.0);
  */
    //zRotated += 0.3;
    sat_rot_angle = fmod(sat_rot_angle + 1.0, 360);
    sat_new_angle = fmod(sat_new_angle + 1.0, 360);
    glutPostRedisplay();
 
  
}

//----------------------------------------------------------------------------

void
reshape( int w, int h )
{
    glMatrixMode (GL_PROJECTION);
    glLoadIdentity();
    
   // gluPerspective (40.0, (GLdouble)w / (GLdouble)h, 0.5, 20.0);
   // glMatrixMode   (GL_MODELVIEW);
    glViewport(0, 0, w, h);
}



//----------------------------------------------------------------------------

int
main( int argc, char **argv )
{
    glutInit( &argc, argv );
    glutInitDisplayMode(GLUT_3_2_CORE_PROFILE | GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH );
   // glutInitDisplayMode    (GLUT_DEPTH | GLUT_DOUBLE);
    glutInitWindowSize( 800, 800 );
    glutCreateWindow( "Color Cube" );
    glPolygonMode          (GL_FRONT_AND_BACK, GL_LINE);
    init();
    
    glutDisplayFunc( display ); //draw_sphere
    glutReshapeFunc( reshape );
    glutKeyboardFunc( keyboard );
    glutMouseFunc( mouse );
    glutIdleFunc( idle );
    
    glutMainLoop();
    return 0;
}



