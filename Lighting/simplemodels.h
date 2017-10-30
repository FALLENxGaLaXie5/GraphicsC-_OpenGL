
//----------------------------------------------------------------------------

const int NumVerticesCube = 36; //(6 faces)(2 triangles/face)(3 vertices/triangle)

extern vec4 points_cube[NumVerticesCube];
extern vec4 colors[NumVerticesCube];
extern vec3 normals_cube[NumVerticesCube];

void colorcube();


//----------------------------------------------------------------------------

const int segments = 16;
const int NumVerticesCylinder = segments*6 + segments*3*2;

extern vec4 points_cylinder[NumVerticesCylinder];
extern vec3 normals_cylinder[NumVerticesCylinder];

void colortube();


//----------------------------------------------------------------------------

const int ssegments = 32;
const int NumVerticesSphere = ssegments*6*(ssegments-2) + ssegments*3*2;

extern vec4 points_sphere[NumVerticesSphere];
extern vec3 normals_sphere[NumVerticesSphere];

void colorbube();


//----------------------------------------------------------------------------
const int torusLevels = 32;
const int NumVerticesTorus = torusLevels * 6 * torusLevels;

extern vec4 points_torus[NumVerticesTorus];
extern vec3 normals_torus[NumVerticesTorus];

void colorTorus();





