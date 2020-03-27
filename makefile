# Variables
OUT = projet
CXX = g++
CXXFLAGS = -g -Wall -std=c++11
LINKING = `pkg-config --cflags gtkmm-3.0`
LDLIBS = `pkg-config --libs gtkmm-3.0`
OFILES = projet.o gui.o ville.o noeud.o tools.o error.o graphic.o

# Dependances
all: $(OUT)

graphic.o: graphic.cc graphic_gui.h graphic.h
	$(CXX) $(CXXFLAGS) $(LINKING) -c $< -o $@ $(LINKING)

error.o: error.cc error.h constantes.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

tools.o: tools.cc tools.h constantes.h graphic.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

noeud.o: noeud.cc noeud.h error.h tools.h constantes.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

ville.o: ville.cc ville.h noeud.h tools.h error.h constantes.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

gui.o: gui.cc gui.h graphic_gui.h
	$(CXX) $(CXXFLAGS) $(LINKING) -c $< -o $@ $(LINKING)

projet.o: projet.cc ville.h gui.h
	$(CXX) $(CXXFLAGS) $(LINKING) -c $< -o $@ $(LINKING)

$(OUT): $(OFILES)
	$(CXX) $(CXXFLAGS) $(LINKING) $(OFILES) -o $@ $(LDLIBS)

# Commandes
clean:
	@echo " *** EFFACE MODULES OBJET ET EXECUTABLE ***"
	@/bin/rm -f *.o *.x *.cc~ *.h~ $(OUT)

depend:
	@echo " *** MISE A JOUR DES DEPENDANCES ***"
	@(sed '/^# DO NOT DELETE THIS LINE/q' Makefile && \
	  $(CXX) -MM $(CXXFLAGS) $(CXXFILES) | \
	  egrep -v "/usr/include" \
	 ) >Makefile.new
	@mv Makefile.new Makefile