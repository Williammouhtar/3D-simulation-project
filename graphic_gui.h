#ifndef GTKMM_EXAMPLE_GRAPHIC_GUI_H
#define GTKMM_EXAMPLE_GRAPHIC_GUI_H

// graphi_gui.h : interface complète de graphic contenue dans gui qui met à jour le 
//				  pointeur ptcr Cairo::Context cr et l'utilise pour les demandes de 
//				  dessin
// Russias Laetitia et El Mouhtar William 
// version 1

#include <gtkmm/drawingarea.h>
#include "graphic.h"

void graphic_set_context(const Cairo::RefPtr<Cairo::Context>& cr);

#endif // GTKMM_EXAMPLE_GRAPHIC_GUI_H
