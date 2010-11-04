/**
 * section: Tree
 * synopsis: Navigates a tree to print element names
 * purpose: Parse a file to a tree, use xmlDocGetRootElement() to
 *          get the root element, then walk the document and print
 *          all the element name in document order.
 * usage: tree1 filename_or_URL
 * test: tree1 test2.xml > tree1.tmp ; diff tree1.tmp tree1.res ; rm tree1.tmp
 * author: Dodji Seketeli
 * copy: see Copyright for the status of this software.
 */
#include <iostream>
#include <libxml/parser.h>
#include <libxml/tree.h>

#include <osmgl/elements.hh>
#include <osmgl/osmgl.hh>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>         /* glut.h includes gl.h and glu.h*/
#endif

#ifndef LIBXML_TREE_ENABLED
#error "libxml2 must have tree enabled"
#endif
void CT_OSMGL::f_render(float in_f_lat_min,float in_f_lat_max,  float in_f_lon_min,
		 float in_f_lon_max) {
	CT_OSM_COORD c_coord_min(in_f_lat_min, in_f_lon_min);
	CT_OSM_COORD c_coord_max(in_f_lat_max, in_f_lon_max);
	map<uint32_t, CT_OSM_WAY*>::iterator ppc_way;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(c_coord_min._d_x, c_coord_max._d_x, c_coord_min._d_y,
			c_coord_max._d_y, -1.0, 1.0);

	glMatrixMode(GL_MODELVIEW);
	/*
	 glBegin(GL_POLYGON);
	 glVertex2f(c_coord_min._d_x, c_coord_min._d_y);
	 glVertex2f(c_coord_min._d_x, c_coord_max._d_y);
	 glVertex2f(c_coord_max._d_x, c_coord_max._d_y);
	 glVertex2f(c_coord_max._d_x, c_coord_min._d_y);
	 glEnd();
	 */

	for (ppc_way = _list_way.begin(); ppc_way != _list_way.end(); ppc_way++) {
		CT_OSM_WAY* pc_way = (*ppc_way).second;
		if(pc_way->f_get_tag("natural") == "coastline") {
			/* Found a coast */
			//CT_OSM_WAY* pc_coast =
		}
	}

	for (ppc_way = _list_way.begin(); ppc_way != _list_way.end(); ppc_way++) {
		CT_OSM_WAY* pc_way = (*ppc_way).second;
		list<CT_OSM_NODE*>::iterator ppc_node;
		//	glColor3f(0.0,0.0,0.0);
		glBegin(GL_LINE_STRIP);
		for (ppc_node = pc_way->f_get_node_list()->begin(); ppc_node
				!= pc_way->f_get_node_list()->end(); ppc_node++) {
			CT_OSM_NODE* pc_node = *ppc_node;
			/*printf("%f, %f (%f,%f) (%f,%f)\n", pc_node->c_coord._d_x,
					pc_node->c_coord._d_y, c_coord_min._d_x, c_coord_max._d_x,
					c_coord_min._d_y, c_coord_max._d_y);*/
			glVertex2d(pc_node->c_coord._d_x, pc_node->c_coord._d_y);

		}
		glEnd();

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

			} else {
				f_parse_xml(cur_node->children);
			}
		}
	}
}

/**
 * Simple example to parse a file called "file.xml",
 * walk down the DOM, and print the name of the
 * xml elements nodes.
 */
