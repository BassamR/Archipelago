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
static int uidCounter(0);
static string convertCritereToString(const double& critere);

//MyArea constructor/destructor:
MyArea::MyArea(): empty(false), xMin(-dim_max), xMax(dim_max), yMin(-dim_max), yMax(dim_max) {
    add_events(Gdk::BUTTON_PRESS_MASK);
    add_events(Gdk::BUTTON_RELEASE_MASK);
    //add_events(Gdk::BUTTON_MOTION_MASK);
}

MyArea::~MyArea() {}

//MyArea canvas methods:
void MyArea::refresh() {
    auto win = get_window();
    if(win) {
        Gdk::Rectangle r(0,0, get_allocation().get_width(), 
            get_allocation().get_height());
        win->invalidate_rect(r, false); //forces a call to on_draw
    }
}

void MyArea::clear() {
    empty = true;
    refresh();
}

void MyArea::draw() {
    empty = false;
    refresh();
}

//MyArea event override methods:
bool MyArea::on_draw(const Cairo::RefPtr<Cairo::Context>& cr) {
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

    if(not empty) {
        makeBgWhite();

        Ville::getVilleInstance()->drawLinks();
        Ville::getVilleInstance()->drawNodes(); //draws the city
        //Ville::getVilleInstance()->tempDrawColor();
        //MyGui::refreshCriteres(); //find a way to refresh criteres 
    
    } else {
        makeBgWhite();
        Ville::getVilleInstance()->resetVille();
        cout << "canvas cleared!" << endl;
    }

    cout << "ondraw called" << endl;

    return true;
}

