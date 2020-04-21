//interface complete de graphic que va utiliser gui

#ifndef GRAPHIC_GUI_HEADER_H
#define GRAPHIC_GUI_HEADER_H

#include <gtkmm/drawingarea.h>
#include "graphic.h"
#include "graphic_color.h"

void graphic_set_context(const Cairo::RefPtr<Cairo::Context>& cr);

#endif