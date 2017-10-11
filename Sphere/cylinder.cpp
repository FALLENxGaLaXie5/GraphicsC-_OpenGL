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


// Array of rotation angles (in degrees) for each coordinate axis
enum { Xaxis = 0, Yaxis = 1, Zaxis = 2, NumAxes = 3 };
int      Axis = Zaxis;
int viewAxis = Xaxis;

GLfloat  Theta1[NumAxes] = { 0.0, 0.0, 0.0 };
GLfloat  Theta2[NumAxes] = { 0.0, 0.0, 0.0 };
GLfloat  ThetaSun[NumAxes] = { 0.0, 0.0, 0.0 };
GLfloat  ThetaSaturn[NumAxes] = { 0.0, 0.0, 0.0 };

GLfloat ThetaTorus[NumAxes] = { 0.0, 0.0, 0.0 };

GLfloat TranslateCube[NumAxes] = {-0.8,0.8,0.0};
GLfloat TranslateCyl[NumAxes] = {0.0,0.0,0.0};
GLfloat TranslateSun[NumAxes] = {0.0,0.0,0.0};
GLfloat TranslateSaturn[NumAxes] = {0.8,0.0,0.0};

GLfloat TranslateTorus[NumAxes] = {0.0,0.0,0.0};

float satRotAngle = 0.0;
float satRevAngle = 0.0;

bool sphericalMode = true;

//all shapes offsets
/**
size_t COLOR_OFFSET;
size_t CUBE_OFFSET;
size_t CYLINDER_OFFSET;
size_t SPHERE_OFFSET;
size_t TORUS_OFFSET;
*/

size_t COLOR_OFFSET;
size_t SPHERE_OFFSET;
size_t SPHERE2_OFFSET;
size_t RINGS_OFFSET;


//----------------------------------------------------------------------------

GLuint program;
GLuint vPosition;
GLuint vNormal;
GLuint vColor;

