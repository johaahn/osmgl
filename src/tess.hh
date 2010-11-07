/*
 * tess.hh
 *
 *  Created on: 7 nov. 2010
 *      Author: johaahn
 */

#ifndef TESS_HH_
#define TESS_HH_

#include <GL/gl.h>
#include <GL/glu.h>

class CT_TESSELLATION {

private:



public:
	GLUtesselator *tobj; // the tessellation object

	void m_f_vertex2dv(GLdouble * in_pd);

	CT_TESSELLATION(void);
	~CT_TESSELLATION(void);
	void m_f_glBegin(void);
	void m_f_glEnd(void);

};

#endif /* TESS_HH_ */
