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
#include <string>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <osmgl/elements.hh>


class CT_OSMGL {
	std::string _str_filename;

	/* OSM nodes */
	std::map<uint32_t, CT_OSM_NODE*> _list_node;

	/* OSM Ways */
	std::map<uint32_t, CT_OSM_WAY*> _list_way;
public:
	CT_OSMGL(char const *);
	void f_render(float in_f_lat_min, float in_f_lon_min, float in_f_lat_max, float in_f_lon_max);
	void f_parse_xml(xmlNode * a_node);
	void f_parse_xml_rules(xmlNode * a_node);

	void f_open_rules(char const * in_str_filename);

private:
	void m_f_glColor(const char * in_pc_color);
};

#endif /* OSMGL_HH_ */