void
init()
{
    // Load shaders and use the resulting shader program
    program = InitShader( "vshader21.glsl", "fshader21.glsl" );
    glUseProgram( program );
    
    //---------------------------------------------------------------------
    //creating all shapes code
    /**
    colorcube();
    colortube();
    colorsphere();
    colorTorus();
     */
    
    colorsphere();
    colorsphere();
    colorTorus();
    
    //---------------------------------------------------------------------

    /**
    CUBE_OFFSET = 0;
    COLOR_OFFSET = sizeof(points_cube);
    CYLINDER_OFFSET = COLOR_OFFSET + sizeof(colors);
    SPHERE_OFFSET = CYLINDER_OFFSET + sizeof(points_cylinder);
    TORUS_OFFSET = SPHERE_OFFSET + sizeof(points_sphere);
    
     */
    
    //creating sun and saturn
    SPHERE_OFFSET = 0;
    COLOR_OFFSET = sizeof(points_sphere);
    SPHERE2_OFFSET = COLOR_OFFSET + sizeof(colors);
    RINGS_OFFSET = SPHERE2_OFFSET + sizeof(points_sphere);
    // Create a vertex array object
    GLuint vao;
    glGenVertexArrays( 1, &vao );  // removed 'APPLE' suffix for 3.2
    glBindVertexArray( vao );
    
    // Create and initialize a buffer object
    //---------------------------------------------------------------------
    
    /**
    GLuint buffer;
    glGenBuffers( 1, &buffer );
    glBindBuffer( GL_ARRAY_BUFFER, buffer );
    glBufferData( GL_ARRAY_BUFFER, sizeof(points_cube) + sizeof(colors) + sizeof(points_cylinder) + sizeof(points_sphere) + sizeof(points_torus), NULL, GL_STATIC_DRAW );
    glBufferSubData( GL_ARRAY_BUFFER, CUBE_OFFSET, sizeof(points_cube), points_cube );
    glBufferSubData( GL_ARRAY_BUFFER, COLOR_OFFSET, sizeof(colors), colors );
    glBufferSubData( GL_ARRAY_BUFFER, CYLINDER_OFFSET, sizeof(points_cylinder), points_cylinder );
    glBufferSubData( GL_ARRAY_BUFFER, SPHERE_OFFSET, sizeof(points_sphere), points_sphere );
    glBufferSubData( GL_ARRAY_BUFFER, TORUS_OFFSET, sizeof(points_torus), points_torus );
    */
    
    
    GLuint buffer;
    glGenBuffers( 1, &buffer );
    glBindBuffer( GL_ARRAY_BUFFER, buffer );
    glBufferData( GL_ARRAY_BUFFER, sizeof(points_sphere) + sizeof(colors) + sizeof(points_sphere) + sizeof(points_torus), NULL, GL_STATIC_DRAW );
    glBufferSubData( GL_ARRAY_BUFFER, SPHERE_OFFSET, sizeof(points_sphere), points_sphere );
    glBufferSubData( GL_ARRAY_BUFFER, COLOR_OFFSET, sizeof(colors), colors );
    glBufferSubData( GL_ARRAY_BUFFER, SPHERE2_OFFSET, sizeof(points_sphere), points_sphere );
    glBufferSubData( GL_ARRAY_BUFFER, RINGS_OFFSET, sizeof(points_torus), points_torus );
    
    // set up vertex arrays
    
    vPosition = glGetAttribLocation( program, "vPosition" );
    glEnableVertexAttribArray( vPosition );
    glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );
    
    vColor = glGetAttribLocation( program, "vColor" );
    glEnableVertexAttribArray( vColor );
    glVertexAttribPointer( vColor, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(points_cube)) );
     
    //---------------------------------------------------------------------
    /**
    // set up vertex arrays
    vPosition = glGetAttribLocation( program, "vPosition" );
    glEnableVertexAttribArray( vPosition );
    glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(CUBE_OFFSET) );
    
    vNormal = glGetAttribLocation( program, "vNormal" );
    glEnableVertexAttribArray( vNormal );
    glVertexAttribPointer( vNormal, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(CUBE_NORMALS_OFFSET) );
    
    vColor = glGetAttribLocation( program, "vColor" );
    glEnableVertexAttribArray( vColor );
    glVertexAttribPointer( vColor, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(COLOR_OFFSET) );
    //---------------------------------------------------------------------
    */
    
    glEnable( GL_DEPTH_TEST );
    glClearColor( 0.0, 0.0, 0.0, 0.0 );
    
    
    
    
    //----set offset variables
    /**
    CUBE_OFFSET = 0;
    CYLINDER_OFFSET = sizeof(points_cube) + sizeof(colors);
    SPHERE_OFFSET = CYLINDER_OFFSET + sizeof(points_cylinder);
    //SPHERE_OFFSET = sizeof(points_cube) + sizeof(colors);
    
    
    // Create a vertex array object
    GLuint vao;
    glGenVertexArrays( 1, &vao );  // removed 'APPLE' suffix for 3.2
    glBindVertexArray( vao );
    
    // Create and initialize a buffer object
    GLuint buffer;
    glGenBuffers( 1, &buffer );
    glBindBuffer( GL_ARRAY_BUFFER, buffer );
    
    
    
    
    glBufferData( GL_ARRAY_BUFFER, sizeof(points_cube) + sizeof(colors) + sizeof(points_cylinder) + sizeof(points_sphere), NULL, GL_STATIC_DRAW );
    glBufferSubData( GL_ARRAY_BUFFER, CUBE_OFFSET, sizeof(points_cube), points_cube );
    glBufferSubData( GL_ARRAY_BUFFER, CYLINDER_OFFSET, sizeof(points_cylinder), points_cylinder );
   // glBufferSubData( GL_ARRAY_BUFFER, sizeof(points_cube), sizeof(colors), colors );
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
    */
}

//----------------------------------------------------------------------------


float sry = 0.0;



vec4 eye = vec4(0.0, 0.0, -0.25, 1.0);
vec4 at = vec4(0.0, 0.0, 0.0, 1.0);
vec4 up = vec4(0.0, 1.0, 0.0, 0.0);
mat4 view_matrix;


float r = 0.3;
float td_y = 0.0;
float td_z = 0.0;

