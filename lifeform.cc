// lifeform.cc : définition des méthodes des classes de lifeform, setters pour afficher
//				 les messages d'erreur, getters pour récupérer les informations des 
//				 entités, des methodes de mise à jour et de dessin des entités.
// Russias Laetitia 50% et El Mouhtar William 50%
// version 3

#include <iostream>
#include <cmath>
#include "message.h"
#include "shape.h"
#include <vector>
#include "lifeform.h"
#include "constantes.h"

using namespace std;

Lifeform::Lifeform(double x, double y, unsigned int a) : coord(x,y), age(a) {}
Lifeform::Lifeform(double x, double y, unsigned int a, bool& succes_lecture) {
	set_age(a, succes_lecture);
	set_coord(x,y, succes_lecture);
}
	
void Lifeform::set_age(int a, bool& succes_lecture) {
	if(a <= 0){
		cout << message::lifeform_age (a);
		succes_lecture = false; 
	}
	else age = a;
}
	
void Lifeform::set_coord(double x, double y, bool& succes_lecture) {
	if(x < 1. or x > dmax-1. or y < 1. or y > dmax-1.) {
		cout << message::lifeform_center_outside(x,y);
		succes_lecture = false; 
	}
	else {
		coord.x = x;
		coord.y = y;
	}
}
S2d Lifeform::get_coord () const {
	return coord;
}

unsigned int Lifeform::get_age () const {
	return age;
}

void Lifeform::incrementation_age() {
	++age;
}


Algue::Algue (double x, double y, unsigned int a) :  Lifeform (x, y, a) {}

Algue::Algue(double x, double y, unsigned int a, bool& succes_lecture) :  
	Lifeform (x, y, a, succes_lecture){}


void Algue::dessine() const {
	draw_circle(1, 0., 0.8, 0., coord,r_alg);
}

Corail::Corail (double x, double y, unsigned int a, int id, bool etat, bool sensRot, 
				bool repro, vector<Segment> seg_cor) : Lifeform (x, y, a), id(id){
		set_vie(etat);
		set_sens(sensRot);
		set_dev(repro);
}
			
Corail::Corail(double x, double y, unsigned int age, int id, bool etat, 
			   bool sensRot, bool repro, vector<Corail> tab_coraux, 
			   bool& succes_lecture) : Lifeform (x, y, age, succes_lecture) {
	set_id(id, tab_coraux, succes_lecture);
	set_vie(etat);
	set_sens(sensRot);
	set_dev(repro);
}

Corail::Corail (Corail parent, Segment premier_segment, vector<Corail> les_coraux) {
	age = 1;
	id = nouvel_id(les_coraux);
	coord = premier_segment.base;
	vie = ALIVE;
	sens = parent.sens;
	dev = EXTEND;
	segments_corail.push_back(premier_segment);
}

void Corail::incrementation_age() {
	Lifeform::incrementation_age();
	if (age >= max_life_cor) vie = DEAD;
}

void Corail::set_id(int identifiant, vector<Corail> tab_coraux, bool& succes_lecture) {
	bool deja_present(false);
	for (size_t i=0; i< tab_coraux.size(); ++i) {
		if(identifiant == tab_coraux[i].id) {
			deja_present = true;
		}
	}
	if (deja_present) {
		cout << message::lifeform_duplicated_id(identifiant);
		succes_lecture = false; 
	}
	else id = identifiant;
}

void Corail::set_vie(bool etat) {
	if (etat) vie = ALIVE;
	else vie = DEAD;	
}
	
void Corail::set_sens (bool sens_rot) {
	if (sens_rot) sens = INVTRIGO;
	else sens = TRIGO;
}
	
int Corail::get_id ()const {
	return id;
}
	
Status_cor Corail::get_statut_cor () {
	return vie;
}

int Corail::get_vie() const {
	return vie;
}

int Corail::get_sens() const {
	return sens;
}

int Corail::get_dev() const {
	return dev;
}

int Corail::get_nb_segments() const {
	return segments_corail.size();
}

