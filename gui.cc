#include <iostream>
#include <string>
#include <sstream>
#include <cairomm/context.h>
#include <gtkmm/filechooser.h>
#include <gtkmm/filechooserdialog.h>
#include "gui.h"
#include "ville.h"
#include "tools.h"
#include "graphic_gui.h"
#include "constantes.h"
using namespace std;

//Static functions and variables
static Gui* guiObject(nullptr);
static int uidCounter(1000);
static string convertCritereToString(const double& critere);

//Canvas constructor/destructor:
Canvas::Canvas(): empty(false), xMin(-dim_max), xMax(dim_max), yMin(-dim_max), yMax(dim_max) {
    add_events(Gdk::BUTTON_PRESS_MASK);
    add_events(Gdk::BUTTON_RELEASE_MASK);
    //add_events(Gdk::BUTTON_MOTION_MASK);
}

Canvas::~Canvas() {}

//Canvas canvas methods:
void Canvas::refresh() {
    auto win = get_window();
    if(win) {
        Gdk::Rectangle r(0,0, get_allocation().get_width(), 
            get_allocation().get_height());
        win->invalidate_rect(r, false); //forces a call to on_draw
    }
}

void Canvas::clear() {
    empty = true;
    refresh();
}

void Canvas::draw() {
    empty = false;
    refresh();
}

//Canvas event override methods:
bool Canvas::on_draw(const Cairo::RefPtr<Cairo::Context>& cr) {
    Gtk::Allocation allocation = get_allocation();
    width = allocation.get_width();
    height = allocation.get_height();

    int xc, yc; //center of the gtk window
    xc = width/2;
    yc = height/2;

    //cr->set_identity_matrix();
    cr->translate(xc, yc);
    cr->scale(width/(xMax - xMin), -height/(yMax - yMin));
	cr->translate(-(xMin + xMax)/2, -(yMin + yMax)/2);

    graphicSetContext(cr);

    //guiObject->refreshCriteres();

    if(not empty) {
        makeBgWhite();
        Ville::getVilleInstance()->drawLinks();
        Ville::getVilleInstance()->drawNodes(); //draws the city
        //Ville::getVilleInstance()->tempDrawColor();    
    } else {
        makeBgWhite();
        Ville::getVilleInstance()->resetVille();
        cout << "canvas cleared!" << endl;
    }

    cout << "ondraw called" << endl;

    return true;
}

bool Canvas::on_button_press_event(GdkEventButton* event) {
    if((gint)event->type == Gdk::BUTTON_PRESS and event->button == 1) {
        double x = event->x;
        double y = event->y;

        Coords leftClickLocation{x, y};

        convertCoordsToModele(leftClickLocation);
        cout << "left mouse clicked (modele)" << " " << leftClickLocation.x << " " << leftClickLocation.y << endl;
        handleLeftClick(leftClickLocation);

        //Ville::getVilleInstance()->setActiveNode(leftClickLocation);
        // Ville::getVilleInstance()->createHousing(uidCounter, x, y, min_capacity);
        //refresh();

        //++uidCounter;
        return true;
    }

    if((gint)event->type == Gdk::BUTTON_PRESS and event->button == 3) {
        double x1 = event->x;
        double y1 = event->y;
        Coords rightClickLocation{x1, y1};
        convertCoordsToModele(rightClickLocation);
        cout << "right mouse clicked (modele)" << " " << rightClickLocation.x << " " << rightClickLocation.y << endl;
        handleRightClick(rightClickLocation);
        return true;
    }

    return true;
}

bool Canvas::on_button_release_event(GdkEventButton* event) {
    if((gint)event->type == Gdk::BUTTON_RELEASE and event->button == 1) {
        cout << "left mouse released" << " " << event->x << " " << event->y << endl;
        return true;
    }

    if((gint)event->type == Gdk::BUTTON_RELEASE and event->button == 3) {
        cout << "right mouse released" << " " << event->x << " " << event->y << endl;
        return true;
    }

    return true;
}

// bool Canvas::on_motion_notify_event(GdkEventMotion* event) {
//     if((gint)event->type == Gdk::MOTION_NOTIFY) {
//         cout << "im holding" << endl;
//         return true;
//     }

//     return true;
// }

