#ifndef SHAPE_H
#define SHAPE_H

// shape.h : déclaration de structures pour modéliser des formes dans le plan, est
//			 indépendante du modèle mais contient une constante qui vise à avoir 
//			 plus de tolérance dans les tests d'intersection
// Russias Laetitia et El Mouhtar William
// version 3

#include "graphic.h"

constexpr double epsil_zero = 0.5;

struct S2d {
	double x;
	double y;
	S2d (double x = 0., double y = 0.) : x(x), y(y) {}
	S2d& operator-=(const S2d& autre_vec);
	bool operator==(const S2d& autre_pt) const;
};

struct Segment {
	Segment(S2d base, int longueur=0, double angle =0.);
	Segment(S2d base, S2d extremite);
	Segment() {}
	S2d base; 
	double angle = 0.;
	unsigned int longueur = 0;
	S2d calcul_extremite();
	S2d extremite;
	bool operator==(const Segment& autre_seg) const;
};

struct Carre {
	Carre (S2d milieu, int cote);
	S2d milieu;
	int cote;
	S2d a, b, c, d;
	Segment s1, s2, s3, s4;
};

double ecart_angulaire (Segment, Segment);
void draw_circle(double line_width, double r,double g, double b, S2d coord, 
				 double rayon);
void draw_segment(double line_width, double r,double g, double b, Segment segment);
void draw_square(double line_width, double r,double g, double b, Carre carre);
bool superposition(Segment seg1, Segment seg2);
bool do_intersect(S2d p1, S2d q1, S2d p2, S2d q2, bool lecture);
bool do_intersect (Segment s1, Segment s2, bool lecture=false);
double ecart_angulaire_base (Segment seg1, Segment seg2);
unsigned int distance(S2d p1, S2d p2);
void angle_dans_intervalle(double& angle);
#endif
