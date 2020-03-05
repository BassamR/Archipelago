# Variables
CXX = g++
CXXFLAGS = -g -Wall -std=c++11
OFILES = projet.o ville.o noeud.o tools.o error.o

# Commandes
all: projet

projet: $(OFILES)
	$(CXX) $(OFILES) -o $@

projet.o: projet.cc constantes.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

ville.o: ville.cc noeud.h tools.h error.h constantes.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

noeud.o: noeud.cc error.h tools.h constantes.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

tools.o: tools.cc constantes.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

error.o: error.cc constantes.h
	$(CXX) $(CXXFLAGS) -c $< -o $@


# Misc
clean:
	@echo " *** EFFACE MODULES OBJET ET EXECUTABLE ***"
	@/bin/rm -f *.o *.x *.cc~ *.h~ projet

depend:
	@echo " *** MISE A JOUR DES DEPENDANCES ***"
	@(sed '/^# DO NOT DELETE THIS LINE/q' Makefile && \
	  $(CXX) -MM $(CXXFLAGS) $(CXXFILES) | \
	  egrep -v "/usr/include" \
	 ) >Makefile.new
	@mv Makefile.new Makefile