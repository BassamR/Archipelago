/**
* \name projet.cc
* \author Hugo Masson, Bassam El Rawas (Sciper 314886, 310635)
* \date May 2020
* \version 3.0
*/

//Architecture: Fig 11 b1

#include <gtkmm/application.h>
#include "ville.h"
#include "gui.h"

int main(int argc, char* argv[]) {
    int fakeArgc = 1; //tricks Gtk into thinking argc=1
    auto app = Gtk::Application::create(fakeArgc, argv, "org.gtkmm.example");

    if(argc == 2) lecture(argv[1]);

    Gui archipelagoGui;

    return app->run(archipelagoGui);
}