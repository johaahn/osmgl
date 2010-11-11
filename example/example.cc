/*
 * OsmGL example.
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


#include <stdlib.h>
#include <getopt.h>
#include <osmgl/osmgl.hh>
#include <iostream>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>         /* glut.h includes gl.h and glu.h*/
#endif

/* Config */
int config_verbose = 0;
char * config_map = NULL;

/* Renderer core */
CT_OSMGL * gpc_osm = NULL;

void display(void)

{


	/* draw unit square polygon */
	//gpc_osm->f_render();
	//  <bounds minlat="43.1159000" minlon="5.5564000" maxlat="43.2120000" maxlon="5.6822000"/>
	//
	float f_lon_min = 5.465698199999999;
	float f_lon_max = 5.847473099999999;
	float f_lat_min = 43.0508269;
	float f_lat_max = 43.2632063;


	CT_OSM_COORD c_coord_min(f_lat_min, f_lon_min);
	CT_OSM_COORD c_coord_max(f_lat_max, f_lon_max);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(c_coord_min._d_x, c_coord_max._d_x, c_coord_min._d_y,
			c_coord_max._d_y, -1.0, 1.0);

	glMatrixMode(GL_MODELVIEW);
	/* clear window */
	glClear(GL_COLOR_BUFFER_BIT);

	gpc_osm->f_render();
	//gpc_osm->f_render(43.150, 43.200, 5.56,5.67);
	//gpc_osm->f_render(43.0508269, 43.2632063, 5.465698199999999,5.847473099999999);

	/* flush GL buffers */
	glFlush();

}

/* Usage */
void usage(void) {
  fprintf(stderr, "example [options] [map.xml]\n");
  fprintf(stderr, "\t--debug \t\tEnable Debug\n");
}


int main(int argc, char** argv) {

  /* Getopt section */
  {
    struct option long_options[] =
    {
      /* These options set a flag. */
      {"verbose", no_argument,       &config_verbose, 1},
      {NULL, 0, 0, 0}
    };

    while (1)
    {
      int c;
      /* getopt_long stores the option index here. */
      int option_index = 0;

      c = getopt_long (argc, argv, "",
          long_options, &option_index);

      /* Detect the end of the options. */
      if (c == -1)
        break;

      switch (c)
      {
      case 0:
        /* If this option set a flag, do nothing else now. */
        if (long_options[option_index].flag != 0)
          break;
        printf ("option %s", long_options[option_index].name);
        if (optarg)
          printf (" with arg %s", optarg);
        printf ("\n");
        break;

      case '?':
      default:
        usage();
        exit(EXIT_FAILURE);
      }
    }

    if (config_verbose)
      puts ("verbose enabled");

    /* Print any remaining command line arguments (not options). */
    if (optind < argc)
    {
      while (optind < argc) {
        config_map = argv[optind++];
      }
    }

  }

  if(!config_map) {
		cerr << "No map provided";
                usage();
		exit(-1);
	}
	try {
		printf("OUOU\n");
		gpc_osm = new CT_OSMGL(config_map);
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

	glutMainLoop();

}

