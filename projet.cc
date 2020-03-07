// un module modeste de haut niveau, qui ne peut communiquer que avec ville et qui
// fournira le fichier executable
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
