// spherical camera + persp + lighting
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
GLfloat  Theta1[NumAxes] = { 80.0, 0.0, 0.0 };
GLfloat  Theta2[NumAxes] = { 0.0, 30.0, 0.0 };
GLfloat  Theta3[NumAxes] = { 0.0, 0.0, 0.0 };

//----------------------------------------------------------------------------

size_t CUBE_OFFSET;
size_t CUBE_NORMALS_OFFSET;

size_t COLOR_OFFSET;

size_t GROUND_OFFSET;
size_t GROUND_NORMALS_OFFSET;

size_t LIGHT_POLE1_OFFSET;
size_t LIGHT_POLE1NORMALS_OFFSET;
size_t SPHERE_LIGHT1_OFFSET;
size_t SPHERE_LIGHT1_NORMALS_OFFSET;
size_t LIGHT1_BASE_OFFSET;
size_t LIGHT1_BASENORMALS_OFFSET;

size_t LIGHT_POLE2_OFFSET;
size_t LIGHT_POLE2NORMALS_OFFSET;
size_t SPHERE_LIGHT2_OFFSET;
size_t SPHERE_LIGHT2_NORMALS_OFFSET;
size_t LIGHT2_BASE_OFFSET;
size_t LIGHT2_BASENORMALS_OFFSET;