//Canvas utility methods:
void Canvas::convertCoordsToModele(Coords& clickLocation) {
    double newX = (clickLocation.x/width) * (xMax-xMin) + xMin;
    double newY = yMax - (clickLocation.y/height) * (yMax-yMin);

    clickLocation.x = newX;
    clickLocation.y = newY;
}

//Canvas event handling methods:
void Canvas::handleLeftClick(Coords clickLocation) {
    if(Ville::getVilleInstance()->getActiveNode() == -1) {
        if(Ville::getVilleInstance()->createHousing(uidCounter, clickLocation.x, clickLocation.y, min_capacity)) {
            guiObject->refreshCriteres();
            ++uidCounter;
        }
    }

    Ville::getVilleInstance()->setActiveNode(clickLocation);

    draw();
}

void Canvas::handleRightClick(Coords clickLocation) {
    int activeNode = Ville::getVilleInstance()->getActiveNode();
    if(activeNode != -1) {
        Ville::getVilleInstance()->getNode(activeNode)->setCoords(clickLocation.x, clickLocation.y);
        guiObject->refreshCriteres();
    }
    draw();
}

//Gui constructor/destructor:
Gui::Gui():
    mBox(Gtk::ORIENTATION_HORIZONTAL, 10), mBoxLeft(Gtk::ORIENTATION_VERTICAL, 10), 
    mBoxRight(Gtk::ORIENTATION_HORIZONTAL, 10), 
    mBoxDisplay(Gtk::ORIENTATION_VERTICAL, 4), mBoxEditor(Gtk::ORIENTATION_VERTICAL),

    mFrameGeneral("General"), mFrameDisplay("Display"), mFrameEditor("Editor"), 
    mFrameInformations("Informations"), 

    mButtonExit("exit"), mButtonNew("new"), mButtonOpen("open"), mButtonSave("save"), 
    mButtonZoomIn("zoom in (i)"), mButtonZoomOut("zoom out (o)"), 
    mButtonZoomR("zoom reset (r)"),

    mTButtonShortest("shortest path"), mTButtonEditLink("edit link"),

    mBBGeneral(Gtk::ORIENTATION_VERTICAL), mBBDisplay(Gtk::ORIENTATION_VERTICAL), 
    mBBEditor(Gtk::ORIENTATION_VERTICAL), mBBInformations(Gtk::ORIENTATION_VERTICAL),

    mRButtonH("housing"), mRButtonT("transport"), mRButtonP("production") {
    //General setup
    guiObject = this;
    setupGui();

    //Add general frame
    initGeneral();

    //Add Display Frame
    initDisplay();

    //Add Editor Frame
    initEditor();

    //Add Informations Frame
    initInformations();

    //Connect buttons to signal handlers
    connectButtons();

    //show all widgets
    show_all_children();
}

Gui::~Gui() {}

//Gui clickable buttons:
void Gui::onButtonClickExit() {
    cout << "exit button clicked" << endl;
    hide(); //exits the app
}

void Gui::onButtonClickNew() {
    Ville::getVilleInstance()->resetVille();
    cout << "new button clicked" << endl;
    mArea.clear();
}

void Gui::onButtonClickOpen() {
    cout << "open button clicked" << endl;
    //mArea.clear();

    Gtk::FileChooserDialog dialog("Please choose a file",
            Gtk::FILE_CHOOSER_ACTION_OPEN);
    //dialog.set_transient_for(*this); 
    //if i remove this, Open works on windows but not on VM

    //Add response buttons the the dialog:
    dialog.add_button("_Cancel", Gtk::RESPONSE_CANCEL);
    dialog.add_button("_Open", Gtk::RESPONSE_OK);

    //Show the dialog and wait for a user response:
    int result = dialog.run();

    //Handle the response:
    switch(result) {
        case(Gtk::RESPONSE_OK): {
            cout << "Open clicked." << endl;

            string filename = dialog.get_filename();
            cout << "File selected: " <<  filename << endl;
            
            char* filenameChar = const_cast<char*>(filename.c_str());
            
            lecture(filenameChar);
            mArea.draw();
            //delete filenameChar;
            break;
        }

        case(Gtk::RESPONSE_CANCEL): {
            cout << "Cancel clicked." << endl;
            break;
        }

        default: {
            cout << "Unexpected button clicked." << endl;
            break;
        }
    }
}

