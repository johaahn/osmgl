/*
 * OsmGL core.
 *
 * Copyright (C) 2010 <Johann Baudy> johann.baudy@gnu-log.net
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 3.0 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA
 */

#include <iostream>
#include <string>

#include <osmgl/elements.hh>
#include <osmgl/osmgl.hh>

#include <GL/gl.h>
#include <tess.hh>

#ifndef LIBXML_TREE_ENABLED
#error "libxml2 must have tree enabled"
#endif
//using namespace std;


void CT_OSMGL::m_f_glColor(const char * in_pc_color) {
	std::string str_color = in_pc_color;
	if (str_color[0] == '#') {
		str_color[0] = ' ';
		uint32_t i_tmp = strtol(str_color.c_str(), NULL, 16);

		char i_blue = i_tmp & 0xff;
		char i_green = (i_tmp >> 8) & 0xff;
		char i_red = (i_tmp >> 16) & 0xff;

		glColor4ub(i_red, i_green, i_blue, 255);

	} else {
		GLubyte red[] = { 255, 0, 0, 255 };
		GLubyte green[] = { 0, 255, 0, 255 };
		GLubyte blue[] = { 0, 0, 255, 255 };
		GLubyte white[] = { 255, 255, 255, 255 };
		GLubyte yellow[] = { 0, 255, 255, 255 };
		GLubyte black[] = { 0, 0, 0, 255 };
		GLubyte orange[] = { 255, 255, 0, 255 };
		GLubyte purple[] = { 255, 0, 255, 255 };
		GLubyte grey[] = { 50, 50, 50, 255 };

		if (str_color == "purple") {
			glColor4ubv(purple);
		} else if (str_color == "grey") {
			glColor4ubv(grey);
		} else if (str_color == "green") {
			glColor4ubv(green);
		} else if (str_color == "white") {
			glColor4ubv(white);
		} else {
			glColor4f(1.0, 1.0, 1.0, 1.0);
		}
	}
}
void CT_OSMGL::f_render(void) {
	f_render(_f_minlat, _f_maxlat, _f_minlon, _f_maxlon);
}

void CT_OSMGL::f_render(float in_f_lat_min, float in_f_lat_max,
		float in_f_lon_min, float in_f_lon_max) {
	CT_OSM_COORD c_coord_min(in_f_lat_min, in_f_lon_min);
	CT_OSM_COORD c_coord_max(in_f_lat_max, in_f_lon_max);



	map<uint32_t, CT_OSM_WAY*>::iterator ppc_way;
	/* Tesselation class */
	CT_TESSELLATION c_tess;
	c_tess.m_f_set_limits(c_coord_min._d_x, c_coord_max._d_x, c_coord_min._d_y, c_coord_max._d_y);



	glDisable(GL_LINE_STIPPLE);
	m_f_glColor("white");
	glBegin(GL_LINE_STRIP);
	glVertex2f(c_coord_min._d_x, c_coord_min._d_y);
	glVertex2f(c_coord_min._d_x, c_coord_max._d_y);
	glVertex2f(c_coord_max._d_x, c_coord_max._d_y);
	glVertex2f(c_coord_max._d_x, c_coord_min._d_y);
	glVertex2f(c_coord_min._d_x, c_coord_min._d_y);
	glEnd();


	for (ppc_way = _list_way.begin(); ppc_way != _list_way.end(); ppc_way++) {
		CT_OSM_WAY* pc_way = (*ppc_way).second;

		enum ET_DRAW_TYPE {
			E_DRAW_TYPE_LINE, E_DRAW_TYPE_POLYGON, E_DRAW_TYPE_NONE,
		};
		int e_type = E_DRAW_TYPE_LINE;
		short i_line_pattern = 0xFFFF;
		char const * pc_color = "#a0a0a0";

		if (pc_way->f_get_tag("natural") == "coastline") {
			e_type = E_DRAW_TYPE_LINE;
			i_line_pattern = 0xCCCC;
			pc_color = "purple";

		} else if (pc_way->f_get_tag("natural") == "scrub") {
			e_type = E_DRAW_TYPE_POLYGON;
			pc_color = "#8dc56c";
		} else if (pc_way->f_get_tag("landuse") == "forest") {
			e_type = E_DRAW_TYPE_POLYGON;
			pc_color = "green";
		} else if (pc_way->f_get_tag("landuse") == "quarry") {
			e_type = E_DRAW_TYPE_POLYGON;
			pc_color = "grey";
		} else if (pc_way->f_get_tag("boundary") != "") {
			e_type = E_DRAW_TYPE_NONE;
		}

		switch (e_type) {
		case E_DRAW_TYPE_LINE:
			m_f_glColor(pc_color);
			glEnable(GL_LINE_STIPPLE);
			glLineStipple(1, i_line_pattern);
#if 0
			glBegin(GL_LINE_STRIP);
			for (list<CT_OSM_NODE*>::iterator ppc_node =
					pc_way->f_get_node_list()->begin(); ppc_node
					!= pc_way->f_get_node_list()->end(); ppc_node++) {
				CT_OSM_NODE* pc_node = *ppc_node;
				glVertex2d(pc_node->c_coord._d_x, pc_node->c_coord._d_y);
			}
			glEnd();
#else
			{
				bool b_line_started = false;
				CT_OSM_NODE* pc_last = NULL;
				for (list<CT_OSM_NODE*>::iterator ppc_node =
						pc_way->f_get_node_list()->begin(); ppc_node
						!= pc_way->f_get_node_list()->end(); ppc_node++) {
					CT_OSM_NODE* pc_node = *ppc_node;

					if((pc_node->c_coord._d_lat < in_f_lat_max)
							&& (pc_node->c_coord._d_lat > in_f_lat_min)
							&& (pc_node->c_coord._d_lon < in_f_lon_max)
							&& (pc_node->c_coord._d_lon > in_f_lon_min)) {
						if(!b_line_started) {
							glBegin(GL_LINE_STRIP);
							if(pc_last) {
								glVertex2d(pc_last->c_coord._d_x, pc_last->c_coord._d_y);
							}
							b_line_started = true;
						}

						glVertex2d(pc_node->c_coord._d_x, pc_node->c_coord._d_y);
					} else {
						if(b_line_started) {
							glVertex2d(pc_node->c_coord._d_x, pc_node->c_coord._d_y);
							glEnd();
							b_line_started = false;
						} else {
							pc_last = pc_node;
						}
					}

				}
				if(b_line_started) {
					glEnd();
				}
			}
#endif

			break;
		case E_DRAW_TYPE_POLYGON: {
			m_f_glColor(pc_color);

			CT_OSM_NODE* pc_node_first = *(pc_way->f_get_node_list()->begin());

			//c_tess.m_f_glBegin();
			c_tess.m_f_glBegin();
			for (list<CT_OSM_NODE*>::iterator ppc_node =
					pc_way->f_get_node_list()->begin(); ppc_node
					!= pc_way->f_get_node_list()->end(); ppc_node++) {
				CT_OSM_NODE* pc_node = *ppc_node;
				if (pc_node_first->c_coord._d_x != pc_node->c_coord._d_x) {
					c_tess.m_f_vertex2dv(&(pc_node->c_coord._d_x));
				}
			}
			c_tess.m_f_glEnd();

		}

			break;

		default:
			break;
		}

	}

}