size_t LIGHT1_OFFSET;
size_t LIGHT1_NORMAL;
size_t LIGHT2_OFFSET;
size_t LIGHT2_NORMAL;


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
    colorcube();
    colortube();
    colorbube();
    
    //---------------------------------------------------------------------
    
    
    //----set offset variables
    
    CUBE_OFFSET = 0;
    COLOR_OFFSET = sizeof(points_cube);
    CUBE_NORMALS_OFFSET = COLOR_OFFSET + sizeof(colors);
    
    GROUND_OFFSET = CUBE_NORMALS_OFFSET + sizeof(normals_cube);
    GROUND_NORMALS_OFFSET = GROUND_OFFSET + sizeof(points_cube);
    
    LIGHT_POLE1_OFFSET = GROUND_NORMALS_OFFSET + sizeof(normals_cube);
    LIGHT_POLE1NORMALS_OFFSET = LIGHT_POLE1_OFFSET + sizeof(points_cylinder);
    SPHERE_LIGHT1_OFFSET = LIGHT_POLE1NORMALS_OFFSET + sizeof(normals_cylinder);
    SPHERE_LIGHT1_NORMALS_OFFSET = SPHERE_LIGHT1_OFFSET + sizeof(points_sphere);
    LIGHT1_BASE_OFFSET = SPHERE_LIGHT1_NORMALS_OFFSET + sizeof(normals_sphere);
    LIGHT1_BASENORMALS_OFFSET = LIGHT1_BASE_OFFSET + sizeof(points_cylinder);
    
    LIGHT_POLE2_OFFSET = LIGHT1_BASENORMALS_OFFSET + sizeof(normals_cylinder);
    LIGHT_POLE2NORMALS_OFFSET = LIGHT_POLE2_OFFSET + sizeof(points_cylinder);
    SPHERE_LIGHT2_OFFSET = LIGHT_POLE2NORMALS_OFFSET + sizeof(normals_cylinder);
    SPHERE_LIGHT2_NORMALS_OFFSET = SPHERE_LIGHT2_OFFSET + sizeof(points_sphere);
    LIGHT2_BASE_OFFSET = SPHERE_LIGHT2_NORMALS_OFFSET + sizeof(normals_sphere);
    LIGHT2_BASENORMALS_OFFSET = LIGHT2_BASE_OFFSET + sizeof(points_cylinder);

    LIGHT1_OFFSET = LIGHT2_BASENORMALS_OFFSET + sizeof(normals_cylinder);
    LIGHT1_NORMAL = LIGHT1_OFFSET + sizeof(points_sphere);
    LIGHT2_OFFSET = LIGHT1_NORMAL + sizeof(normals_sphere);
    LIGHT2_NORMAL = LIGHT2_OFFSET + sizeof(points_sphere);
    
    
    // Create a vertex array object
    GLuint vao;
    glGenVertexArrays( 1, &vao );  // removed 'APPLE' suffix for 3.2
    glBindVertexArray( vao );
    
    //cube, cube, cylinder, cylinder, cylinder, cylinder, sphere, sphere
    
    // Create and initialize a buffer object
    GLuint buffer;
    glGenBuffers( 1, &buffer );
    glBindBuffer( GL_ARRAY_BUFFER, buffer );
    glBufferData( GL_ARRAY_BUFFER, sizeof(points_cube) + sizeof(normals_cube) + sizeof(points_cube) + sizeof(normals_cube) + sizeof(colors) + sizeof(points_cylinder) + sizeof(normals_cylinder) + sizeof(points_cylinder) + sizeof(normals_cylinder) + sizeof(points_cylinder) + sizeof(normals_cylinder) + sizeof(points_cylinder) + sizeof(normals_cylinder) + sizeof(points_sphere) + sizeof(normals_sphere) + sizeof(points_sphere) + sizeof(normals_sphere) + sizeof(points_sphere) + sizeof(normals_sphere) + sizeof(points_sphere) + sizeof(normals_sphere),NULL, GL_STATIC_DRAW );
    
    glBufferSubData( GL_ARRAY_BUFFER, CUBE_OFFSET, sizeof(points_cube), points_cube );
    glBufferSubData( GL_ARRAY_BUFFER, COLOR_OFFSET, sizeof(colors), colors );
    glBufferSubData( GL_ARRAY_BUFFER, CUBE_NORMALS_OFFSET, sizeof(normals_cube), normals_cube );
    glBufferSubData( GL_ARRAY_BUFFER, GROUND_OFFSET, sizeof(points_cube), points_cube );
    glBufferSubData( GL_ARRAY_BUFFER, GROUND_NORMALS_OFFSET, sizeof(normals_cube), normals_cube );
    
    glBufferSubData( GL_ARRAY_BUFFER, LIGHT_POLE1_OFFSET, sizeof(points_cylinder), points_cylinder );
    glBufferSubData( GL_ARRAY_BUFFER, LIGHT_POLE1NORMALS_OFFSET, sizeof(normals_cylinder), normals_cylinder );
    glBufferSubData( GL_ARRAY_BUFFER, SPHERE_LIGHT1_OFFSET, sizeof(points_sphere), points_sphere );
    glBufferSubData( GL_ARRAY_BUFFER, SPHERE_LIGHT1_NORMALS_OFFSET, sizeof(normals_sphere), normals_sphere );
    glBufferSubData( GL_ARRAY_BUFFER, LIGHT1_BASE_OFFSET, sizeof(points_cylinder), points_cylinder );
    glBufferSubData( GL_ARRAY_BUFFER, LIGHT1_BASENORMALS_OFFSET, sizeof(normals_cylinder), normals_cylinder );

    glBufferSubData( GL_ARRAY_BUFFER, LIGHT_POLE2_OFFSET, sizeof(points_cylinder), points_cylinder );
    glBufferSubData( GL_ARRAY_BUFFER, LIGHT_POLE2NORMALS_OFFSET, sizeof(normals_cylinder), normals_cylinder );
    glBufferSubData( GL_ARRAY_BUFFER, SPHERE_LIGHT2_OFFSET, sizeof(points_sphere), points_sphere );
    glBufferSubData( GL_ARRAY_BUFFER, SPHERE_LIGHT2_NORMALS_OFFSET, sizeof(normals_sphere), normals_sphere );
    glBufferSubData( GL_ARRAY_BUFFER, LIGHT2_BASE_OFFSET, sizeof(points_cylinder), points_cylinder );
    glBufferSubData( GL_ARRAY_BUFFER, LIGHT2_BASENORMALS_OFFSET, sizeof(normals_cylinder), normals_cylinder );
    
    glBufferSubData( GL_ARRAY_BUFFER, LIGHT1_OFFSET, sizeof(points_sphere), points_sphere );
    glBufferSubData( GL_ARRAY_BUFFER, LIGHT1_NORMAL, sizeof(normals_sphere), normals_sphere );
    glBufferSubData( GL_ARRAY_BUFFER, LIGHT2_OFFSET, sizeof(points_sphere), points_sphere );
    glBufferSubData( GL_ARRAY_BUFFER, LIGHT2_NORMAL, sizeof(normals_sphere), normals_sphere );
    
    
    
    //---------------------------------------------------------------------
    
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
    
    glEnable( GL_DEPTH_TEST );
    glClearColor( 0.0, 0.0, 0.0, 0.0 );
}


