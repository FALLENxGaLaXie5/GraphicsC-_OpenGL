
/**
attribute vec4 vPosition;

void
main()
{
    gl_Position = vPosition;
}
**/


#version 150

//---- ins
in vec4    vPosition;          // object-space vertex position.
in vec4    vColor;             // per-vertex color

//---- outs
out vec4 color;

//---- uniforms
uniform mat4 model;

//===========================================================================
// vshader main
//===========================================================================
void
main()
{
    float rotateOnX = 0.7;
    float rotateOnY = 0.0;
    float rotateOnZ = 0.7;
    
    
    
    //---- transform the vertex
    float cX = cos(rotateOnX);
    float sX = sin(rotateOnX);
    
    ////
    
    float cY = cos(rotateOnY);
    float sY = sin(rotateOnY);
    
    ////
    
    float cZ = cos(rotateOnZ);
    float sZ = sin(rotateOnZ);
    
    
    mat4 rx = mat4(1.0, 0.0, 0.0, 0.0,
                   0.0, cX, -sX, 0.0,
                   0.0, sX, cX, 0.0,
                   0.0, 0.0, 0.0, 1.0);
    
    
    //float mat[10] = {0.5,0.5,0.0,0.0};
    
    /**
    mat4 rx = mat4(1.0, 0.0, 0.0, 0.0,
                   0.0, c, ns, 0.0,
                   0.0, s, c, 0.0,
                   0.0, 0.0, 0.0, 1.0);
    
     */
    mat4 ry = mat4(cY, 0.0, sY, 0.0,
                   0.0, 1.0, 0.0, 0.0,
                   -sY, 0.0, cY, 0.0,
                   0.0, 0.0, 0.0, 1.0);
    
    
    
    mat4 rz = mat4(cZ, -sZ, 0.0, 0.0,
                   sZ, cZ, 0.0, 0.0,
                   0.0, 0.0, 1.0, 0.0,
                   0.0, 0.0, 0.0, 1.0);
    
    //gl_Position = vPosition * rz;// * ry * rz;
    
    gl_Position = model * vPosition;
    
    color = vColor;
}

