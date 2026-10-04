#ifndef SIMULATION_H
#define SIMULATION_H

// simulation.h : déclaration de la classe simulation qui gère les différents ensemble 
//				  du Modèle ainsi que les sauvegardes et lectures de l'etat du modèle
// Russias Laetitia et El Mouhtar William
// version 2

#include <iostream>
#include "message.h"
#include <string>
#include "lifeform.h"
#include <fstream>
#include <sstream>
#include <vector>
#include <random>

class Simulation {
public :
	Simulation (bool lec=true, int nbmaj=0) : succes_lecture(lec), nombre_maj(nbmaj){};
	void lire_algues(std::ifstream& fichier_a_lire,std::string& line);
	void lire_coraux(std::ifstream& fichier_a_lire, std::string& line);
	void lire_scavengers(std::ifstream& fichier_a_lire, std::string& line);
	Algue lire_une_algue(std::ifstream& file, std::string& line);
	Corail lire_un_corail(std::ifstream& file, std::string& line);
	Scavenger lire_un_scavenger(std::ifstream& file, std::string& line);
	void lecture(char * nom_fichier);
	void lecture(std::ifstream& fichier, std::string& line);
	void lecture(std::string nom_fichier);
	void sauvegarde (std::string filename) const;
	int get_nombre_maj() const;
	int get_nombre_algue() const;
	int get_nombre_corail() const;
	int get_nombre_scavengers() const;
	void reinitialisation();
	void step();
	void dessine();
	void set_naissance_algue(bool naissance);
	bool get_naissance_algue() const;
	
private :
	std::vector<Algue> les_algues;
	std::vector<Corail> les_coraux;
	std::vector<Scavenger> les_scavengers;
	//devient faux quand il y a detection d'erreur
	bool succes_lecture;
	bool naissance_algue=false;
	//nombre de mise à jour
	int nombre_maj;
	//attribut pour gérer la notion de hasard réinitialisé à chaque lecture
	std::default_random_engine e;
};
#endif
