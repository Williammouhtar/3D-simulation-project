// gui.cc : définitions des constructeurs de Monde et Gui, de la méthode on_draw pour
//			représenter le Modèle graphiquement et des méthodes de réactions de la 
//			simulation avec les boutons.
// Russias Laetitia 50% et El Mouhtar William 50%
// version 1

#include "gui.h"
#include <cairomm/context.h>
#include <iostream>
#include "graphic_gui.h"
#include <gtkmm.h>
#include <gtkmm/filechooserdialog.h>

//Construction d'un Monde qui est initialisé en pointant sur une simulation(de Gui)
Monde::Monde(Simulation& sim): pt_simulation(&sim) {
	set_content_width(taille_dessin);
	set_content_height(taille_dessin);
	set_draw_func(sigc::mem_fun(*this, &Monde::on_draw));
}

Monde::~Monde() {
	delete pt_simulation;
}

void Monde::step() {
	queue_draw();
}

void Monde::on_draw(const Cairo::RefPtr<Cairo::Context>& cr, int width, int height) {
	graphic_set_context(cr);
	if (width>=height) {
		double delta_y=dmax;
		double delta_x=delta_y*width/height;
		cr->translate(width/2,height/2);
		cr->scale(width/delta_x, -height/delta_y);
		cr->translate(-dmax/2, -dmax/2);
	} else {
		double delta_x=dmax;
		double delta_y=delta_x*height/width;
		cr->translate(width/2,height/2);
		cr->scale(width/delta_x, -height/delta_y);
		cr->translate(-dmax/2, -dmax/2);
	}
	pt_simulation->dessine();
	
}

//constructeur de Gui initialisé avec la simulation en paramêtre
Gui::Gui(Simulation sim):
	simulation(sim),
	m_Monde(simulation),
	m_Main_Box(Gtk::Orientation::HORIZONTAL, 1),
	m_Dialogue(Gtk::Orientation::VERTICAL, 1),
	m_Buttons_Box(Gtk::Orientation::VERTICAL, 2),
	m_Info(Gtk::Orientation::VERTICAL, 2),
	m_Label_General("General"),
	m_Label_Info("Info : nombre de..."),
	m_Label_Nb_Maj("mise à jour: " + std::to_string(simulation.get_nombre_maj())),
	m_Label_Nb_Algue("algues: " + std::to_string(simulation.get_nombre_algue())),
	m_Label_Nb_Corail("corails: " + std::to_string(simulation.get_nombre_corail())),
	m_Label_Nb_Sca("charognards: "+std::to_string(simulation.get_nombre_scavengers())),
	m_Button_Exit("exit"),
	m_Button_Open("open"),
	m_Button_Save("save"),
	m_Button_Start("start"),
	m_Button_Step("step"),
	m_Button_Naissance("naissance d'algue"),
	timeout_value(25),
	start(true) {
		
	m_Monde.set_expand(true);
	set_title("Microrécif");
	set_child(m_Main_Box);
	m_Main_Box.append(m_Dialogue);
	m_Main_Box.append(m_Separator1);
	m_Main_Box.append(m_Monde);
	m_Dialogue.append(m_Buttons_Box);
	m_Dialogue.append(m_Separator2);
	m_Dialogue.append(m_Info);
	m_Buttons_Box.append(m_Label_General);
	m_Buttons_Box.append(m_Button_Exit);
	m_Buttons_Box.append(m_Button_Open);
	m_Buttons_Box.append(m_Button_Save);
	m_Buttons_Box.append(m_Button_Start);
	m_Buttons_Box.append(m_Button_Step);
	m_Buttons_Box.append(m_Button_Naissance);
	m_Info.append(m_Label_Info);
	m_Info.append(m_Label_Nb_Maj);
	m_Info.append(m_Label_Nb_Algue);
	m_Info.append(m_Label_Nb_Corail);
	m_Info.append(m_Label_Nb_Sca);
	m_Button_Exit.signal_clicked().connect(
		sigc::mem_fun(*this, &Gui::on_button_clicked_exit));
		
    m_Button_Open.signal_clicked().connect(
		sigc::mem_fun(*this, &Gui::on_button_clicked_open));
		
	m_Button_Save.signal_clicked().connect(
		sigc::mem_fun(*this, &Gui::on_button_clicked_save));
		
	m_Button_Start.signal_clicked().connect(
		sigc::mem_fun(*this, &Gui::on_button_clicked_start));
		
	m_Button_Step.signal_clicked().connect(
		sigc::mem_fun(*this, &Gui::on_button_clicked_step));
		
	m_Button_Naissance.signal_toggled().connect(
		sigc::mem_fun(*this, &Gui::on_button_toggled_naissance));
		
	auto controller = Gtk::EventControllerKey::create();
    controller->signal_key_pressed().connect(
                  sigc::mem_fun(*this, &Gui::on_window_key_pressed), false);
    add_controller(controller);	  
}

