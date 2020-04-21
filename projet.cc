/**
* \name projet.cc
* \author Hugo Masson, Bassam El Rawas (Sciper 314886, 310635)
* \date Mars 2020
* \version 1.0
*/

#include <iostream>
#include <gtkmm/application.h>
#include "ville.h"
#include "gui.h"
using namespace std;

int main(int argc, char* argv[]) {
    int fakeArgc = 1; //tricks Gtk into thinking argc=1
    auto app = Gtk::Application::create(fakeArgc, argv, "org.gtkmm.example");

    if(argc == 2) lecture(argv[1]);

    Gui archipelagoGui;

    return app->run(archipelagoGui);
}