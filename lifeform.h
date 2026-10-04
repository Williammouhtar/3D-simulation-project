#ifndef LIFEFORM_H
#define LIFEFORM_H

// lifeform.h : déclaration d'une super-classe Lifeform et de trois sous-classes Algue,
//				Corail et Scavenger.
// Russias Laetitia et El Mouhtar William
// version 3

#include <vector>
#include "shape.h"
#include "constantes.h"

class Lifeform {
public : 
	Lifeform(double x=0, double y=0, unsigned int a=0);
	Lifeform (double x, double y, unsigned int a, bool& succes_lecture); 
	S2d get_coord () const;
	unsigned int get_age() const;
	void incrementation_age();
	
protected :
	void set_age(int a, bool& succes_lecture);
	void set_coord(double x, double y, bool& succes_lecture); 
	S2d coord;
	unsigned int age;
	void dessine() const;
};

class Algue : public Lifeform {
public :
	Algue (double x, double y, unsigned int a);
	Algue (double x, double y, unsigned int a, bool& succes_lecture); 
	void dessine() const;
};

class Scavenger;

class Corail : public Lifeform {
public : 
	Corail (double x, double y, unsigned int age, int id=0, bool etat=0, 
			bool sensRot=0, bool repro=0, std::vector<Segment> seg_cor={});
	Corail (Corail parent, Segment premier_segment, std::vector<Corail> les_coraux);
	Corail (double x, double y, unsigned int age, int id, bool etat, bool sensRot, 
			bool repro, std::vector<Corail> tab_coraux, bool& succes_lecture); 
	Status_cor get_statut_cor ();
	int get_id ()const;
	void set_segments (unsigned int l, double a, std::vector<Corail> tab_coraux, 
					   bool lecture, bool& succes_lecture);
	int get_vie() const;
	int get_sens() const;
	int get_dev() const;
	int get_nb_segments() const;
	std::vector<Segment> get_segments_corail() const;
	void dessine() const;
	void incrementation_angle(std::vector<Corail>& tab_coraux, 
							  std::vector<Algue>& les_algues,
							  std::vector<Scavenger>& les_sca);
	void incrementation_age();
	S2d get_coord_effecteur();
	void corail_mange();
	void set_sca_trouve(bool est_cible);
	
private :
	int id;
	Status_cor vie;
	Dir_rot_cor sens;
	Status_dev dev;
	std::vector<Segment> segments_corail;
	bool sca_trouve=false;
	void set_id(int identifiant, std::vector<Corail> tab_coraux, bool& succes_lecture);
	void set_vie(bool vie);
	void set_sens(bool sens_rot);
	void set_dev(bool repro);
	void verifie_segment(Segment seg, std::vector<Corail> tab_coraux, bool lecture, 
						 bool& succes_lecture);
	bool intersection_segments(std::vector<Corail> les_coraux, Segment moving_seg);
	bool superposition_seg();
	bool segment_sortant(Segment seg);
	int nouvel_id(std::vector<Corail> les_coraux);
	int recherche_algue_proche(std::vector<Algue> les_algues);
	int sca_proche(std::vector<Scavenger> les_sca);
	void step_dead_cor(std::vector<Scavenger>& les_sca);
	void step_alive_cor(std::vector<Corail>& les_coraux, 
						std::vector<Algue>& les_algues);
	void rotation_sans_algue(double angle_rot, Segment& moving_seg, 
						     std::vector<Corail>& les_coraux);
	void rotation_avec_algue(double delta, Segment& moving_seg, 
							 std::vector<Corail>& les_coraux, 
							 std::vector<Algue>& les_algues, int num_algue_mange);
	void reproduction(Segment& moving_seg, std::vector<Corail>& les_coraux, 
					  bool& intersection, bool& en_dehors);
	void extension(Segment& moving_seg, std::vector<Corail>& les_coraux, 
				   bool& intersection, bool& en_dehors);
};

class Scavenger : public Lifeform {
public :
	Scavenger(double x, double y, unsigned int a, bool mange=0, double r=0., 
			  int corail_cible=0);
	Scavenger(S2d coordonnee);
	Scavenger(double x, double y, unsigned int a, bool mange, double r, 
			  std::vector <Corail> tab_coraux, int corail_cible, bool& succes_lecture);
	int get_rayon() const;
	int get_etat()const;
	int get_corail_id_cible() const;
	void dessine() const;
	void set_etat(int id);
	void deplacement(std::vector <Corail>& les_coraux, std::vector<Scavenger>& 
					 les_scavengers);
private :
	unsigned int rayon;
	Status_sca etat;
	int corail_id_cible;
	void set_etat(bool mange, std::vector <Corail>& tab_coraux, int corail_cible, 
				  bool& succes_lecture);
	void set_rayon(double r, bool& succes_lecture);
	void grandi(std::vector<Scavenger>& les_scavengers, Segment seg_cor);
	void mange_corail(std::size_t num_cor_cible, std::vector<Corail>& les_coraux, 
					  std::vector<Scavenger>& les_scavengers);
};



#endif
