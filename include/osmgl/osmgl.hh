/*
 * osmgl.hh
 *
 *  Created on: 3 juin 2010
 *      Author: johaahn
 */

#ifndef OSMGL_HH_
#define OSMGL_HH_

#define EC_OSMGL_BYPASS -2
#define EC_OSMGL_FAILURE -1
#define EC_OSMGL_SUCCESS 0

#include <osmgl/elements.hh>

class CT_OSMGL {
	string _str_filename;

	/* OSM nodes */
	map<uint32_t, CT_OSM_NODE*> _list_node;

	/* OSM Ways */
	map<uint32_t, CT_OSM_WAY*> _list_way;
public:
	CT_OSMGL(string in_str_filename);
	void f_parse_xml(xmlNode * a_node);
	void f_render(float in_f_lat_min, float in_f_lon_min, float in_f_lat_max, float in_f_lon_max);
};

#endif /* OSMGL_HH_ */
