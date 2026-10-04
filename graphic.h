#ifndef GTKMM_EXAMPLE_GRAPHIC_H
#define GTKMM_EXAMPLE_GRAPHIC_H

// graphi.h : interface partielle de graphic contenue dans shape pour permettre les 
//			  de faire les fonction de dessin sur le plan
// Russias Laetitia et El Mouhtar William
// version 1
void graphic_draw_circle(double line_width, double r, double g, double b, double x, 
						 double y, double rayon);
void graphic_draw_segment(double line_width, double r, double g, double b, double x_i,
						  double y_i, double x_f, double y_f);


#endif // GTKMM_EXAMPLE_GRAPHIC_H