vector<Segment> Corail::get_segments_corail() const {
	return segments_corail;
}

S2d Corail::get_coord_effecteur() {
	return segments_corail[segments_corail.size()-1].extremite;
}


void Corail::set_dev(bool repro) {
	if (repro) dev = REPRO;
	else dev = EXTEND;
}

void Corail::set_sca_trouve(bool est_cible) {
	sca_trouve = est_cible;
}

// set_segments ajoute un segment à un corail après avoir vérifier les conditions 
// d'existance de ce segment
void Corail::set_segments(unsigned int l, double a, vector<Corail> tab_coraux, 
						  bool lecture, bool& succes_lecture) {
	Segment seg;
	if (segments_corail.size() == 0) {
		seg.base = coord;
	}
	else {
		seg.base = segments_corail[(segments_corail.size()-1)].extremite;
	}
	if ((l < l_repro-l_seg_interne) or (l >= l_repro)){
		cout << message::segment_length_outside(id, l);
		succes_lecture=false;
	}
	else {
		seg.longueur = l;
	}
	if((a < -1 * M_PI) or (a > M_PI)) {
		cout << message::segment_angle_outside(id,a);
		succes_lecture= false; 
	}
	else {
		seg.angle = a;
	}
	S2d extr = seg.calcul_extremite();
	if (lecture) {
		if(extr.x <= 0. or extr.x >= dmax or extr.y <= 0. or extr.y >= dmax) {
			cout << message::lifeform_computed_outside(id, extr.x, extr.y);
			succes_lecture= false; 
		}
	}
	else {
		if (extr.x <= epsil_zero or extr.x >= dmax-epsil_zero 
			or extr.y <= epsil_zero or extr.y >= dmax-epsil_zero) {
				cout << message::lifeform_computed_outside(id, extr.x, extr.y);
				succes_lecture= false; 
			}
	}
	seg.extremite = extr;
	verifie_segment(seg, tab_coraux, lecture, succes_lecture);
	segments_corail.push_back(seg);
}
	
// verifie_segment est appelée par set_segments pour vérifier la superposition et la 
// collision avec les autres coraux et les segments précédants du même corail.
void Corail::verifie_segment(Segment seg, vector<Corail> tab_coraux, bool lecture, 
							 bool& succes_lecture) {
	if (segments_corail.size() > 0) {
			if (superposition(seg,segments_corail[segments_corail.size()-1])){
				cout << message::segment_superposition(id, segments_corail.size()-1, 
				segments_corail.size());
				succes_lecture= false;
			}
	}
	for (size_t i(0); i< tab_coraux.size(); ++i) {
		for (size_t j(0); j< tab_coraux[i].segments_corail.size(); ++j) {
			if (do_intersect(tab_coraux[i].segments_corail[j], seg, lecture)) {
					unsigned int id1 = tab_coraux[i].id;
					unsigned int id2 = id;
					unsigned int index1 = j;
					unsigned int index2 = segments_corail.size();
					cout << message::segment_collision(id1, index1, id2, index2);
					succes_lecture= false;
			}
		}
	}
	if (segments_corail.size() > 1) {
		for (size_t i(0); i<(segments_corail.size()-1); ++i) {
			if (do_intersect(segments_corail[i], seg, lecture)) {
				unsigned int index1 = i;
				unsigned int index2 = segments_corail.size();
				cout << message::segment_collision(id, index1, id, index2);
				succes_lecture= false;
			}
		}
	}
}

void Corail::dessine() const {
	Carre base_cor(segments_corail[0].base, d_cor);
	if (vie == ALIVE) {
		draw_square(1, 0.,0., 0.8, base_cor);
		for (int i(0); i<get_nb_segments(); ++i) {
			draw_segment(1, 0.,0., 0.8, segments_corail[i]);
		}
	}
	else {
		draw_square(1, 0.5, 0.5, 0.5, base_cor);
		for (int i(0); i<get_nb_segments(); ++i) {
			draw_segment(1, 0.5, 0.5, 0.5, segments_corail[i]);
		}
	}
}