//----------------------------------------------------------------------------

mat4 proj_matrix;
mat4 view_matrix;

float r = 2.9;


float poleRadius = 0.3;


void
display( void )
{
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    
    //---- lights
    
    vec4 lpos = vec4( -15.0, 4.5, 0.0, 1.0 );
    glUniform4fv( glGetUniformLocation(program, "LightPosition"), 1, lpos);
    
    vec4 la(0.2, 0.2, 0.2, 1.0);
    vec4 ld(0.7, 0.7, 0.7, 1.0);
    vec4 ls(0.4, 0.4, 0.4, 1.0);
    glUniform4fv( glGetUniformLocation(program, "AmbientLight"), 1, la);
    glUniform4fv( glGetUniformLocation(program, "DiffuseLight"), 1, ld);
    glUniform4fv( glGetUniformLocation(program, "SpecularLight"), 1, ls);
    
    
    //---- camera intrinsic parameters
    
    float left = -0.5;
    float right = 0.5;
    float bottom = -0.5;
    float top = 0.5;
    float zNear = 0.5;
    float zFar = 100.0;
    
    proj_matrix = Frustum( left, right, bottom, top, zNear, zFar );
    
    glUniformMatrix4fv( glGetUniformLocation( program, "projection" ), 1, GL_TRUE, proj_matrix );
    
    //---- camera extrinsic parameters
    
    float tr_y = Theta3[Yaxis]* M_PI/180.0;
    float tr_z = Theta3[Zaxis]* M_PI/180.0;
    float eye_z = r * (cos(tr_z)) * cos(tr_y);
    float eye_x = r * (cos(tr_z)) * sin(tr_y);
    float eye_y = r * sin(tr_z);
    
    vec4 up = vec4(0.0, cos(tr_z), 0.0, 0.0);
    //cout << up << ' ' << normalize(up) << endl;
    
    view_matrix = LookAt( vec4(eye_x, eye_y, eye_z, 1.0), vec4(0.0, 0.0, 0.0, 1.0), vec4(0.0, cos(tr_z), 0.0, 0.0));
    
    glUniformMatrix4fv( glGetUniformLocation( program, "view" ), 1, GL_TRUE, view_matrix );
    
    
    
    
    //---- cube
    
    // material properties
    vec4 ka, kd, ks;
    //ka = vec4(0.4, 0.4, 0.4, 1.0);
    //kd = vec4(0.3, 0.3, 1.0, 1.0);
    //ks = vec4(0.2, 0.2, 0.2, 1.0);
    
    ka = vec4(0.7, 0.0, 1.0, 1.0);
    kd = vec4(0.7, 0.0, 1.0, 1.0);
    ks = vec4(0.7, 0.0, 1.0, 1.0);
    float s = 1.0;
    
    glUniform4fv( glGetUniformLocation(program, "ka"), 1, ka );
    glUniform4fv( glGetUniformLocation(program, "kd"), 1, kd );
    glUniform4fv( glGetUniformLocation(program, "ks"), 1, ks );
    glUniform1f( glGetUniformLocation(program, "Shininess"), s );
    
    mat4 transform = Translate( -15.0, 1.0, -5.0 ) * Scale(2.0, 2.0, 2.0);
    
    glUniformMatrix4fv( glGetUniformLocation( program, "model" ), 1, GL_TRUE, transform );
    
    glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(CUBE_OFFSET) );
    glVertexAttribPointer( vNormal, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(CUBE_NORMALS_OFFSET) );
    
    glUniform1i( glGetUniformLocation(program, "obj_color_on"), false );
    
    glDrawArrays( GL_TRIANGLES, 0, NumVerticesCube );
    
    
    
    
    //---- ground
    
    // material properties

    //ka = vec4(0.4, 0.4, 0.4, 1.0);
    //kd = vec4(0.3, 0.3, 1.0, 1.0);
    //ks = vec4(0.2, 0.2, 0.2, 1.0);
    
    ka = vec4(0.9, 0.5, 0.1, 1.0);
    kd = vec4(0.9, 0.5, 0.1, 1.0);
    ks = vec4(0.9, 0.5, 0.1, 1.0);
    s = 1.0;
    
    glUniform4fv( glGetUniformLocation(program, "ka"), 1, ka );
    glUniform4fv( glGetUniformLocation(program, "kd"), 1, kd );
    glUniform4fv( glGetUniformLocation(program, "ks"), 1, ks );
    glUniform1f( glGetUniformLocation(program, "Shininess"), s );
    
    transform = Translate( 0.0, 0.0, 0.0 ) * Scale(20.0, 1.0, 40.0);
    
    glUniformMatrix4fv( glGetUniformLocation( program, "model" ), 1, GL_TRUE, transform );
    
    glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(GROUND_OFFSET) );
    glVertexAttribPointer( vNormal, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(GROUND_NORMALS_OFFSET) );
    
    glUniform1i( glGetUniformLocation(program, "obj_color_on"), true );
    glUniform4fv( glGetUniformLocation(program, "obj_color"), 1, vec4(0.0, 0.9, 0.0, 1.0) );
    
    glDrawArrays( GL_TRIANGLES, 0, NumVerticesCube );
    
    
    
    //---- ground1
    
    // material properties
    
    //ka = vec4(0.4, 0.4, 0.4, 1.0);
    //kd = vec4(0.3, 0.3, 1.0, 1.0);
    //ks = vec4(0.2, 0.2, 0.2, 1.0);
    
    ka = vec4(0.1, 0.9, 0.1, 1.0);
    kd = vec4(0.1, 0.9, 0.1, 1.0);
    ks = vec4(0.1, 0.9, 0.1, 1.0);
    s = 1.0;
    
    glUniform4fv( glGetUniformLocation(program, "ka"), 1, ka );
    glUniform4fv( glGetUniformLocation(program, "kd"), 1, kd );
    glUniform4fv( glGetUniformLocation(program, "ks"), 1, ks );
    glUniform1f( glGetUniformLocation(program, "Shininess"), s );
    
    transform = Translate( -20.0, 0.0, 0.0 ) * Scale(20.0, 1.0, 40.0);
    
    glUniformMatrix4fv( glGetUniformLocation( program, "model" ), 1, GL_TRUE, transform );
    
    glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(GROUND_OFFSET) );
    glVertexAttribPointer( vNormal, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(GROUND_NORMALS_OFFSET) );
    
    glUniform1i( glGetUniformLocation(program, "obj_color_on"), true );
    glUniform4fv( glGetUniformLocation(program, "obj_color"), 1, vec4(0.0, 0.9, 0.0, 1.0) );
    
    glDrawArrays( GL_TRIANGLES, 0, NumVerticesCube );
    
    
    
    
    //---- ground2
    
    // material properties
    
    //ka = vec4(0.4, 0.4, 0.4, 1.0);
    //kd = vec4(0.3, 0.3, 1.0, 1.0);
    //ks = vec4(0.2, 0.2, 0.2, 1.0);
    
    
    lpos = vec4( 15.0, 2.0, 0.0, 1.0 );
    glUniform4fv( glGetUniformLocation(program, "LightPosition"), 1, lpos);
    
    
    ka = vec4(0.1, 0.9, 0.1, 1.0);
    kd = vec4(0.1, 0.9, 0.1, 1.0);
    ks = vec4(0.1, 0.9, 0.1, 1.0);
    s = 1.0;
    
    glUniform4fv( glGetUniformLocation(program, "ka"), 1, ka );
    glUniform4fv( glGetUniformLocation(program, "kd"), 1, kd );
    glUniform4fv( glGetUniformLocation(program, "ks"), 1, ks );
    glUniform1f( glGetUniformLocation(program, "Shininess"), s );
    
    transform = Translate( 20.0, 0.0, 0.0 ) * Scale(20.0, 1.0, 40.0);
    
    glUniformMatrix4fv( glGetUniformLocation( program, "model" ), 1, GL_TRUE, transform );
    
    glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(GROUND_OFFSET) );
    glVertexAttribPointer( vNormal, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(GROUND_NORMALS_OFFSET) );
    
    glUniform1i( glGetUniformLocation(program, "obj_color_on"), true );
    glUniform4fv( glGetUniformLocation(program, "obj_color"), 1, vec4(0.0, 0.9, 0.0, 1.0) );
    
    glDrawArrays( GL_TRIANGLES, 0, NumVerticesCube );
    
    
    //---- cube1
    
    // material properties
    ka, kd, ks;
    //ka = vec4(0.4, 0.4, 0.4, 1.0);
    //kd = vec4(0.3, 0.3, 1.0, 1.0);
    //ks = vec4(0.2, 0.2, 0.2, 1.0);
    
    ka = vec4(0.7, 0.0, 1.0, 1.0);
    kd = vec4(0.7, 0.0, 1.0, 1.0);
    ks = vec4(0.7, 0.0, 1.0, 1.0);
    s = 1.0;
    
    glUniform4fv( glGetUniformLocation(program, "ka"), 1, ka );
    glUniform4fv( glGetUniformLocation(program, "kd"), 1, kd );
    glUniform4fv( glGetUniformLocation(program, "ks"), 1, ks );
    glUniform1f( glGetUniformLocation(program, "Shininess"), s );
    
    transform = Translate( 15.0, 1.0, -5.0 ) * Scale(2.0, 2.0, 2.0);
    
    glUniformMatrix4fv( glGetUniformLocation( program, "model" ), 1, GL_TRUE, transform );
    
    glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(CUBE_OFFSET) );
    glVertexAttribPointer( vNormal, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(CUBE_NORMALS_OFFSET) );
    
    glUniform1i( glGetUniformLocation(program, "obj_color_on"), false );
    
    glDrawArrays( GL_TRIANGLES, 0, NumVerticesCube );
    
    
    
    
    
    
    
    
    //---- light pole 1
    
    // material properties
    
    //ka = vec4(0.4, 0.4, 0.4, 1.0);
    //kd = vec4(0.3, 0.3, 1.0, 1.0);
    //ks = vec4(0.2, 0.2, 0.2, 1.0);
    
    ka = vec4(0.2, 0.0, 0.0, 1.0);
    kd = vec4(0.2, 0.0, 0.0, 1.0);
    ks = vec4(0.2, 0.0, 0.0, 1.0);
    s = 5.0;
    
    lpos = vec4( -15.0, 3.3, 2.0, 1.0 );
    glUniform4fv( glGetUniformLocation(program, "LightPosition"), 1, lpos);
    
    glUniform4fv( glGetUniformLocation(program, "ka"), 1, ka );
    glUniform4fv( glGetUniformLocation(program, "kd"), 1, kd );
    glUniform4fv( glGetUniformLocation(program, "ks"), 1, ks );
    glUniform1f( glGetUniformLocation(program, "Shininess"), s );
    
    transform = Translate( -15.0, 3.0, 0.0 ) * Scale(poleRadius, 5.0, poleRadius);
    
    glUniformMatrix4fv( glGetUniformLocation( program, "model" ), 1, GL_TRUE, transform );
    
    glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(LIGHT_POLE1_OFFSET) );
    glVertexAttribPointer( vNormal, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(LIGHT_POLE1NORMALS_OFFSET) );
    
    glUniform1i( glGetUniformLocation(program, "obj_color_on"), false );
    
    glDrawArrays( GL_TRIANGLES, 0, NumVerticesCylinder );
    
    
    
    //---- sphere for light 1

    
    
    lpos = vec4( -15.0, 3.0, 0.0, 1.0 );
    glUniform4fv( glGetUniformLocation(program, "LightPosition"), 1, lpos);
    
    // material properties
    ka = vec4(1.0, 1.0, 1.0, 1.0);
    kd = vec4(1.0, 1.0, 1.0, 1.0);
    ks = vec4(1.0, 1.0, 1.0, 1.0);
    s = 25.0;
    
    glUniform4fv( glGetUniformLocation(program, "ka"), 1, ka );
    glUniform4fv( glGetUniformLocation(program, "kd"), 1, kd );
    glUniform4fv( glGetUniformLocation(program, "ks"), 1, ks );
    glUniform1f( glGetUniformLocation(program, "Shininess"), s );

    transform = Translate( -15.0, 4.3, 0.0 ) * Scale(poleRadius, poleRadius, poleRadius);
    
    glUniformMatrix4fv( glGetUniformLocation( program, "model" ), 1, GL_TRUE, transform );
    
    glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(SPHERE_LIGHT1_OFFSET) );
    glVertexAttribPointer( vNormal, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(SPHERE_LIGHT1_NORMALS_OFFSET) );
    
    glUniform1i( glGetUniformLocation(program, "obj_color_on"), false );
    
    glDrawArrays( GL_TRIANGLES, 0, NumVerticesSphere );
    
     
    
    
    
    
    //---- light base 1
    
    lpos = vec4( -15.0, 1.5, 0.0, 1.0 );
    glUniform4fv( glGetUniformLocation(program, "LightPosition"), 1, lpos);
    
    
    // material properties
    
    //ka = vec4(0.4, 0.4, 0.4, 1.0);
    //kd = vec4(0.3, 0.3, 1.0, 1.0);
    //ks = vec4(0.2, 0.2, 0.2, 1.0);
    
    ka = vec4(0.8, 0.8, 0.8, 1.0);
    kd = vec4(0.8, 0.8, 0.8, 1.0);
    ks = vec4(0.8, 0.8, 0.8, 1.0);
    s = 2.0;
    
    glUniform4fv( glGetUniformLocation(program, "ka"), 1, ka );
    glUniform4fv( glGetUniformLocation(program, "kd"), 1, kd );
    glUniform4fv( glGetUniformLocation(program, "ks"), 1, ks );
    glUniform1f( glGetUniformLocation(program, "Shininess"), s );
    
    transform = Translate( -15.0, 0.5, 0.0 ) * Scale(3.0, 1.5, 3.0);
    
    glUniformMatrix4fv( glGetUniformLocation( program, "model" ), 1, GL_TRUE, transform );
    
    glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(LIGHT1_BASE_OFFSET) );
    glVertexAttribPointer( vNormal, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(LIGHT1_BASENORMALS_OFFSET) );
    
    glUniform1i( glGetUniformLocation(program, "obj_color_on"), false );
    
    glDrawArrays( GL_TRIANGLES, 0, NumVerticesCylinder );
    
    
    
    
    
    
    //---- light pole 2
    
    // material properties
    
    //ka = vec4(0.4, 0.4, 0.4, 1.0);
    //kd = vec4(0.3, 0.3, 1.0, 1.0);
    //ks = vec4(0.2, 0.2, 0.2, 1.0);
    
    ka = vec4(0.2, 0.0, 0.0, 1.0);
    kd = vec4(0.2, 0.0, 0.0, 1.0);
    ks = vec4(0.2, 0.0, 0.0, 1.0);
    s = 5.0;
    
    glUniform4fv( glGetUniformLocation(program, "ka"), 1, ka );
    glUniform4fv( glGetUniformLocation(program, "kd"), 1, kd );
    glUniform4fv( glGetUniformLocation(program, "ks"), 1, ks );
    glUniform1f( glGetUniformLocation(program, "Shininess"), s );
    
    transform = Translate( 15.0, 3.0, 0.0 ) * Scale(poleRadius, 5.0, poleRadius);
    
    glUniformMatrix4fv( glGetUniformLocation( program, "model" ), 1, GL_TRUE, transform );
    
    glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(LIGHT_POLE2_OFFSET) );
    glVertexAttribPointer( vNormal, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(LIGHT_POLE2NORMALS_OFFSET) );
    
    glUniform1i( glGetUniformLocation(program, "obj_color_on"), false );
    
    glDrawArrays( GL_TRIANGLES, 0, NumVerticesCylinder );
    
    
    
    
    
    
    //---- sphere for light 2
    
    
    
    
    
    // material properties
    ka = vec4(1.0, 1.0, 1.0, 1.0);
    kd = vec4(1.0, 1.0, 1.0, 1.0);
    ks = vec4(1.0, 1.0, 1.0, 1.0);
    s = 25.0;
    
    glUniform4fv( glGetUniformLocation(program, "ka"), 1, ka );
    glUniform4fv( glGetUniformLocation(program, "kd"), 1, kd );
    glUniform4fv( glGetUniformLocation(program, "ks"), 1, ks );
    glUniform1f( glGetUniformLocation(program, "Shininess"), s );
    
    transform = Translate( 15.0, 4.3, 0.0 ) * Scale(poleRadius, poleRadius, poleRadius);
    
    glUniformMatrix4fv( glGetUniformLocation( program, "model" ), 1, GL_TRUE, transform );
    
    glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(SPHERE_LIGHT2_OFFSET) );
    glVertexAttribPointer( vNormal, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(SPHERE_LIGHT2_NORMALS_OFFSET) );
    
    glUniform1i( glGetUniformLocation(program, "obj_color_on"), false );
    
    glDrawArrays( GL_TRIANGLES, 0, NumVerticesSphere );
    
    
    
    
    
    
    //---- light base 2
    
    
    lpos = vec4( 15.0, 1.5, 0.0, 1.0 );
    glUniform4fv( glGetUniformLocation(program, "LightPosition"), 1, lpos);
    
    // material properties
    
    //ka = vec4(0.4, 0.4, 0.4, 1.0);
    //kd = vec4(0.3, 0.3, 1.0, 1.0);
    //ks = vec4(0.2, 0.2, 0.2, 1.0);
    
    ka = vec4(0.8, 0.8, 0.8, 1.0);
    kd = vec4(0.8, 0.8, 0.8, 1.0);
    ks = vec4(0.8, 0.8, 0.8, 1.0);
    s = 1.5;
    
    glUniform4fv( glGetUniformLocation(program, "ka"), 1, ka );
    glUniform4fv( glGetUniformLocation(program, "kd"), 1, kd );
    glUniform4fv( glGetUniformLocation(program, "ks"), 1, ks );
    glUniform1f( glGetUniformLocation(program, "Shininess"), s );
    
    transform = Translate( 15.0, 0.5, 0.0 ) * Scale(3.0, 1.5, 3.0);
    
    glUniformMatrix4fv( glGetUniformLocation( program, "model" ), 1, GL_TRUE, transform );
    
    glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(LIGHT2_BASE_OFFSET) );
    glVertexAttribPointer( vNormal, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(LIGHT2_BASENORMALS_OFFSET) );
    
    glUniform1i( glGetUniformLocation(program, "obj_color_on"), false );
    
    glDrawArrays( GL_TRIANGLES, 0, NumVerticesCylinder );
    
    
    
    //---- light 1
    
    
    lpos = vec4( -15.0, 3.0, 0.0, 1.0 );
    
    glUniform1i(glGetUniformLocation(program, "reverse"), true);
    glUniform4fv( glGetUniformLocation(program, "LightPosition"), 1, lpos);
    
    
    // material properties
    ka = vec4(1.0, 1.0, 1.0, 1.0);
    kd = vec4(1.0, 1.0, 1.0, 1.0);
    ks = vec4(1.0, 1.0, 1.0, 1.0);
    s = 25.0;
    
    glUniform4fv( glGetUniformLocation(program, "ka"), 1, ka );
    glUniform4fv( glGetUniformLocation(program, "kd"), 1, kd );
    glUniform4fv( glGetUniformLocation(program, "ks"), 1, ks );
    glUniform1f( glGetUniformLocation(program, "Shininess"), s );
    
    transform = Translate( -15.0, 5.5, 0.0 ) * Scale(1.5, 1.5, 1.5);
    
    glUniformMatrix4fv( glGetUniformLocation( program, "model" ), 1, GL_TRUE, transform );
    
    glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(LIGHT1_OFFSET) );
    glVertexAttribPointer( vNormal, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(LIGHT1_NORMAL) );
    
    glUniform1i( glGetUniformLocation(program, "obj_color_on"), false );
    
    glDrawArrays( GL_TRIANGLES, 0, NumVerticesSphere );
    
    
    
    
    //---- light 2
    
    
    
    lpos = vec4( 15.0, 3.0, 0.0, 1.0 );
    glUniform4fv( glGetUniformLocation(program, "LightPosition"), 1, lpos);
    
    // material properties
    ka = vec4(1.0, 1.0, 1.0, 1.0);
    kd = vec4(1.0, 1.0, 1.0, 1.0);
    ks = vec4(1.0, 1.0, 1.0, 1.0);
    s = 25.0;
    
    glUniform4fv( glGetUniformLocation(program, "ka"), 1, ka );
    glUniform4fv( glGetUniformLocation(program, "kd"), 1, kd );
    glUniform4fv( glGetUniformLocation(program, "ks"), 1, ks );
    glUniform1f( glGetUniformLocation(program, "Shininess"), s );
    
    transform = Translate( 15.0, 5.5, 0.0 ) * Scale(1.5, 1.5, 1.5);
    
    glUniformMatrix4fv( glGetUniformLocation( program, "model" ), 1, GL_TRUE, transform );
    
    glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(LIGHT2_OFFSET) );
    glVertexAttribPointer( vNormal, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(LIGHT2_NORMAL) );
    
    glUniform1i( glGetUniformLocation(program, "obj_color_on"), false );
    
    glDrawArrays( GL_TRIANGLES, 0, NumVerticesSphere );
    
    
    
    glutSwapBuffers();
    
}


