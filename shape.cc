// shape.cc : définitions de fonctions utiles au modèle 2D et de dessin des formes.
// Russias Laetitia 50% et El Mouhtar William 50%
// version 3

#include "shape.h"
#include <iostream>
#include <cmath>
#include "graphic.h"
using namespace std;

bool on_segment(S2d, S2d, S2d);
int orientation(S2d, S2d, S2d, bool);

Segment::Segment(S2d base, int l, double a) : base(base), angle(a), longueur(l) {
	calcul_extremite();
}

Segment::Segment(S2d p1, S2d p2) : base(p1), extremite(p2) {
	if (extremite.x >= base.x) {
		angle = atan((extremite.y-base.y)/(extremite.x-base.x));
	}
	else {
		angle = M_PI + atan((extremite.y-base.y)/(extremite.x-base.x));
		if (angle>M_PI) angle = angle - 2*M_PI;
	}
	longueur = sqrt((extremite.y-base.y) * (extremite.y-base.y)+
					(extremite.x-base.x) * (extremite.x-base.x));
}

S2d& S2d::operator-=(const S2d& autre_vec) {
		x-=autre_vec.x;
		y-=autre_vec.y;
		return *this;
}
	
bool S2d::operator==(const S2d& autre_pt) const {
	if (x==autre_pt.x and y == autre_pt.y) return true;
	return false;
}

bool Segment::operator==(const Segment& autre_seg) const {
	if (base == autre_seg.base and extremite==autre_seg.extremite) {
		return true;
	}
	else return false;
}

unsigned int distance(S2d p1, S2d p2) {
	return sqrt((p1.x-p2.x)*(p1.x-p2.x)+(p1.y-p2.y)*(p1.y-p2.y));
}
S2d Segment::calcul_extremite() {
	double x_extr = base.x + longueur*cos(angle);
	double y_extr = base.y + longueur*sin(angle);
	S2d extr(x_extr, y_extr);
	extremite=extr;
	return extr;
}

//ecart_angulaire renvoie l'écart d'angle compris entre [-Pi, Pi] de deux segments
//ayant une base et une extremité en commun
double ecart_angulaire(Segment seg1, Segment seg2) {
	double a = M_PI - seg2.angle + seg1.angle;
	if (a<(-M_PI)) a = 2*M_PI + a;
	if (a>M_PI) a = a - 2*M_PI;
	return a; 
}

//ecart_angulaire_base renvoie l'écart d'angle compris entre [-Pi, Pi] de deux segments
//ayant la meme base
double ecart_angulaire_base (Segment seg1, Segment seg2) {
	double a = seg2.angle - seg1.angle;
	if (a<(-M_PI)) a = 2*M_PI + a;
	if (a>M_PI) a = a - 2*M_PI;
	return a; 
}

bool superposition(Segment seg1, Segment seg2) {
	if (ecart_angulaire(seg1, seg2) == 0.) return true;
	return false;
}

bool on_segment(S2d p, S2d q, S2d r) { 
    if (q.x <= max(p.x, r.x) && q.x >= min(p.x, r.x) && 
        q.y <= max(p.y, r.y) && q.y >= min(p.y, r.y)) 
       return true; 
  
    return false; 
} 
  
int orientation(S2d p, S2d q, S2d r, bool lecture) { 
    double val = (q.y - p.y) * (r.x - q.x) - 
              (q.x - p.x) * (r.y - q.y); 
	val /= sqrt(pow(q.x-p.x, 2) + pow(q.y-p.y, 2));
	if (lecture) {
		if (fabs(val) < epsil_zero ) return 0;  
	}
	else {
		if (val == 0) return 0; 
	}
	return (val > 0)? 1: 2; 
} 

// do_intersect prend un booléen faux par défaut en paramêtre pour avoir ou non une 
// tolérance d'epsil_zero
bool do_intersect(S2d p1, S2d q1, S2d p2, S2d q2, bool lecture=false) { 
    int o1 = orientation(p1, q1, p2, lecture); 
    int o2 = orientation(p1, q1, q2, lecture); 
    int o3 = orientation(p2, q2, p1, lecture); 
    int o4 = orientation(p2, q2, q1, lecture); 
  
    if (o1 != o2 && o3 != o4) return true; 
    if (o1 == 0 && on_segment(p1, p2, q1)) return true; 
    if (o2 == 0 && on_segment(p1, q2, q1)) return true; 
    if (o3 == 0 && on_segment(p2, p1, q2)) return true; 
    if (o4 == 0 && on_segment(p2, q1, q2)) return true; 
  
    return false; 
} 

bool do_intersect (Segment s1, Segment s2, bool lecture) {
	return do_intersect(s1.base, s1.extremite, s2.base, s2.extremite, lecture);
}

void draw_circle(double line_width, double r,double g, double b, S2d coord, 
				 double rayon) {
	graphic_draw_circle(line_width, r, g, b, coord.x, coord.y, rayon);
}
void draw_segment(double line_width, double r,double g, double b, Segment segment) {
	graphic_draw_segment(line_width, r, g, b, segment.base.x, segment.base.y, 
						 segment.extremite.x, segment.extremite.y);
}

Carre::Carre (S2d milieu, int cote) : milieu(milieu), cote(cote) {
	a.x=milieu.x-cote/2.;
	a.y=milieu.y-cote/2.;
	b.x=milieu.x-cote/2.;
	b.y=milieu.y+cote/2.;
	c.x=milieu.x+cote/2.;
	c.y=milieu.y-cote/2.;
	d.x=milieu.x+cote/2.;
	d.y=milieu.y+cote/2.;
	s1.base=a;
	s1.extremite=b;
	s2.base=b;
	s2.extremite=d;
	s3.base=d;
	s3.extremite=c;
	s4.base=c;
	s4.extremite=a;
}

void draw_square(double line_width, double r,double g, double b, Carre carre) {

	draw_segment(line_width, r, g, b, carre.s1);
	draw_segment(line_width, r, g, b, carre.s2);
	draw_segment(line_width, r, g, b, carre.s3);
	draw_segment(line_width, r, g, b, carre.s4);
}

void angle_dans_intervalle (double& angle) {
	if (angle<(-M_PI)) angle += 2*M_PI;
	if (angle>M_PI) angle -=  2*M_PI;
}
