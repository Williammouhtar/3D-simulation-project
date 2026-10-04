// projet.cc : point d'entrée du Modèle, déclare une simulation et lit le fichier de 
//			   et crée une fenêtre Gui et lui transmet la simulation lue.
// Russias Laetitia et El Mouhtar William
// version 1

#include <iostream>
#include "simulation.h"
#include "gui.h"
#include <gtkmm/application.h>

using namespace std;

int main(int argc, char* argv[]) {
	
	Simulation simulation;
	simulation.lecture(argv[1]);
	auto app = Gtk::Application::create();
	return app->make_window_and_run<Gui>(1, argv, simulation);
	
}	
	
