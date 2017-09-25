//----------------------------------------------------------------------------

const int NumVerticesCube = 36; //(6 faces)(2 triangles/face)(3 vertices/triangle)

extern vec4 points_cube[NumVerticesCube];
extern vec4 colors[NumVerticesCube];

void colorcube();


//----------------------------------------------------------------------------

const int segments = 16;

const int NumVerticesCylinder = segments*6 + segments*3*2;

const int sphericalLevels = 100;
const int NumVerticesSphere = sphericalLevels * 6 * (sphericalLevels);// + sphericalLevels * 3 * 2;


const int torusLevels = 32;
const int NumVerticesTorus = torusLevels * 6 * torusLevels;


extern vec4 points_cylinder[NumVerticesCylinder];
extern vec4 points_sphere[NumVerticesSphere];
extern vec4 points_torus[NumVerticesTorus];

void colortube();
void colorsphere();
void colorTorus();