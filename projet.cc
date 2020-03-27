/**
* \name projet.cc
* \author Hugo Masson, Bassam El Rawas (Sciper 314886, 310635)
* \date Mars 2020
* \version 1.0
*/

#include <iostream>
#include <gtkmm/application.h>
#include <gtkmm/window.h>
#include "ville.h"
#include "gui.h"
using namespace std;

int main(int argc, char* argv[]) {
    if (argc != 2) {
        cout << "Lecture impossible: arguments manquants/en plus" << endl;
    } else {
        lecture(argv[1]);
    }
    return 0;
}