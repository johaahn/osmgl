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
		GLfloat weight[4], GLdouble **dataOut) {
	GLdouble *vertex;

	// TODO DELETE
	vertex = new GLdouble[3];// * sizeof(GLdouble));
	vertex[0] = coords[0];
	vertex[1] = coords[1];
	vertex[2] = coords[2];

	*dataOut = vertex;
}

void f_vertexCallback(GLvoid *vertex) {
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
	//gluTessProperty(tobj, GLU_TESS_BOUNDARY_ONLY, GL_TRUE);


	// No limits by default
	_b_has_limits = false;
}
CT_TESSELLATION::~CT_TESSELLATION(void) {
	gluDeleteTess(tobj);
}

void CT_TESSELLATION::m_f_set_limits(GLdouble in_f_x_min, GLdouble in_f_x_max,
		GLdouble in_f_y_min, GLdouble in_f_y_max) {

	// Save extreme coordinates
	_ad_coord_ext1 = {in_f_x_min, in_f_y_min, 0.0};
	_ad_coord_ext2 = {in_f_x_min, in_f_y_max, 0.0};
	_ad_coord_ext3 = {in_f_x_max, in_f_y_max, 0.0};
	_ad_coord_ext4 = {in_f_x_max, in_f_y_min, 0.0};

	// Set limits
	_b_has_limits = true;
	gluTessProperty(tobj, GLU_TESS_WINDING_RULE, GLU_TESS_WINDING_ABS_GEQ_TWO);
}

void CT_TESSELLATION::m_f_vertex2dv(GLdouble * in_pd) {
	gluTessVertex(tobj, in_pd, in_pd);
}

void CT_TESSELLATION::m_f_glBegin() {

	gluTessBeginPolygon(tobj, NULL);

	if (_b_has_limits) {
		gluTessBeginContour(tobj);
		gluTessVertex(tobj, _ad_coord_ext1, _ad_coord_ext1);
		gluTessVertex(tobj, _ad_coord_ext2, _ad_coord_ext2);
		gluTessVertex(tobj, _ad_coord_ext3, _ad_coord_ext3);
		gluTessVertex(tobj, _ad_coord_ext4, _ad_coord_ext4);
		gluTessEndContour(tobj);
	}

	gluTessBeginContour(tobj);
}

void CT_TESSELLATION::m_f_glEnd(void) {
	gluTessEndContour(tobj);
	gluTessEndPolygon(tobj);

}

