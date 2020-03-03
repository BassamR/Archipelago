# Variables
CXX = g++
CXXFLAGS = -g -std=c++11
OFILES = projet.o 

# Commandes
all: projet

projet: projet.o 
	$(CXX) $(OFILES) -o $@

projet.o: projet.cc constantes.h
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