//nouvel_id renvoie un id qui n'est pas deja utilisé
int Corail::nouvel_id(std::vector<Corail> les_coraux) {
	bool id_trouve(false);
	int i(0);
	while (!id_trouve) {
		for (size_t j(0); j<les_coraux.size(); ++j) {
			if (les_coraux[j].id==i) {
				++i;
				break;
			}
			if (j==les_coraux.size()-1) id_trouve=true;
		}
	}
	return i;
}

bool Corail::superposition_seg() {
	if (segments_corail.size()>1) {
		if (fabs(ecart_angulaire(segments_corail[segments_corail.size()-1], 
			segments_corail[segments_corail.size()-2]))<=delta_rot) {
			return true;
		}
	}
	return false;	
	
}
bool Corail::segment_sortant(Segment seg) {
	if (seg.extremite.x <= epsil_zero or seg.extremite.x >= dmax-epsil_zero 
		or seg.extremite.y <= epsil_zero or seg.extremite.y >= dmax-epsil_zero) {
		return true;
	}
	return false;
}

bool Corail::intersection_segments(vector<Corail> les_coraux, Segment moving_seg) {
	for (size_t i(0); i< les_coraux.size(); ++i) {
		for (size_t j(0); j< les_coraux[i].segments_corail.size(); j++) {
			if (les_coraux[i].segments_corail[j]==moving_seg or 
				les_coraux[i].segments_corail[j].extremite==moving_seg.base) continue;
			if (do_intersect(les_coraux[i].segments_corail[j], moving_seg, true)) {
				return true;
			}
		}
	}
	return false;
}

//recherche_algue_proche renvoie le rang du scavenger le plus proche dans le tableau
int Corail::recherche_algue_proche(vector<Algue> les_algues) {
	Segment seg_corail(segments_corail[segments_corail.size()-1]);
	int num_algue_proche(0); 
	double ecart_algue_proche;
	Segment seg_algue(seg_corail.base, les_algues[0].get_coord());
	if (sens==TRIGO) {
		ecart_algue_proche = ecart_angulaire_base(seg_corail, seg_algue);
	}
	else if (sens==INVTRIGO) {
		ecart_algue_proche=ecart_angulaire_base(seg_algue, seg_corail);
	}
	for (size_t i(1); i< les_algues.size(); ++i) {
		Segment seg_algue(seg_corail.base, les_algues[i].get_coord());
		if (sens == TRIGO) { 
			if (ecart_algue_proche<0
				and ecart_angulaire_base(seg_corail,seg_algue)>0){ 
				ecart_algue_proche = ecart_angulaire_base(seg_corail, seg_algue);
				num_algue_proche = i;
			}
			if (ecart_angulaire_base(seg_corail, seg_algue) < ecart_algue_proche and
				ecart_angulaire_base(seg_corail, seg_algue) > 0 and 
				seg_algue.longueur<seg_corail.longueur) {
				ecart_algue_proche = ecart_angulaire_base(seg_corail, seg_algue);
				num_algue_proche = i;
			}
		}
		else if (sens == INVTRIGO) {
			if (ecart_algue_proche<0 and ecart_angulaire_base(seg_algue,seg_corail)>0){ 
				ecart_algue_proche = ecart_angulaire_base(seg_algue, seg_corail);
				num_algue_proche = i;
			}
			if (ecart_angulaire_base(seg_algue, seg_corail) < ecart_algue_proche and
				ecart_angulaire_base(seg_algue, seg_corail) > 0 and 
				seg_algue.longueur<seg_corail.longueur) {
				ecart_algue_proche = ecart_angulaire_base(seg_algue, seg_corail);
				num_algue_proche = i;
			}
		}
	}
	return num_algue_proche;
}