void CT_OSMGL::f_parse_xml_rules(xmlNode * a_node) {
	for (xmlNode * cur_node = a_node; cur_node; cur_node = cur_node->next) {
		if (cur_node->type == XML_ELEMENT_NODE) {

			string str_name = (const char*) cur_node->name;
			printf("%s\n", str_name.c_str());
			if (str_name == "Rule") {
				/* Parse tags */
				for (xmlNode * ps_child_node = cur_node->children; ps_child_node; ps_child_node
						= ps_child_node->next) {
					if (ps_child_node->type == XML_ELEMENT_NODE) {
						string str_child_name =
								(const char*) ps_child_node->name;
						if (str_child_name == "Filter") {
							printf("%s", ps_child_node->content);
						}
					}
				}
			}
		}
	}
}

void CT_OSMGL::f_parse_xml(xmlNode * a_node) {
	for (xmlNode * cur_node = a_node; cur_node; cur_node = cur_node->next) {
		if (cur_node->type == XML_ELEMENT_NODE) {

			string str_name = (const char*) cur_node->name;
			if (str_name == "node") {
				char const * str_tmp;

				str_tmp = (char const *) xmlGetProp(cur_node,
						(const xmlChar*) "id");
				uint32_t i_id = atoi(str_tmp);
				str_tmp = (char const *) xmlGetProp(cur_node,
						(const xmlChar*) "lat");
				float f_lat = atof(str_tmp);
				str_tmp = (char const *) xmlGetProp(cur_node,
						(const xmlChar*) "lon");
				float f_lon = atof(str_tmp);

				// printf("New node %d %f %f\n",i_id,f_lat,f_lon);

				CT_OSM_NODE * pc_node = new CT_OSM_NODE(i_id, f_lat, f_lon);
				_list_node[i_id] = pc_node;

				/* Parse tags */
				for (xmlNode * ps_child_node = cur_node->children; ps_child_node; ps_child_node
						= ps_child_node->next) {
					if (ps_child_node->type == XML_ELEMENT_NODE) {
						string str_child_name =
								(const char*) ps_child_node->name;
						if (str_child_name == "tag") {
							string str_key = (char const *) xmlGetProp(
									ps_child_node, (const xmlChar*) "k");
							string str_value = (char const *) xmlGetProp(
									ps_child_node, (const xmlChar*) "v");
							pc_node->f_add_tag(str_key, str_value);
							//			printf("\ttags[%s]:%s \n",str_key.c_str(), str_value.c_str());
						}
					}
				}

			} else if (str_name == "way") {
				char const * str_tmp;

				str_tmp = (char const *) xmlGetProp(cur_node,
						(const xmlChar*) "id");
				uint32_t i_id = atoi(str_tmp);
				//printf("New way %d\n", i_id);

				CT_OSM_WAY * pc_way = new CT_OSM_WAY(i_id);
				_list_way[i_id] = pc_way;

				/* Parse tags */
				for (xmlNode * ps_child_node = cur_node->children; ps_child_node; ps_child_node
						= ps_child_node->next) {
					if (ps_child_node->type == XML_ELEMENT_NODE) {
						string str_child_name =
								(const char*) ps_child_node->name;
						if (str_child_name == "tag") {
							string str_key = (char const *) xmlGetProp(
									ps_child_node, (const xmlChar*) "k");
							string str_value = (char const *) xmlGetProp(
									ps_child_node, (const xmlChar*) "v");
							pc_way->f_add_tag(str_key, str_value);
							/*printf("\ttags[%s]:%s \n", str_key.c_str(),
							 str_value.c_str());*/
						} else if (str_child_name == "nd") {
							str_tmp = (char const *) xmlGetProp(ps_child_node,
									(const xmlChar*) "ref");
							uint32_t i_ref = atoi(str_tmp);

							if (_list_node.find(i_ref) == _list_node.end()) {
								fprintf(stderr, "ERRPR ref[%d] not found \n",
										i_ref);
								throw EC_OSMGL_FAILURE;
							}
							//printf("\tref[%d]:%p \n", i_ref, _list_node[i_ref]);

							pc_way->f_add_node(_list_node[i_ref]);
						}
					}
				}
			} else if (str_name == "bounds") {
				//  <bounds minlat="43.1159000" minlon="5.5564000" maxlat="43.2120000" maxlon="5.6822000"/>
				char const * str_tmp;
				str_tmp = (char const *) xmlGetProp(cur_node,
						(const xmlChar*) "minlat");
				_f_minlat = atof(str_tmp);
				str_tmp = (char const *) xmlGetProp(cur_node,
						(const xmlChar*) "minlon");
				_f_minlon = atof(str_tmp);
				str_tmp = (char const *) xmlGetProp(cur_node,
						(const xmlChar*) "maxlat");
				_f_maxlat = atof(str_tmp);
				str_tmp = (char const *) xmlGetProp(cur_node,
						(const xmlChar*) "maxlon");
				_f_maxlon = atof(str_tmp);

			} else {
				f_parse_xml(cur_node->children);
			}
		}
	}
}

