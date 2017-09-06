/**
void
main()
{
    gl_FragColor = vec4( 1.0, 0.0, 0.0, 1.0 );
}
**/



#version 150

//---- ins
in  vec4 color;

//---- outs
out vec4 FragColor;

//===========================================================================
// fshader main
//===========================================================================
void
main()
{
    //---- compute the output color for this fragment
    
    FragColor = color;
}