bool MyArea::on_button_press_event(GdkEventButton* event) {
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

bool MyArea::on_button_release_event(GdkEventButton* event) {
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

// bool MyArea::on_motion_notify_event(GdkEventMotion* event) {
//     if((gint)event->type == Gdk::MOTION_NOTIFY) {
//         cout << "im holding" << endl;
//         return true;
//     }

//     return true;
// }

//MyArea utility methods:
void MyArea::convertCoordsToModele(Coords& clickLocation) {
    double newX = (clickLocation.x/width) * (xMax-xMin) + xMin;
    double newY = yMax - (clickLocation.y/height) * (yMax-yMin);

    clickLocation.x = newX;
    clickLocation.y = newY;
}

//MyArea event handling methods:
void MyArea::handleLeftClick(Coords clickLocation) {
    Ville::getVilleInstance()->setActiveNode(clickLocation);
    refresh();
}

void MyArea::handleRightClick(Coords clickLocation) {
    refresh();
}

//Gui constructor/destructor:
MyGui::MyGui():
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

MyGui::~MyGui() {}

//Gui clickable buttons:
void MyGui::onButtonClickExit() {
    cout << "exit button clicked" << endl;
    hide(); //exits the app
}

void MyGui::onButtonClickNew() {
    Ville::getVilleInstance()->resetVille();
    refreshCriteres();
    cout << "new button clicked" << endl;
    mArea.clear();
}

void MyGui::onButtonClickOpen() {
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
            // mArea.clear();
            // deleteCity();
            cout << "Open clicked." << endl;

            //Notice that this is a std::string, not a Glib::ustring.
            string filename = dialog.get_filename();
            cout << "File selected: " <<  filename << endl;
            
            char* filenameChar = const_cast<char*>(filename.c_str());
            
            lecture(filenameChar);
            refreshCriteres();
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

void MyGui::onButtonClickSave() {
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

void MyGui::onButtonClickZoomIn() {
    cout << "zoomIn button clicked" << endl;
}

void MyGui::onButtonClickZoomOut() {
    cout << "zoomOut button clicked" << endl;
}

void MyGui::onButtonClickZoomR() {
    cout << "zoomReset button clicked" << endl;
}

//Shortest path togglebutton
void MyGui::onTButtonClickShortest() {
    if(mTButtonShortest.get_active()) onTButtonPressShortest();
    else onTButtonReleaseShortest();
}

void MyGui::onTButtonPressShortest() {
    cout << "shortest path button pressed" << endl;
}

void MyGui::onTButtonReleaseShortest() {
    cout << "shortest path button released" << endl;
}

//Gui edit link togglebutton
void MyGui::onTButtonClickEditLink() {
    if(mTButtonEditLink.get_active()) onTButtonPressEditLink();
    else onTButtonReleaseEditLink();
}

void MyGui::onTButtonPressEditLink() {
    cout << "edit link button pressed" << endl;
}

void MyGui::onTButtonReleaseEditLink() {
    cout << "edit link button released" << endl;
}

//Gui housing radiobutton
void MyGui::onRButtonClickH() {
    if(mRButtonH.get_active()) onRButtonPressH();
    else onRButtonReleaseH();
}

void MyGui::onRButtonPressH() {
    cout << "housing radiobutton pressed" << endl;
}

void MyGui::onRButtonReleaseH() {
    cout << "housing radiobutton released" << endl;
}

//Gui transport radiobutton
void MyGui::onRButtonClickT() {
    if(mRButtonT.get_active()) onRButtonPressT();
    else onRButtonReleaseT();
}

void MyGui::onRButtonPressT() {
    cout << "transport radiobutton pressed" << endl;
}

void MyGui::onRButtonReleaseT() {
    cout << "transport radiobutton released" << endl;
}

//Gui production radiobutton 
void MyGui::onRButtonClickP() {
    if(mRButtonP.get_active()) onRButtonPressP();
    else onRButtonReleaseP();
}

void MyGui::onRButtonPressP() {
    cout << "production radiobutton pressed" << endl;

}

void MyGui::onRButtonReleaseP() {
    cout << "production radiobutton released" << endl;
}

//Misc MyGui methods:
void MyGui::refreshCriteres() {
    string enj = convertCritereToString(Ville::getVilleInstance()->critereENJ());
    string ci = convertCritereToString(Ville::getVilleInstance()->critereCI());
    string mta = convertCritereToString(Ville::getVilleInstance()->critereMTA());

    mLabelCriteres.set_text(string("ENJ: ") + enj + string("\nCI: ") + ci 
            + string("\nMTA: ") + mta);
}

//Private gui methods (to better organize gui constructor):
void MyGui::setupGui() {
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

void MyGui::initGeneral() {
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

void MyGui::initDisplay() {
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

void MyGui::initEditor() {
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

void MyGui::initInformations() {
    mBoxLeft.pack_start(mFrameInformations, false, false);
    mFrameInformations.add(mLabelCriteres);

    string enj = convertCritereToString(Ville::getVilleInstance()->critereENJ());
    string ci = convertCritereToString(Ville::getVilleInstance()->critereCI());
    string mta = convertCritereToString(Ville::getVilleInstance()->critereMTA());

    mLabelCriteres.set_text(string("ENJ: ") + enj + string("\nCI: ") + ci 
            + string("\nMTA: ") + mta);
} //initialize informations section of gui

void MyGui::connectButtons() {
    mButtonExit.signal_clicked().connect(sigc::mem_fun(*this,
                    &MyGui::onButtonClickExit));
    mButtonNew.signal_clicked().connect(sigc::mem_fun(*this,
                    &MyGui::onButtonClickNew));
    mButtonOpen.signal_clicked().connect(sigc::mem_fun(*this,
                    &MyGui::onButtonClickOpen));
    mButtonSave.signal_clicked().connect(sigc::mem_fun(*this,
                    &MyGui::onButtonClickSave));
    mButtonZoomIn.signal_clicked().connect(sigc::mem_fun(*this,
                    &MyGui::onButtonClickZoomIn));
    mButtonZoomOut.signal_clicked().connect(sigc::mem_fun(*this,
                    &MyGui::onButtonClickZoomOut));
    mButtonZoomR.signal_clicked().connect(sigc::mem_fun(*this,
                    &MyGui::onButtonClickZoomR));

    mTButtonShortest.signal_clicked().connect(sigc::mem_fun(*this,
                    &MyGui::onTButtonClickShortest));
    mTButtonEditLink.signal_clicked().connect(sigc::mem_fun(*this,
                    &MyGui::onTButtonClickEditLink));
    
    mRButtonH.signal_clicked().connect(sigc::mem_fun(*this,
                    &MyGui::onRButtonClickH));
    mRButtonT.signal_clicked().connect(sigc::mem_fun(*this,
                    &MyGui::onRButtonClickT));
    mRButtonP.signal_clicked().connect(sigc::mem_fun(*this,
                    &MyGui::onRButtonClickP));

    
} //connect buttons to signal handlers

//Static functions definition
string convertCritereToString(const double& critere) {
    stringstream temp;
    temp << critere;
    return temp.str();
}