/**
* \name graphic_gui.h
* \author Hugo Masson, Bassam El Rawas (Sciper 314886, 310635)
* \date May 2020
* \version 1.0
*/

//Architecture: Fig 11 b1

#ifndef GRAPHIC_GUI_HEADER_H
#define GRAPHIC_GUI_HEADER_H

#include <gtkmm/drawingarea.h>
#include "graphic.h"
#include "graphic_color.h"

void graphicSetContext(const Cairo::RefPtr<Cairo::Context>& cr);

#endif