//sca_proche renvoie le rang du scavenger le plus proche dans le tableau
int Corail::sca_proche(vector<Scavenger> les_sca) {
	S2d cor= segments_corail[segments_corail.size()-1].extremite;
	double distance_sca = distance(les_sca[0].get_coord(), cor);
	int num_cor_proche = 0;
	for (size_t i(0); i<les_sca.size(); ++i) {
		if (les_sca[num_cor_proche].get_etat()==EATING and 
			les_sca[i].get_etat()==FREE){
			num_cor_proche = i;
			distance_sca = distance(les_sca[i].get_coord(), cor);
		}
		else if (distance_sca > distance(les_sca[i].get_coord(), cor) and 
				 les_sca[i].get_etat()==FREE){
			num_cor_proche = i;
			distance_sca = distance(les_sca[i].get_coord(), cor);
		}		
	}
	return num_cor_proche;
}

//corail_mange est appelé par mange_corail pour réduire la taille du corail
void Corail::corail_mange() {
	if (segments_corail.back().longueur <= delta_l) {
		segments_corail.pop_back();
	}
	else{
		segments_corail.back().longueur-=delta_l;
		segments_corail.back().calcul_extremite();
	}
}

//step_dead_cor gère la recherche d'un scavenger proche pour que le corail mort soit 
//une cible 
void Corail::step_dead_cor(vector<Scavenger>& les_sca) {
	if (les_sca.size() > 0) {
		if (not sca_trouve) {
			int num_sca(sca_proche(les_sca));
			if (les_sca[num_sca].get_etat()==FREE) {
				les_sca[num_sca].set_etat(id);
				sca_trouve=true;
			}
		}
	}
}

//rotation_sans_algue gère les rotation de +/- delta_rot
void Corail::rotation_sans_algue(double angle_rot, Segment& moving_seg, 
								 vector<Corail>& les_coraux) {
	bool intersection(false);
	bool superposition(false);
	bool en_dehors(false);
	moving_seg.angle+= angle_rot;
	angle_dans_intervalle(moving_seg.angle);
	moving_seg.calcul_extremite();
	intersection = intersection_segments(les_coraux, moving_seg);
	superposition = superposition_seg();
	en_dehors = segment_sortant(moving_seg);
	if (superposition or intersection or en_dehors) {
		if (sens==INVTRIGO) sens = TRIGO;
		else sens = INVTRIGO;
		moving_seg.angle-= angle_rot;
		angle_dans_intervalle(moving_seg.angle);
		moving_seg.calcul_extremite();
	}
	
}

void Corail::reproduction(Segment& moving_seg, vector<Corail>& les_coraux, 
						  bool& intersection, bool& en_dehors) {
	moving_seg.longueur = l_repro-l_seg_interne;
	S2d nouvelle_base = moving_seg.extremite;
	moving_seg.longueur = l_repro/2;
	Segment premier_seg(nouvelle_base, l_repro-l_seg_interne, moving_seg.angle);
	if (not intersection and intersection_segments(les_coraux, premier_seg)) {
		intersection = true; 
		moving_seg.longueur = l_repro;
	}
	else if (not en_dehors and segment_sortant(premier_seg)) {
		en_dehors=true;
		moving_seg.longueur = l_repro;
	}
	else { 
		Corail nouveau_corail(*this, premier_seg, les_coraux);
		les_coraux.push_back(nouveau_corail);
		dev = EXTEND;
	}
}

void Corail::extension(Segment& moving_seg, vector<Corail>& les_coraux, 
					   bool& intersection, bool& en_dehors) {
	moving_seg.longueur = l_seg_interne;
	Segment nouveau_seg(moving_seg.extremite, l_repro-l_seg_interne, moving_seg.angle);
	if (not intersection and intersection_segments(les_coraux, nouveau_seg)) {
		intersection = true; 
		moving_seg.longueur = l_repro;
	}
	else if (segment_sortant(nouveau_seg)) {
		en_dehors=true;
		moving_seg.longueur = l_repro;
	}
	else {
		segments_corail.push_back(nouveau_seg);
		dev = REPRO;
	}
}