void Gui::onButtonClickSave() {
    cout << "save button clicked" << endl;

    Gtk::FileChooserDialog dialog("Please choose a file",
            Gtk::FILE_CHOOSER_ACTION_SAVE);
    //dialog.set_transient_for(*this); //if i remove this, Open works...

    dialog.add_button("_Cancel", Gtk::RESPONSE_CANCEL);
    dialog.add_button("_Save", Gtk::RESPONSE_OK);

    int result = dialog.run();

    switch(result) {
        case(Gtk::RESPONSE_OK): {
            cout << "Save clicked." << endl;

            string filename = dialog.get_filename();
            cout << "File selected for save: " <<  filename << endl;
            
            Ville::getVilleInstance()->saveVille(filename);
            break;
        }

        case(Gtk::RESPONSE_CANCEL): {
            cout << "Cancel clicked." << endl;
            break;
        }

        default: {
            cout << "Unexpected button clicked." << endl;
            break;
        }
    }

}

void Gui::onButtonClickZoomIn() {
    cout << "zoomIn button clicked" << endl;
}

void Gui::onButtonClickZoomOut() {
    cout << "zoomOut button clicked" << endl;
}

void Gui::onButtonClickZoomR() {
    cout << "zoomReset button clicked" << endl;
}

//Shortest path togglebutton
void Gui::onTButtonClickShortest() {
    if(mTButtonShortest.get_active()) onTButtonPressShortest();
    else onTButtonReleaseShortest();
}

void Gui::onTButtonPressShortest() {
    cout << "shortest path button pressed" << endl;
}

void Gui::onTButtonReleaseShortest() {
    cout << "shortest path button released" << endl;
}

//Gui edit link togglebutton
void Gui::onTButtonClickEditLink() {
    if(mTButtonEditLink.get_active()) onTButtonPressEditLink();
    else onTButtonReleaseEditLink();
}

void Gui::onTButtonPressEditLink() {
    cout << "edit link button pressed" << endl;
}

void Gui::onTButtonReleaseEditLink() {
    cout << "edit link button released" << endl;
}

//Gui housing radiobutton
void Gui::onRButtonClickH() {
    if(mRButtonH.get_active()) onRButtonPressH();
    else onRButtonReleaseH();
}

void Gui::onRButtonPressH() {
    cout << "housing radiobutton pressed" << endl;
}

void Gui::onRButtonReleaseH() {
    cout << "housing radiobutton released" << endl;
}

//Gui transport radiobutton
void Gui::onRButtonClickT() {
    if(mRButtonT.get_active()) onRButtonPressT();
    else onRButtonReleaseT();
}

void Gui::onRButtonPressT() {
    cout << "transport radiobutton pressed" << endl;
}

void Gui::onRButtonReleaseT() {
    cout << "transport radiobutton released" << endl;
}

//Gui production radiobutton 
void Gui::onRButtonClickP() {
    if(mRButtonP.get_active()) onRButtonPressP();
    else onRButtonReleaseP();
}

void Gui::onRButtonPressP() {
    cout << "production radiobutton pressed" << endl;

}

void Gui::onRButtonReleaseP() {
    cout << "production radiobutton released" << endl;
}

//Misc MyGui methods:
void Gui::refreshCriteres() {
    string enj = convertCritereToString(Ville::getVilleInstance()->critereENJ());
    string ci = convertCritereToString(Ville::getVilleInstance()->critereCI());
    string mta = convertCritereToString(Ville::getVilleInstance()->critereMTA());

    mLabelCriteres.set_text(string("ENJ: ") + enj + string("\nCI: ") + ci 
            + string("\nMTA: ") + mta);
}

//Private gui methods (to better organize gui constructor):
void Gui::setupGui() {
    //Basic definitions
    set_title("Archipelago");
    set_border_width(5);
    set_position(Gtk::WIN_POS_CENTER);
    //set_default_size(default_drawing_size, default_drawing_size);

    //Add main box
    add(mBox);

    //Fill main box
    mBox.pack_start(mBoxLeft);
    mBox.pack_start(mBoxRight);

    //Start canvas
    mArea.set_size_request(default_drawing_size, default_drawing_size);
    mBoxRight.pack_start(mFrameCanvas, false, false);
    mFrameCanvas.add(mArea); //if i remove this, Open button works...
}