//----------------------------------------------------------------------------

void
keyboard( unsigned char key, int x, int y )
{
    switch( key ) {
        case 033:    // Escape key
        case 'q': case 'Q':
            exit( EXIT_SUCCESS );
            break;
            
        case 'y':    //---- theta
            Axis = Yaxis;
            Theta3[Axis] += 5.0;
            Theta3[Axis] = fmod(Theta3[Axis], 360.0);
            glutPostRedisplay();
        case 'Y':    //---- theta
            Axis = Yaxis;
            Theta3[Axis] -= 5.0;
            Theta3[Axis] = fmod(Theta3[Axis], 360.0);
            glutPostRedisplay();
            break;
        case 'z':    //---- phi
            Axis = Zaxis;
            Theta3[Axis] += 5.0;
            Theta3[Axis] = fmod(Theta3[Axis], 360.0);
            glutPostRedisplay();
            break;
        case 'Z':    //---- phi
            Axis = Zaxis;
            Theta3[Axis] -= 5.0;
            Theta3[Axis] = fmod(Theta3[Axis], 360.0);
            glutPostRedisplay();
            break;
        case 'r':    //---- increase radius
            r += 0.5;
            glutPostRedisplay();
            break;
        case 'R':    //---- decrease radius
            r -= 0.5;
            glutPostRedisplay();
            break;
            
        case 'u':
            //view_matrix = default_view_matrix;
            Theta3[Xaxis] = 0.0;
            Theta3[Yaxis] = 0.0;
            Theta3[Zaxis] = 30.0;
            r+=25.0;
            glutPostRedisplay();
            break;
        case 'c':

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

void
idle( void )
{
    Theta1[Axis] = fmod(Theta1[Axis]+1, 360.0);
    Theta2[Axis] = fmod(Theta2[Axis]+2, 360.0);
    
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
    //glutIdleFunc( idle );
    
    glutMainLoop();
    return 0;
}

















