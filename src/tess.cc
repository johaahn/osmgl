/*
 * tess.cc
 *
 *  Created on: 7 nov. 2010
 *      Author: johaahn
 */

#include <GL/gl.h>
#include <GL/glu.h>

#include <stdlib.h>
#include <tess.hh>

extern "C" {
void f_combineCallback(GLdouble coords[3], GLdouble *vertex_data[4],
		GLfloat weight[4], GLdouble **dataOut);
void f_vertexCallback(GLvoid *vertex);
}

void f_combineCallback(GLdouble coords[3], GLdouble *vertex_data[4],
		GLfloat weight[4], GLdouble **dataOut)
{
	GLdouble *vertex;

	// TODO DELETE
	vertex = new GLdouble[3];// * sizeof(GLdouble));
	vertex[0] = coords[0];
	vertex[1] = coords[1];
	vertex[2] = coords[2];

	*dataOut = vertex;
}

void  f_vertexCallback(GLvoid *vertex)
{
	GLdouble *ptr;
	ptr = (GLdouble *) vertex;

	glVertex3dv((GLdouble *) ptr);
	//glColor3dv((GLdouble *) ptr + 3);
}

CT_TESSELLATION::CT_TESSELLATION(void) {
	// Create a new tessellation object
	tobj = gluNewTess();

	// Set callback functions
	gluTessCallback(tobj, GLU_TESS_VERTEX, (GLvoid(*)()) &f_vertexCallback);
	gluTessCallback(tobj, GLU_TESS_BEGIN, (GLvoid(*)()) &glBegin);
	gluTessCallback(tobj, GLU_TESS_END, (GLvoid(*)()) &glEnd);
	gluTessCallback(tobj, GLU_TESS_COMBINE, (GLvoid(*)()) &f_combineCallback);

	// Set the winding rule
	gluTessProperty(tobj, GLU_TESS_WINDING_RULE, GLU_TESS_WINDING_POSITIVE);
}
CT_TESSELLATION::~CT_TESSELLATION(void)
{
	gluDeleteTess(tobj);
}

void
CT_TESSELLATION::m_f_vertex2dv(GLdouble * in_pd)
{
	gluTessVertex(tobj, in_pd, in_pd);
}

void
CT_TESSELLATION::m_f_glBegin(void)
{
	gluTessBeginPolygon(tobj, NULL);
	gluTessBeginContour(tobj);
}
void
CT_TESSELLATION::m_f_glEnd(void)
{
	gluTessEndContour(tobj);
	gluTessEndPolygon(tobj);

}




