#include <iostream>
#include <cairomm/context.h>
#include "gui.h"
#include "ville.h"
#include "tools.h"
#include "graphic_gui.h"
#include "constantes.h"
using namespace std;

//MyArea class definition:
MyArea::MyArea() {}
MyArea::~MyArea() {}

bool MyArea::on_draw(const Cairo::RefPtr<Cairo::Context>& cr) {
    Gtk::Allocation allocation = get_allocation();
    const int width = allocation.get_width();
    const int height = allocation.get_height();

    // coordinates for the center of the GTKmm window
    int xc, yc;
    xc = width / 2;
    yc = height / 2;
    graphic_set_context(cr);

    //call draw function from ville in here, which will call noeud's individual draw functions
    //they will call functions from tools which will call functions from graphic
    graphic_draw_shape(width, height,xc,yc);

    drawCity();

    return true;
}

//Gui class definition:
MyGui::MyGui():
    mBox(Gtk::ORIENTATION_HORIZONTAL, 10), mBoxLeft(Gtk::ORIENTATION_VERTICAL, 10), 
    mBoxRight(Gtk::ORIENTATION_HORIZONTAL, 10), mBoxDisplay(Gtk::ORIENTATION_VERTICAL),
    mFrameGeneral("General"), mFrameDisplay("Display"), mFrameEditor("Editor"), 
    mFrameInformations("Informations"), mButtonExit("exit"), mButtonNew("new"), 
    mButtonOpen("open"), mButtonSave("save"), mButtonShortest("shortest path"), 
    mButtonZoomIn("zoom in"), mButtonZoomOut("zoom out"), mButtonZoomR("zoom reset"), 
    mButtonEditLink("edit link"), mLabelENJ("ENJ: \nCI: \nMTA: "), mLabelZoom("zoom"),
    mBBGeneral(Gtk::ORIENTATION_VERTICAL), mBBDisplay(Gtk::ORIENTATION_VERTICAL), 
    mBBEditor(Gtk::ORIENTATION_VERTICAL), mBBInformations(Gtk::ORIENTATION_VERTICAL) {
    //Basic definitions
    set_title("Archipelago");
    set_border_width(5);

    //Add main box
    add(mBox);

    //Fill main box
    mBox.pack_start(mBoxLeft);
    mBox.pack_start(mBoxRight);

    //Start canvas
    mArea.set_size_request(600, 600);
    mBoxRight.pack_start(mFrameCanvas, false, false);
    mFrameCanvas.add(mArea);

    //Add general frame
    mBoxLeft.pack_start(mFrameGeneral, false, false);
    //Add ButtonBox inside frame
    mFrameGeneral.add(mBBGeneral);
    mBBGeneral.set_spacing(5);
    mBBGeneral.add(mButtonExit);
    mBBGeneral.add(mButtonNew);
    mBBGeneral.add(mButtonOpen);
    mBBGeneral.add(mButtonSave);

    //Add Display Frame
    mBoxLeft.pack_start(mFrameDisplay, false, false);
    //Add box inside
    mFrameDisplay.add(mBoxDisplay);
    //Add ButtonBox
    mBoxDisplay.pack_start(mBBDisplay, false, false);
    mBBDisplay.set_spacing(5);
    mBBDisplay.add(mButtonShortest);
    mBBDisplay.add(mButtonZoomIn);
    mBBDisplay.add(mButtonZoomOut);
    mBBDisplay.add(mButtonZoomR);
    //Add zoom label
    mBoxDisplay.pack_start(mLabelZoom);

    mBoxLeft.pack_start(mFrameEditor, false, false);
    mFrameEditor.add(mButtonEditLink);

    mBoxLeft.pack_start(mFrameInformations, false, false);
    mFrameInformations.add(mLabelENJ);


    show_all_children();
}

MyGui::~MyGui() {}