void Gui::initGeneral() {
    //Add general frame to box
    mBoxLeft.pack_start(mFrameGeneral, false, false);

    //add a box here which will contain the buttonbox

    //Add ButtonBox inside frame
    mFrameGeneral.add(mBBGeneral);
    mBBGeneral.set_spacing(5);

    mBBGeneral.add(mButtonExit);
    mBBGeneral.add(mButtonNew);
    mBBGeneral.add(mButtonOpen);
    mBBGeneral.add(mButtonSave);
} //initialize general section of gui

void Gui::initDisplay() {
    mBoxLeft.pack_start(mFrameDisplay, false, false);

    //Add box inside (to be able to put multiple widgets)
    mFrameDisplay.add(mBoxDisplay);

    //Add ButtonBox
    mBoxDisplay.pack_start(mBBDisplay, false, false);
    mBBDisplay.set_spacing(5);
    mBBDisplay.add(mTButtonShortest);
    mBBDisplay.add(mButtonZoomIn);
    mBBDisplay.add(mButtonZoomOut);
    mBBDisplay.add(mButtonZoomR);

    //Add zoom label
    mBoxDisplay.pack_start(mLabelZoom);
    mLabelZoom.set_text(string("zoom: ") + string("1.00x")); //temp nonfunctional zoom
} //initialize dislay section of gui

void Gui::initEditor() {
    mBoxLeft.pack_start(mFrameEditor, false, false);
    mFrameEditor.add(mBoxEditor);

    mBoxEditor.pack_start(mTButtonEditLink, false, false);

    mRButtonT.join_group(mRButtonH);
    mRButtonP.join_group(mRButtonH); //RadioButtons work in "groups"
    mBoxEditor.pack_start(mRButtonH);
    mBoxEditor.pack_start(mRButtonT);
    mBoxEditor.pack_start(mRButtonP);
    mRButtonH.set_active();
} //initialize editor section of gui

void Gui::initInformations() {
    mBoxLeft.pack_start(mFrameInformations, false, false);
    mFrameInformations.add(mLabelCriteres);

    string enj = convertCritereToString(Ville::getVilleInstance()->critereENJ());
    string ci = convertCritereToString(Ville::getVilleInstance()->critereCI());
    string mta = convertCritereToString(Ville::getVilleInstance()->critereMTA());

    mLabelCriteres.set_text(string("ENJ: ") + enj + string("\nCI: ") + ci 
            + string("\nMTA: ") + mta);
} //initialize informations section of gui

void Gui::connectButtons() {
    mButtonExit.signal_clicked().connect(sigc::mem_fun(*this,
                    &Gui::onButtonClickExit));
    mButtonNew.signal_clicked().connect(sigc::mem_fun(*this,
                    &Gui::onButtonClickNew));
    mButtonOpen.signal_clicked().connect(sigc::mem_fun(*this,
                    &Gui::onButtonClickOpen));
    mButtonSave.signal_clicked().connect(sigc::mem_fun(*this,
                    &Gui::onButtonClickSave));
    mButtonZoomIn.signal_clicked().connect(sigc::mem_fun(*this,
                    &Gui::onButtonClickZoomIn));
    mButtonZoomOut.signal_clicked().connect(sigc::mem_fun(*this,
                    &Gui::onButtonClickZoomOut));
    mButtonZoomR.signal_clicked().connect(sigc::mem_fun(*this,
                    &Gui::onButtonClickZoomR));

    mTButtonShortest.signal_clicked().connect(sigc::mem_fun(*this,
                    &Gui::onTButtonClickShortest));
    mTButtonEditLink.signal_clicked().connect(sigc::mem_fun(*this,
                    &Gui::onTButtonClickEditLink));
    
    mRButtonH.signal_clicked().connect(sigc::mem_fun(*this,
                    &Gui::onRButtonClickH));
    mRButtonT.signal_clicked().connect(sigc::mem_fun(*this,
                    &Gui::onRButtonClickT));
    mRButtonP.signal_clicked().connect(sigc::mem_fun(*this,
                    &Gui::onRButtonClickP));

    
} //connect buttons to signal handlers

//Static functions definition
string convertCritereToString(const double& critere) {
    stringstream temp;
    temp << critere;
    return temp.str();
}