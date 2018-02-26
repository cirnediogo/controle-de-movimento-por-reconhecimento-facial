/*
created with obj2opengl.pl

source file : ./dcaSeams.obj
vertices : 6761
faces : 12217
normals : 723
texture coords : 9084


// include generated arrays
#include "./dcaSeams.h"

// set input data to arrays
glVertexPointer(3, GL_FLOAT, dcaSeamsStride, dcaSeamsVerts);
glNormalPointer(GL_FLOAT, dcaSeamsStride, dcaSeamsNormals);
glTexCoordPointer(2, GL_FLOAT, dcaSeamsStride, dcaSeamsTexCoords);

// draw data
glDrawArrays(GL_TRIANGLES, 0, dcaSeamsNumVerts);
*/

typedef struct {
 float vertex[3];
 float normal[3];
 float texcoord[2];
} dcaSeamsData_t;

extern dcaSeamsData_t dcaSeamsData[];

static const GLsizei dcaSeamsNumVerts = 36651;

static const GLsizei dcaSeamsStride = sizeof(dcaSeamsData_t);

static const float *dcaSeamsVerts = dcaSeamsData[0].vertex;
static const float *dcaSeamsNormals = dcaSeamsData[0].normal;
static const float *dcaSeamsTexCoords = dcaSeamsData[0].texcoord;

static void dcaSeamsDraw(void){
 glEnableClientState(GL_VERTEX_ARRAY);
 glEnableClientState(GL_NORMAL_ARRAY);
 glEnableClientState(GL_TEXTURE_COORD_ARRAY);
 glVertexPointer(3, GL_FLOAT, dcaSeamsStride, dcaSeamsVerts);
 glNormalPointer(GL_FLOAT, dcaSeamsStride, dcaSeamsNormals);
 glTexCoordPointer(2, GL_FLOAT, dcaSeamsStride, dcaSeamsTexCoords);
 glDrawArrays(GL_TRIANGLES, 0, dcaSeamsNumVerts);
 glDisableClientState(GL_TEXTURE_COORD_ARRAY);
 glDisableClientState(GL_NORMAL_ARRAY);
 glDisableClientState(GL_VERTEX_ARRAY);
}