//mise à jour de la simulation 
void Gui::step() {
	simulation.step();
	m_Monde.step();
	maj_boutons();
}

void Gui::maj_boutons() {
	m_Label_Nb_Maj.set_label("mise à jour: " +
							 std::to_string(simulation.get_nombre_maj()));
	m_Label_Nb_Algue.set_label("algues: " +
							   std::to_string(simulation.get_nombre_algue()));
	m_Label_Nb_Corail.set_label("corails: " +
								std::to_string(simulation.get_nombre_corail()));
	m_Label_Nb_Sca.set_label("charognards: " + 
							 std::to_string(simulation.get_nombre_scavengers()));
}

void Gui::on_button_clicked_step() {
	if (start) {
		step();
	}
}

void Gui::on_button_clicked_save() {
	dialog_rep=SAVE;
	auto dialog = new Gtk::FileChooserDialog("Please choose a file",
		  Gtk::FileChooser::Action::SAVE);
	dialog->set_transient_for(*this);
	dialog->set_modal(true);
	dialog->signal_response().connect(sigc::bind(
	sigc::mem_fun(*this, &Gui::on_file_dialog_response), dialog));
	
	dialog->add_button("_Cancel", Gtk::ResponseType::CANCEL);
	dialog->add_button("_Save", Gtk::ResponseType::OK);
	
	auto filter_text = Gtk::FileFilter::create();
	filter_text->set_name("Text files");
	filter_text->add_mime_type("text/plain");
	dialog->add_filter(filter_text);
	
	dialog->show();
	
}

void Gui::on_button_clicked_open() {
	dialog_rep=OPEN;
	auto dialog = new Gtk::FileChooserDialog("Please choose a file",
		  Gtk::FileChooser::Action::OPEN);
	dialog->set_transient_for(*this);
	dialog->set_modal(true);
	dialog->signal_response().connect(sigc::bind(
	sigc::mem_fun(*this, &Gui::on_file_dialog_response), dialog));
	
	dialog->add_button("_Cancel", Gtk::ResponseType::CANCEL);
	dialog->add_button("_Open", Gtk::ResponseType::OK);
	
	auto filter_text = Gtk::FileFilter::create();
	filter_text->set_name("Text files");
	filter_text->add_mime_type("text/plain");
	dialog->add_filter(filter_text);
	
	dialog->show();
}

void Gui::on_file_dialog_response(int response_id, Gtk::FileChooserDialog* dialog) {
	switch (response_id){
		case Gtk::ResponseType::OK: {
		    auto filename = dialog->get_file()->get_path();
		    if (dialog_rep == SAVE) {
				simulation.sauvegarde(filename);
			}
			if (dialog_rep== OPEN) {
				simulation.lecture(filename);
				m_Monde.step();
				maj_boutons();
			}
		}
		case Gtk::ResponseType::CANCEL: {
		    break;
		}
		default: {
		    break;
		}
	}
	delete dialog;
}

bool Gui::on_window_key_pressed(guint keyval, guint, Gdk::ModifierType state) {
	switch(gdk_keyval_to_unicode(keyval)) {
		case 's':
			start=!start;
			on_button_clicked_start();
			on_button_clicked_start();
			return true;
		case '1':
			on_button_clicked_step();
			
			return true;
	}
return false;
}

void Gui::on_button_toggled_naissance() {
	if (not simulation.get_naissance_algue()) simulation.set_naissance_algue(true);
	else simulation.set_naissance_algue(false);
}

void Gui::on_button_clicked_start() {
	if (start) {
		start=false;
		m_Button_Start.set_label("stop");
		sigc::slot<bool()> my_slot = sigc::bind(sigc::mem_fun(*this,
												&Gui::on_timeout));
		auto conn = Glib::signal_timeout().connect(my_slot,timeout_value);
	}
	else {
		m_Button_Start.set_label("start");
		start = true;
	}
}

void Gui::on_button_clicked_exit() {
	hide();
}

bool Gui::on_timeout() {
	if(start) return false; 
	step();
	return true;
}