void
display( void )
{
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    
    
    float tr_y = td_y * M_PI/180.0;
    float tr_z = td_z * M_PI/180.0;
    eye.z = r * (cos(tr_z)) * cos(tr_y);
    eye.x = r * (cos(tr_z)) * sin(tr_y);
    eye.y = r * sin(tr_z);
    up = vec4(0.0, cos(tr_z), 0.0, 0.0);
    
    
    /**
    vec4 n = normalize(eye - at);
    vec4 u = vec4(normalize(cross(up, n)), 0.0);
    vec4 v = vec4(normalize(cross(n, u)), 0.0);
    vec4 t = vec4(0.0,0.0,0.0,1.0);
    mat4 c = mat4(u,v,n,t);
     
    view_matrix = c * Translate(-eye.x, -eye.y, -eye.z);
    */
    //at = {eye[0], eye[1], static_cast<GLfloat>(eye[2] + 0.5), 1.0};
    
    if (sphericalMode == true)
    {
        view_matrix = LookAt(eye, at, up);
    }
    
    glUniformMatrix4fv( glGetUniformLocation( program, "view" ), 1, GL_TRUE, view_matrix );

    
    
    //FOR CYLINDER
    /**
    mat4  rot1 = ( RotateX( Theta1[Xaxis] ) * RotateY( Theta1[Yaxis] ) * RotateZ( Theta1[Zaxis] ) );
    mat4 transform = Translate( 0.5, 0.0, 0.0 ) * rot1 * Scale(0.25, 1.0, 0.25);
    
    glUniformMatrix4fv( glGetUniformLocation( program, "model" ), 1, GL_TRUE, transform );
    
    glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(CYLINDER_OFFSET) );
    
    glUniform1i( glGetUniformLocation(program, "obj_color_on"), true );
    glUniform4fv( glGetUniformLocation(program, "obj_color"), 1, vec4(0.8, 0.0, 0.0, 1.0) );
    
    glDrawArrays( GL_TRIANGLES, 0, NumVerticesCylinder );
    */
    

    
    //FOR CUBE
    
    /*
    mat4  rot2 = ( RotateX( Theta2[Xaxis] ) * RotateY( Theta2[Yaxis] ) * RotateZ( Theta2[Zaxis] ) );
    transform = Translate( TranslateCube[Xaxis], TranslateCube[Yaxis], TranslateCube[Zaxis] ) * rot2 * Scale(0.25, 1.0, 0.25);
    
    glUniformMatrix4fv( glGetUniformLocation( program, "model" ), 1, GL_TRUE, transform );
    
    glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(CUBE_OFFSET) );
    
    glUniform1i( glGetUniformLocation(program, "obj_color_on"), false );
    
    glDrawArrays( GL_TRIANGLES, 0, NumVerticesCube );
    
    //glutSwapBuffers();
    */
    
    
    //FOR SATURN
    
    //mat4  rot3 = RotateY(sry);
    
    
    //mat4 rot3 = ( RotateX( ThetaSaturn[Xaxis] ) * RotateY( ThetaSaturn[Yaxis] ) * RotateZ( ThetaSaturn[Zaxis] ) );
    //mat4 transform = Translate( TranslateSaturn[Xaxis], TranslateSaturn[Yaxis], TranslateSaturn[Zaxis] ) * rot3 * Scale(0.4, 0.4, 0.4);
    
    mat4 transform = RotateY(satRevAngle) * Translate(0.9, 0.0, 0.0) * RotateY(satRotAngle) * Scale(0.1, 0.1, 0.1);
    
    glUniformMatrix4fv( glGetUniformLocation( program, "model" ), 1, GL_TRUE, transform );
    
    glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(SPHERE_OFFSET) );
    
    glUniform1i( glGetUniformLocation(program, "obj_color_on"), false );
    
    glDrawArrays( GL_TRIANGLES, 0, NumVerticesSphere );
    
    
    
    //FOR SATURN'S RINGS
    transform = RotateY(satRevAngle) * Translate(0.9, 0.0, 0.0) * RotateY(satRotAngle) * RotateX(90.0) * Scale(0.5, 0.5, 0.1);
    
    glUniformMatrix4fv( glGetUniformLocation( program, "model" ), 1, GL_TRUE, transform );
    
    glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(RINGS_OFFSET) );
    
    glUniform1i( glGetUniformLocation(program, "obj_color_on"), true );
    glUniform4fv(glGetUniformLocation(program, "obj_color"), 1, vec4(0.8, 0.0, 0.0, 1.0));
    
    glDrawArrays( GL_TRIANGLES, 0, NumVerticesTorus );
    
    
    
    
    
    
    //FOR SUN
    mat4 rot5 = ( RotateX( ThetaSun[Xaxis] ) * RotateY( ThetaSun[Yaxis] ) * RotateZ( ThetaSun[Zaxis] ) );
    transform = Translate( TranslateSun[Xaxis], TranslateSun[Yaxis], TranslateSun[Zaxis] ) * rot5 * Scale(0.5, 0.5, 0.5);
    glUniformMatrix4fv( glGetUniformLocation( program, "model" ), 1, GL_TRUE, transform );
    
    glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(SPHERE_OFFSET) );
    
    glUniform1i( glGetUniformLocation(program, "obj_color_on"), false );
    
    glDrawArrays( GL_TRIANGLES, 0, NumVerticesSphere );
    
    
    //FOR TORUS
    /**
    mat4 rot4 = ( RotateX( ThetaTorus[Xaxis] ) * RotateY( ThetaTorus[Yaxis] ) * RotateZ( ThetaTorus[Zaxis] ) );
    transform = Translate( TranslateTorus[Xaxis], TranslateTorus[Yaxis], TranslateTorus[Zaxis] ) * rot4 * Scale(0.8, 0.8, 0.8);
    glUniformMatrix4fv( glGetUniformLocation( program, "model" ), 1, GL_TRUE, transform );
    
    glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(TORUS_OFFSET) );
    
    glUniform1i( glGetUniformLocation(program, "obj_color_on"), false );
    
    glDrawArrays( GL_TRIANGLES, 0, NumVerticesTorus );
    
    */
    
    
    glutSwapBuffers();
    
    
}


