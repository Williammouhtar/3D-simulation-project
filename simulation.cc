// simulation.cc : définitions des méthodes de simulations comprenant la lecture, la
//				   sauvegarde de fichiers. une méthode de mise à jour et de dessin
// Russias Laetitia 50% et El Mouhtar William 50%
// version 3

#include <locale>
#include <iostream>
#include "message.h"
#include <string>
#include "lifeform.h"
#include <fstream>
#include <sstream>
#include <vector>
#include "constantes.h"
#include "simulation.h"

using namespace std;

//écriture de la simulation en cours du même type que le fichier en entrée
void Simulation::sauvegarde(string filename) const {
    ofstream etat_simulation(filename.c_str());
    if(etat_simulation) {
		etat_simulation.imbue(locale::classic());
		etat_simulation << les_algues.size() << endl;
		for (size_t i(0); i< les_algues.size(); ++i) {
			etat_simulation << les_algues[i].get_coord().x << " " 
			<< les_algues[i].get_coord().y << " " << les_algues[i].get_age() << endl;
		}
		etat_simulation << les_coraux.size() << endl;
		for (size_t i(0); i< les_coraux.size(); ++i) {
			etat_simulation << les_coraux[i].get_coord().x << " " 
			<< les_coraux[i].get_coord().y << " " << les_coraux[i].get_age() <<" " 
			<<les_coraux[i].get_id()<<" " << les_coraux[i].get_vie() << " " <<
			les_coraux[i].get_sens() << " " << les_coraux[i].get_dev() << " " <<
			les_coraux[i].get_nb_segments() << endl;
			
			for (size_t j(0); j<les_coraux[i].get_segments_corail().size(); ++j) {
				etat_simulation<< les_coraux[i].get_segments_corail()[j].angle <<" " <<
				les_coraux[i].get_segments_corail()[j].longueur<< endl;
			}
		}
		etat_simulation << les_scavengers.size() << endl;
		for (size_t i(0); i< les_scavengers.size(); ++i) {
			etat_simulation << les_scavengers[i].get_coord().x << " " 
			<< les_scavengers[i].get_coord().y << " " << les_scavengers[i].get_age() <<
			" "<<les_scavengers[i].get_rayon()<<" "<< les_scavengers[i].get_etat() << 
			" "<< les_scavengers[i].get_corail_id_cible()<< endl;
		}
    }
}

//lit le fichier en entrée est surchargé pour l'ouverture d'un fichier depuis le Gui
void Simulation::lecture(char * nom_fichier) {
    string line;
    ifstream fichier(nom_fichier); 
    lecture(fichier, line);
}
void Simulation::lecture(string nom_fichier) {
	ifstream nouveau_fichier(nom_fichier.c_str());
    string line;
    lecture(nouveau_fichier, line);
}
void Simulation::lecture(ifstream& fichier, string& line) {
	succes_lecture=true;
	reinitialisation();
	e.seed(1);
    if(!fichier.fail()) {
		lire_algues(fichier, line);
		lire_coraux(fichier, line);
		lire_scavengers(fichier, line);
		if (succes_lecture) cout << message::success();
		else reinitialisation();
	}
	fichier.close();
}

Algue Simulation::lire_une_algue(ifstream& file, string& line) {
	do {
		getline(file>>ws, line);
	} while (line[0]=='#');
	double x(0.);
	double y(0.);
	int age(0);
	istringstream(line)>> x >> y >> age;
	Algue une_algue(x, y, age, succes_lecture);
	return une_algue;
}

Corail Simulation::lire_un_corail(ifstream& file, string& line) {
	do {
		getline(file>>ws, line);
	} while (line[0]=='#');
	double x(0.);
	double y(0.);
	int age(0);
	int id(0);
	bool vie(0);
	bool sens_rot(0);
	bool dev(0);
	int nb_segment(0);
	istringstream(line) >> x >> y >> age >> id >> vie >> sens_rot >> dev >> nb_segment;
	Corail un_corail(x, y, age, id, vie, sens_rot, dev, les_coraux, succes_lecture);
	for (int i=0; i<nb_segment; ++i) {
		do {
			getline(file>>ws, line);
		} while (line[0]=='#');
		double angle;
		int longueur;
		istringstream(line) >> angle >> longueur;
		un_corail.set_segments(longueur, angle, les_coraux, true, succes_lecture);
	}
	return un_corail;
}

