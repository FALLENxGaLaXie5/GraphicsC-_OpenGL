
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


//===========================================================================
// vshader main
//===========================================================================
void
main()
{
    //---- transform the vertex
    float c = cos(0.2);
    float nc = -(cos(0.2));
    float s = sin(0.2);
    float ns = -(sin(0.2));
    
    
    mat4 rx = mat4(1.0, 0.0, 0.0, 0.0,
                   0.0, c, -s, 0.0,
                   0.0, s, c, 0.0,
                   0.0, 0.0, 0.0, 1.0);
    
    
    //float mat[10] = {0.5,0.5,0.0,0.0};
    
    /**
    mat4 rx = mat4(1.0, 0.0, 0.0, 0.0,
                   0.0, c, ns, 0.0,
                   0.0, s, c, 0.0,
                   0.0, 0.0, 0.0, 1.0);
    
     */
    mat4 ry = mat4(c, 0.0, s, 0.0,
                   0.0, 1.0, 0.0, 0.0,
                   ns, 0.0, c, 0.0,
                   0.0, 0.0, 0.0, 1.0);
    
    
    gl_Position = vPosition * rx * ry;
    
    color = vColor;
}

