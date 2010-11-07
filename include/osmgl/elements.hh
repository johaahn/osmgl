/*
 * osm_element.hh
 *
 *  Created on: 3 juin 2010
 *      Author: johaahn
 */

#ifndef OSM_ELEMENT_HH_
#define OSM_ELEMENT_HH_

#include <map>
#include <list>
#include <string>
#include <math.h>
#include <stdint.h>
using namespace std;

class CT_OSM_COORD {
public:
		double _d_lat;
		double _d_lon;
		double _d_x;
		double _d_y;
		double _d_z;

		CT_OSM_COORD(double in_d_lat, double in_d_long) {
			_d_lat = in_d_lat;
			_d_lon = in_d_long;

			_d_x = _d_lon * 6371000.0 * M_PI / 180.0;
			_d_y = log(tan(M_PI_4 + _d_lat * M_PI / 360.0)) * 6371000.0;

			/* Convert Lat lont */
			/*
			cout << float(_d_lon) << endl;
			cout << float(_d_lat) << endl;
			cout << float(_d_x) << endl;
			cout << float(_d_y) << endl;
			*/
			//_f_x = _f_lat/(2.0f*M_PI);
			//_f_y = _f_long/(2.0f*M_PI);
		}


		bool is_in(CT_OSM_COORD & in_c_min, CT_OSM_COORD & in_c_max) {
			if((in_c_min._d_lat < _d_lat) && (in_c_min._d_lat > _d_lat) && (in_c_max._d_lon < _d_lon) && (in_c_max._d_lon > _d_lon)) {
				return true;
			} else {
				return false;
			}
		}
};

class CT_OSM_NODE {
	uint32_t _i_id;


	map<string,string> _m_tags;
public:

	CT_OSM_COORD c_coord;

	CT_OSM_NODE(uint32_t i_id, double f_lat, double f_long);
	void f_add_tag(string &in_str_name, string &in_str_value);
};

CT_OSM_NODE::CT_OSM_NODE(uint32_t in_i_id, double in_f_lat, double in_f_long) :
c_coord(in_f_lat, in_f_long){
	_i_id = in_i_id;


}

void
CT_OSM_NODE::f_add_tag(string &in_str_name, string &in_str_value) {
	_m_tags[in_str_name] = in_str_value;
}

class CT_OSM_WAY {
	uint32_t _i_id;
	list<CT_OSM_NODE*> _l_nodes;
	map<string,string> _m_tags;
public:
	CT_OSM_WAY(uint32_t in_i_id);
	void f_add_tag(string &in_str_name, string &in_str_value);
	string f_get_tag(char const * in_str_name);
	void f_add_node(CT_OSM_NODE* in_pc_node);
	list<CT_OSM_NODE*> * f_get_node_list(void) {return &_l_nodes;}

};

CT_OSM_WAY::CT_OSM_WAY(uint32_t in_i_id) {
	_i_id = in_i_id;
}

string CT_OSM_WAY::f_get_tag(char const * in_str_name) {
	map<string,string>::iterator it;
	it = _m_tags.find(in_str_name);
	if(it == _m_tags.end()) {
		return "";
	} else {
		return _m_tags[in_str_name];
	}
}

void
CT_OSM_WAY::f_add_tag(string &in_str_name, string &in_str_value) {
	_m_tags[in_str_name] = in_str_value;
}

void
CT_OSM_WAY::f_add_node(CT_OSM_NODE* in_pc_node) {
	_l_nodes.push_back(in_pc_node);
}

#endif /* OSM_ELEMENT_HH_ */
