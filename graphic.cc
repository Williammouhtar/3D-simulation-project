// graphic.cc : définitions des fonctions de dessin faisant des appels directs à Gtk
//				qui utilisent le pointeur ptcr pour les demandes de dessin.
// Russias Laetitia 50% et El Mouhtar William 50%
// version 1
#include "graphic_gui.h"
#include <cmath>
static const Cairo::RefPtr<Cairo::Context>* ptcr(nullptr); 
void graphic_set_context(const Cairo::RefPtr<Cairo::Context>& cr) {
	ptcr = &cr;
}

//les paramêtre r, g, b gèrent la couleur : valeur attendue dans [0;1]
void graphic_draw_circle(double line_width, double r, double g, double b, double x, 
						 double y, double rayon) {
	(*ptcr)->set_line_width(line_width);
	(*ptcr)->set_source_rgb(r,g,b);
	(*ptcr)->arc(x, y, rayon, -M_PI, M_PI);
	(*ptcr)->stroke();
}

//les paramêtre r, g, b gèrent la couleur : valeur attendue dans [0;1]
void graphic_draw_segment(double line_width, double r, double g, double b, double x_i, 
						  double y_i, double x_f, double y_f) {
	(*ptcr)->set_line_width(line_width);
	(*ptcr)->set_source_rgb(r,g,b);
	(*ptcr)->move_to(x_i, y_i);
	(*ptcr)->line_to(x_f, y_f);
	(*ptcr)->stroke();
}