void CT_OSMGL::f_open_rules(char const * in_str_filename) {
	xmlDoc *doc = NULL;
	xmlNode *root_element = NULL;

	/*parse the file and get the DOM */
	doc = xmlReadFile(in_str_filename, NULL, 0);

	if (doc == NULL) {
		fprintf(stderr, "osmgl: could not parse file %s\n", in_str_filename);
		throw EC_OSMGL_FAILURE;
	}

	/*Get the root element nodEC_OSM_FAILUREe */
	root_element = xmlDocGetRootElement(doc);

	f_parse_xml_rules(root_element);

	/*free the document */
	xmlFreeDoc(doc);

	/*
	 *Free the global variables that may
	 *have been allocated by the parser.
	 */
	xmlCleanupParser();
}

/**
 * Simple example to parse a file called "file.xml",
 * walk down the DOM, and print the name of the
 * xml elements nodes.
 */
CT_OSMGL::CT_OSMGL(char const * in_str_filename) {

#ifdef FF_PARSERONLY
	int yyparse(void);
	yyparse();
	exit(EXIT_FAILURE);
#endif

	xmlDoc *doc = NULL;
	xmlNode *root_element = NULL;

	_str_filename = in_str_filename;

	/*
	 * this initialize the library and check potential ABI mismatches
	 * between the version it was compiled for and the actual shared
	 * library used.
	 */
	LIBXML_TEST_VERSION

	//f_open_rules("./rules/mapnik/osm.xml");

	/*parse the file and get the DOM */
	doc = xmlReadFile(_str_filename.c_str(), NULL, 0);

	if (doc == NULL) {
		fprintf(stderr, "osmgl: could not parse file %s\n",
				_str_filename.c_str());
		throw EC_OSMGL_FAILURE;
	}

	/*Get the root element nodEC_OSM_FAILUREe */
	root_element = xmlDocGetRootElement(doc);

	f_parse_xml(root_element);

	/*free the document */
	xmlFreeDoc(doc);

	/*
	 *Free the global variables that may
	 *have been allocated by the parser.
	 */
	xmlCleanupParser();

}

class CT_PARSER {
	int i_type;
	void * data;
};