CT_OSMGL::CT_OSMGL(string in_str_filename) {
	xmlDoc *doc = NULL;
	xmlNode *root_element = NULL;

	_str_filename = in_str_filename;

	/*
	 * this initialize the library and check potential ABI mismatches
	 * between the version it was compiled for and the actual shared
	 * library used.
	 */
	LIBXML_TEST_VERSION

	/*parse the file and get the DOM */
	doc = xmlReadFile(in_str_filename.c_str(), NULL, 0);

	if (doc == NULL) {
		printf("error: could not parse file %s\n", in_str_filename.c_str());
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
#if 0
bool
CT_OSMGL::m_f_match(std::string in_str_src) {

	string str_tmp;

	str_tmp
}
#endif
#if 0
bool
CT_OSMGL::m_f_match_or(std::string in_str_string) {
	if(in_str_string.size() < 4) {
		throw EC_OSMGL_BYPASS;
	}

	if(in_str_string[0] != '(') {
		throw EC_OSMGL_BYPASS;
	}

	if(in_str_string[in_str_string.size()-1] != ')') {
		throw EC_OSMGL_BYPASS;
	}

	/* Match exp inside () */
	try {
		return m_f_match_exp(in_str_string.substr(1,in_str_string.size()-2));
	} catch (ec) {
		if(ec == EC_OSMGL_FAILURE) {
			throw ec;
		}
	}

	throw EC_OSMGL_BYPASS;
}

bool
CT_OSMGL::m_f_match_pp(std::string in_str_string) {
	if(in_str_string.size() < 4) {
		throw EC_OSMGL_BYPASS;
	}

	if(in_str_string[0] != '(') {
		throw EC_OSMGL_BYPASS;
	}

	if(in_str_string[in_str_string.size()-1] != ')') {
		throw EC_OSMGL_BYPASS;
	}

	/* Match exp inside () */
	try {
		return m_f_match_exp(in_str_string.substr(1,in_str_string.size()-2));
	} catch (ec) {
		if(ec == EC_OSMGL_FAILURE) {
			throw ec;
		}
	}

	throw EC_OSMGL_BYPASS;
}

bool
CT_OSMGL::m_f_match_exp(std::string in_str_rule) {

	/* Match (x) */
	try {
		return m_f_match_pp(in_str_rule);
	} catch (ec) {
		if(ec == EC_OSMGL_FAILURE) {
			throw ec;
		}
	}

	/* Match xxx or xxx */
	try {
		return m_f_match_or(in_str_rule);
	} catch (ec) {
		if(ec == EC_OSMGL_FAILURE) {
			throw ec;
		}
	}

	/* Match xxx and xxx */
	try {
		return m_f_match_and(in_str_rule);
	} catch (ec) {
		if(ec == EC_OSMGL_FAILURE) {
			throw ec;
		}
	}

	/* Match xxx */
	try {
		return m_f_match_key_value(in_str_rule);
	} catch (ec) {
		if(ec == EC_OSMGL_FAILURE) {
			throw ec;
		}
	}

	throw EC_OSMGL_BYPASS;
}
#endif
CT_OSMGL * gpc_osm = NULL;
void display(void)

{
	/* clear window */

	glClear(GL_COLOR_BUFFER_BIT);

	/* draw unit square polygon */
	gpc_osm->f_render(43.0508269, 43.2632063, 5.465698199999999,
			5.847473099999999);
	/*
	 glBegin(GL_POLYGON);
	 glVertex2f(-0.5, -0.5);
	 glVertex2f(-0.5, 0.5);
	 glVertex2f(0.5, 0.5);
	 glVertex2f(0.5, -0.5);
	 glEnd();
	 */
	/* flush GL buffers */

	glFlush();

}

void init() {

	/* set clear color to black */

	/* 	glClearColor (0.0, 0.0, 0.0, 0.0); */
	/* set fill  color to white */

	/* 	glColor3f(1.0, 1.0, 1.0); */

	/* set up standard orthogonal view with clipping */
	/* box as cube of side 2 centered at origin */
	/* This is default view and these statement could be removed */

	/* */

}

int main(int argc, char** argv) {

	/* Initialize mode and open a window in upper left corner of screen */
	/* Window title is name of program (arg[0]) */
	try {
		gpc_osm = new CT_OSMGL(argv[1]);
	} catch (int ec) {
		cerr << "A failure as occured during init";
		exit(-1);
	}

	/* You must call glutInit before any other OpenGL/GLUT calls */
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("simple");
	glutDisplayFunc(display);
	init();
	glutMainLoop();

}

