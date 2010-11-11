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

	bool _b_has_limits;
	GLdouble _ad_coord_ext1 [3];
	GLdouble _ad_coord_ext2 [3];
	GLdouble _ad_coord_ext3 [3];
	GLdouble _ad_coord_ext4 [3];

public:
	GLUtesselator *tobj; // the tessellation object

	void m_f_vertex2dv(GLdouble * in_pd);

	CT_TESSELLATION();
	~CT_TESSELLATION(void);

	void m_f_set_limits(GLdouble in_f_x_min, GLdouble in_f_x_max,
			GLdouble in_f_y_min, GLdouble in_f_y_max);
	void m_f_glBegin(void);
	void m_f_glEnd(void);

};

#endif /* TESS_HH_ */