//effectue une rotation de taille adapté à l'algue la plus proche et gère la EXTEND et
//REPRO du corail en appelant reproduction et extension au besoin
void Corail::rotation_avec_algue(double delta, Segment& moving_seg, 
								 vector<Corail>& les_coraux, vector<Algue>& les_algues, 
								 int num_algue_mange) {
	bool intersection(false);
	bool superposition(false);
	bool en_dehors(false);
	moving_seg.angle+=delta;
	angle_dans_intervalle(moving_seg.angle);
	moving_seg.longueur+=delta_l;
	moving_seg.calcul_extremite();
	intersection = intersection_segments(les_coraux, moving_seg);
	superposition = superposition_seg();
	en_dehors = segment_sortant(moving_seg);
	if (moving_seg.longueur >= l_repro and !superposition and !en_dehors and
		!intersection) { 
		if(dev == REPRO) {
			reproduction(moving_seg, les_coraux, intersection, en_dehors);
		}
		else if(dev==EXTEND) {
			extension(moving_seg, les_coraux, intersection, en_dehors);
		}
	}
	if (superposition or intersection or en_dehors) {
		if (sens==INVTRIGO) {
			sens = TRIGO;
		}
		else {
			sens = INVTRIGO;
		}
		moving_seg.angle -= delta;
		angle_dans_intervalle(moving_seg.angle);
		moving_seg.longueur-= delta_l;
		moving_seg.calcul_extremite();
	}
	else {
		swap(les_algues[num_algue_mange], les_algues.back());
		les_algues.pop_back();
	}
}

//etape pour corail vivant : appel les fonctions de rotation du corail
void Corail::step_alive_cor(vector<Corail>& les_coraux, vector<Algue>& les_algues) {
	double delta(M_PI);
	int num_algue_mange(0);
	double distance(40);
	bool zero_algue(false);
	if(les_algues.size()==0) zero_algue=true;
	Segment& moving_seg(segments_corail[segments_corail.size()-1]);
	if (! zero_algue) {
		num_algue_mange = recherche_algue_proche(les_algues);
		Segment seg_algue(moving_seg.base, les_algues[num_algue_mange].get_coord());
		delta=ecart_angulaire_base(moving_seg, seg_algue);
		distance = seg_algue.longueur;
	}
	if (fabs(delta) > delta_rot or (sens == TRIGO and delta < 0) or (sens == INVTRIGO 
	and delta > 0) or zero_algue or distance > moving_seg.longueur) {
		switch (sens) {
			case TRIGO: {
				rotation_sans_algue(delta_rot, moving_seg, les_coraux);
				break;
			}
			case INVTRIGO: {
				rotation_sans_algue(-delta_rot, moving_seg, les_coraux);
				break;
			}
		}
	}
	else {
		rotation_avec_algue(delta, moving_seg, les_coraux,les_algues, num_algue_mange);
	}
}

void Corail::incrementation_angle(vector<Corail>& les_coraux,vector<Algue>& les_algues,
								  vector<Scavenger>& les_sca) {
	switch (vie) {
		case DEAD:
			step_dead_cor(les_sca);
			break;
		case ALIVE:
			step_alive_cor(les_coraux, les_algues);
			break;
		}
}

Scavenger::Scavenger(double x, double y, unsigned int a, bool mange, double r, 
					 int corail_cible) : Lifeform (x, y, a), rayon(r), 
										 corail_id_cible(corail_cible) {}
		  
Scavenger::Scavenger(double x, double y, unsigned int a, bool mange, double r, 
					 vector<Corail>tab_coraux, int corail_cible, bool& succes_lecture):
	Lifeform (x, y, a, succes_lecture) {
	set_etat(mange, tab_coraux, corail_cible, succes_lecture);
	set_rayon (r, succes_lecture);
}	

//création d'un scavenger pendant la simulation
Scavenger::Scavenger(S2d coordonnee) {
	coord=coordonnee;
	age = 1;
	rayon = r_sca;
	etat = FREE;
}
	 