/**
float r = 0.5;
int p = 0;
int n = 0;
GLfloat pp = M_PI * (float)p / (float)sphericalLevels;
GLfloat tt = 2 * M_PI * (float)n / (float)sphericalLevels;
*/





//----------------------------------------------------------------------------
float cameraSpeed = 0.03;
void
keyboard( unsigned char key, int x, int y )
{
    switch( key )
    {
        case 033: // Escape Key
        case 'q': case 'Q':
            exit( EXIT_SUCCESS );
            break;
        case 'x':  Axis = Xaxis;
            //viewAxis = Xaxis;
            break;
        case 'y':  Axis = Yaxis;
            //viewAxis = Yaxis;
            break;
        case 'z':  Axis = Zaxis;
            //viewAxis = Zaxis;
            break;
            //o will activate or deactivate spherical mode
        case 'o':
            sphericalMode = !sphericalMode;
            break;
            //e will be the default location
        case 'e':
            eye = vec4(0.0, 0.0, 2.0, 1.0);
            at = vec4(0.0, 0.0, 0.0, 1.0);
            up = vec4(0.0, 1.0, 0.0, 0.0);
            view_matrix = LookAt(eye, at, up);
            glutPostRedisplay();
            break;
        case 'a':
            if (sphericalMode)
            {
                td_y -= 5.0;
                td_y = fmod(td_y, 360.0);
            }
            else
            {
                view_matrix = Translate(0.2, 0.0, 0.0) * view_matrix;
            }
            glutPostRedisplay();
            break;
        case 'd':
            if (sphericalMode)
            {
                td_y += 5.0;
                td_y = fmod(td_y, 360.0);
            }
            else
            {
                view_matrix = Translate(-0.2, 0.0, 0.0) * view_matrix;
            }
            glutPostRedisplay();

            break;
        case 'w':
            //eye[Yaxis]-=cameraSpeed;
            if (sphericalMode)
            {
                td_z += 5.0;
                td_z = fmod(td_z, 360.0);
            }
            else
            {
                view_matrix = Translate(0, 0.0, 0.2) * view_matrix;
            }

            glutPostRedisplay();

            break;
        case 's':
            if (sphericalMode)
            {
                td_y -= 5.0;
                td_z = fmod(td_z, 360.0);
                //eye[Yaxis]+=cameraSpeed;
            }
            else
            {
                view_matrix = Translate(0, 0.0, -0.2) * view_matrix;
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
int speed2 = 1;
int speed3 = 4;

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
    //ThetaSaturn[Axis] = fmod(ThetaSaturn[Axis]+speed2, 360.0);
    ThetaTorus[Axis] = fmod(ThetaTorus[Axis]+speed2, 360.0);

    
    Axis = Yaxis;
    Theta1[Axis] = fmod(Theta1[Axis]+speed1, 360.0);
    Theta2[Axis] = fmod(Theta2[Axis]+speed2, 360.0);
    //ThetaSaturn[Axis] = fmod(ThetaSaturn[Axis]+speed3, 360.0);
    ThetaTorus[Axis] = fmod(ThetaTorus[Axis]+speed2, 360.0);

    satRotAngle = fmod(satRotAngle + 0.5, 360.0);
    satRevAngle = fmod(satRevAngle + 0.5, 360.0);
    
    
    Axis = Xaxis;
    Theta1[Axis] = fmod(Theta1[Axis]+speed1, 360.0);
    Theta2[Axis] = fmod(Theta2[Axis]+speed2, 360.0);
    //ThetaSaturn[Axis] = fmod(ThetaSaturn[Axis]+speed1, 360.0);
    ThetaTorus[Axis] = fmod(ThetaTorus[Axis]+speed2, 360.0);

    
    //ry = fmod( sry + 0.2, 360.0);
    //Axis = Yaxis;
    //ThetaSphere[Axis] = fmod(<#double#>, <#double#>)
    
    //move camera
    
    /**
    if (eye[0]<0.8)
    {
        eye[0]+=0.02;
    }
    else if (eye[0]>0.8)
    {
        eye[0]-=0.2;
    }
    */
    
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







