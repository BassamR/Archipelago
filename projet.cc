//Projet Archipelago
//Auteurs: Hugo Masson, Bassam El Rawas (Sciper xxx, 310635)
#include <iostream>
#include "ville.h"
#include "constantes.h"
using namespace std;

int main(int argc, char* argv[]) {
    if (argc != 2) {
        cout << "Lecture impossible: arguments manquants" << endl;
    } else {
        lecture(argv[1]);
    }
    return 0;
}

//Conventions de prog, problemes:
//Indentations, namespace