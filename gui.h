#ifndef GTKMM_GUI_H
#define GTKMM_GUI_H

// gui.h : interface du module qui gère les intéractions utilisateur, déclaration de 
//		   sous-classe MyArea de Gtk::DrawingArea qui gère la partie dessin de la 
//		   fenêtre. Déclartion de la sous-classe Gui de Gtk::Window qui gère les 
//		   différents boutons d'interactions du Modèle avec l'utilisateur.
// Russias Laetitia et El Mouhtar William 
// version 1

#include <gtkmm/window.h>
#include <gtkmm/box.h>
#include <gtkmm/button.h>
#include <gtkmm/drawingarea.h>
#include <gtkmm/label.h>
#include <gtkmm/separator.h>
#include <gtkmm/checkbutton.h>
#include "graphic_gui.h"
#include "graphic.h"
#include "simulation.h"
#include <gtkmm/filechooserdialog.h>
#include <string>

constexpr unsigned taille_dessin(500); 

//pour déterminer le comportement de on_file_dialog_response
enum Dialog_response {
	SAVE,
	OPEN
};

class Monde : public Gtk::DrawingArea {
public:
	Monde (Simulation& sim);
	virtual ~Monde();
	void step();

protected:
	void on_draw(const Cairo::RefPtr<Cairo::Context>& cr, int width, int height);
	Simulation* pt_simulation;

};

class Gui : public Gtk::Window {
public:
	Gui(Simulation sim);

protected:
	Simulation simulation;
	void on_button_clicked_exit();
	void on_button_clicked_open();
	void on_button_clicked_save();
	void on_button_clicked_start();
	void on_button_clicked_step();
	void step();
	void maj_boutons();
	void on_button_toggled_naissance();
	bool on_window_key_pressed(guint keyval, guint keycode, Gdk::ModifierType state); 
	void on_file_dialog_response(int response_id, Gtk::FileChooserDialog* dialog);
	bool on_timeout();
	Monde m_Monde;
	Gtk::Separator m_Separator1, m_Separator2;
	Gtk::Box m_Main_Box;
	Gtk::Box m_Dialogue;
	Gtk::Box m_Buttons_Box;
	Gtk::Box m_Info;
	Gtk::Label m_Label_General;
	Gtk::Label m_Label_Info;
	Gtk::Label m_Label_Nb_Maj;
	Gtk::Label m_Label_Nb_Algue;
	Gtk::Label m_Label_Nb_Corail;
	Gtk::Label m_Label_Nb_Sca;
	Gtk::Button m_Button_Exit;
	Gtk::Button m_Button_Open;
	Gtk::Button m_Button_Save;
	Gtk::Button m_Button_Start;
	Gtk::Button m_Button_Step;
	Gtk::CheckButton m_Button_Naissance;
	const int timeout_value;
	//booléen qui est vrai quand le bouton doit afficher start, faux si stop
	bool start;
	Dialog_response dialog_rep;
};

#endif // GTKMM_MY_EVENT_H