void Scavenger::set_etat (bool mange, vector <Corail>& tab_coraux, int corail_cible, 
						  bool& succes_lecture) {
	if (mange) {
		bool corail_trouve(false);
		for (size_t i (0); i< tab_coraux.size() ; ++i) {
			if (corail_cible == tab_coraux[i].get_id() and 
				tab_coraux[i].get_statut_cor() == DEAD) {
				corail_trouve = true;
				tab_coraux[i].set_sca_trouve(true); 
				break;
			}
		}
		if (corail_trouve) {
			etat = EATING;
			corail_id_cible = corail_cible;
		}
		else {
			cout << message::lifeform_invalid_id(corail_cible);
			succes_lecture= false;
		}
	}	 
	else etat = FREE;
}

void Scavenger::set_etat(int id) {
	etat = EATING;
	corail_id_cible = id;
}

void Scavenger::set_rayon(double r, bool& succes_lecture) {
	if ((r <r_sca) or (r >= r_sca_repro)) {
		cout << message::scavenger_radius_outside(r);
		succes_lecture= false; 
	}
	else rayon = r;
}

int Scavenger::get_rayon() const{
	return rayon;
}

int Scavenger::get_etat() const{
	return etat;
}

int Scavenger::get_corail_id_cible() const{
	return corail_id_cible;
}

void Scavenger::dessine() const {
	draw_circle(1, 0.8, 0., 0., coord, rayon);
}

void Scavenger::deplacement(vector<Corail>& les_coraux, 
							vector<Scavenger>& les_scavengers) {
	if (etat == EATING) {
		size_t num_cor_cible;
		for (size_t i(0); i<les_coraux.size(); ++i) {
			if(les_coraux[i].get_id()==corail_id_cible) num_cor_cible = i;
		}
		if (coord == les_coraux[num_cor_cible].get_coord_effecteur()){
			mange_corail (num_cor_cible, les_coraux, les_scavengers);
		}
		else {
			if (distance(coord, les_coraux[num_cor_cible].get_coord_effecteur()) 
				<= delta_l) {
				coord = les_coraux[num_cor_cible].get_coord_effecteur();
			}
			else {
				Segment sca_to_cor(coord, 
								   les_coraux[num_cor_cible].get_coord_effecteur());
				sca_to_cor.longueur=delta_l;
				sca_to_cor.calcul_extremite();
				coord = sca_to_cor.extremite;
			}
		}
	}
}

void Scavenger::grandi(vector<Scavenger>& les_scavengers, Segment seg_cor) {
	rayon += delta_r_sca; 
			if (rayon >= r_sca_repro) {
				rayon = r_sca;
				Segment sca_to_new_sca(coord, delta_l, -seg_cor.angle);
				Scavenger nouveau_sca(sca_to_new_sca.extremite);
				les_scavengers.push_back(nouveau_sca);
			}
}

//mange_corail appel corail_mange pour diminuer la taille du corail si besoin
void Scavenger::mange_corail(size_t num_cor_cible, vector<Corail>& les_coraux, 
							 vector<Scavenger>& les_scavengers) {
	Segment seg_cor = les_coraux[num_cor_cible].get_segments_corail().back();
	if (distance(coord, les_coraux[num_cor_cible].get_segments_corail().back().base) 
		<= delta_l){
		coord = les_coraux[num_cor_cible].get_coord_effecteur();
		seg_cor.longueur = 0;
		coord = seg_cor.base;
		les_coraux[num_cor_cible].corail_mange();
		if (les_coraux[num_cor_cible].get_segments_corail().size() == 0) {
			swap(les_coraux[num_cor_cible], les_coraux.back());
			les_coraux.pop_back();
			etat = FREE;
		}
	}
	else {
		seg_cor.longueur-=delta_l;
		seg_cor.calcul_extremite();
		coord = seg_cor.extremite;
		les_coraux[num_cor_cible].corail_mange();
	}
	grandi(les_scavengers, seg_cor);
}