Scavenger Simulation::lire_un_scavenger(ifstream& file, string& line) {
	do {
		getline(file>>ws, line);
	} while (line[0]=='#');
	double x(0.);
	double y(0.);
	int age(0.);
	int rayon(0);
	bool etat(0);
	int corail_cible(0);
	istringstream(line) >> x >> y >> age >> rayon >> etat >> corail_cible;
	Scavenger un_scavenger(x, y, age, etat, rayon, les_coraux, corail_cible, 
						   succes_lecture);
	return un_scavenger;
}

void Simulation::lire_algues(ifstream& fichier_a_lire, string& line) {
	do {
		getline(fichier_a_lire>>ws, line);
	} while (line[0]=='#');
	int nb_algues(0);
	istringstream(line) >> nb_algues;
	for (int i(0); i<nb_algues; ++i) {
		Algue une_algue = lire_une_algue(fichier_a_lire, line);
		les_algues.push_back(une_algue);
	}
}

void Simulation::lire_coraux(ifstream& fichier_a_lire, string& line) {
	do {
		getline(fichier_a_lire>>ws, line);
	} while (line[0]=='#');
	int nb_cor(0);
	istringstream(line) >> nb_cor;
	for (int i(0); i< nb_cor; ++i) {
		Corail un_corail = lire_un_corail(fichier_a_lire, line);
		les_coraux.push_back(un_corail);
	}
}

void Simulation::lire_scavengers(ifstream& fichier_a_lire, string& line) {
	do {
		getline(fichier_a_lire>>ws, line);
	} while (line[0]=='#');
	int nb_scavengers(0);
	istringstream(line) >> nb_scavengers;
	for (int i(0); i<nb_scavengers; ++i) {
		Scavenger un_sca = lire_un_scavenger(fichier_a_lire, line);
		les_scavengers.push_back(un_sca);
	}
	
}

//reinitialisation permet de remettre la simulation vide en cas de lecture de nouveau
//fichier ou de detection d'erreur
void Simulation::reinitialisation() {
	les_algues={};
	les_coraux={};
	les_scavengers={};
	nombre_maj=0;
	
}

int Simulation::get_nombre_maj() const {
	return nombre_maj;
}

int Simulation::get_nombre_algue() const {
	return les_algues.size();
}

int Simulation::get_nombre_corail() const {
	return les_coraux.size();
}

int Simulation::get_nombre_scavengers() const {
	return les_scavengers.size();
}

//fonction qui modifie la simulation d'une mise à jour
void Simulation::step() {
	++nombre_maj;
	for (size_t i(0); i< les_algues.size(); ++i) {
		les_algues[i].incrementation_age();
		if (les_algues[i].get_age() >= max_life_alg) {
			swap(les_algues[i], les_algues.back());
			les_algues.pop_back();
			--i;
		}
	}
	for (size_t i(0); i< les_coraux.size(); ++i) {
		les_coraux[i].incrementation_age();
		les_coraux[i].incrementation_angle(les_coraux, les_algues, les_scavengers);
	}
	for (size_t i(0); i< les_scavengers.size(); ++i) {
		les_scavengers[i].incrementation_age();
		if (les_scavengers[i].get_age() >= max_life_sca) {
			swap(les_scavengers[i], les_scavengers.back());
			les_scavengers.pop_back();
		}
		les_scavengers[i].deplacement(les_coraux, les_scavengers);
	}
	if (naissance_algue) {
		bernoulli_distribution b(alg_birth_rate); 
		if(b(e)) {
			uniform_int_distribution<unsigned> u(1,dmax-1);
			int x = u(e);
			int y = u(e);
			Algue nouvelle_algue(x, y, 1);
			les_algues.push_back(nouvelle_algue);
		}
	}
}

void Simulation::set_naissance_algue(bool naissance) {
	naissance_algue=naissance;
}

void Simulation::dessine() {
	S2d centre(dmax/2, dmax/2);
	Carre cadre(centre, dmax);
	draw_square(1, 0.5, 0.5, 0.5, cadre);
	for (size_t i(0); i< les_algues.size(); ++i) {
		les_algues[i].dessine();
	}
	for (size_t i(0); i< les_coraux.size(); ++i) {
		les_coraux[i].dessine();
	}
	for (size_t i(0); i< les_scavengers.size(); ++i) {
		les_scavengers[i].dessine();
	}
}

bool Simulation::get_naissance_algue() const { 
	return naissance_algue; 
}
