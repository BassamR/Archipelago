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

//Static functions
static string convertCritereToString(const double& critere);

//MyArea class definition:
MyArea::MyArea(): empty(false) {
    add_events(Gdk::BUTTON_PRESS_MASK);
    add_events(Gdk::BUTTON_RELEASE_MASK);
    add_events(Gdk::BUTTON_MOTION_MASK);
}

MyArea::~MyArea() {}

void MyArea::refresh() {
    auto win = get_window();
    if(win) {
        Gdk::Rectangle r(0,0, get_allocation().get_width(), 
            get_allocation().get_height());
        win->invalidate_rect(r, false); //forces a call to on_draw
    }
}

bool MyArea::on_draw(const Cairo::RefPtr<Cairo::Context>& cr) {
    // Gtk::Allocation allocation = get_allocation();
    // const int width = allocation.get_width();
    // const int height = allocation.get_height();

    // coordinates for the center of the GTKmm window
    // int xc, yc;
    // xc = width / 2;
    // yc = height / 2;
    graphic_set_context(cr);

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
        cout << "left mouse clicked" << " " << event->x << " " << event->y << endl;
        return true;
    }

    if((gint)event->type == Gdk::BUTTON_PRESS and event->button == 3) {
        cout << "right mouse clicked" << " " << event->x << " " << event->y << endl;
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

bool MyArea::on_motion_notify_event(GdkEventMotion* event) {
    if((gint)event->type == Gdk::MOTION_NOTIFY) {
        cout << "im holding" << endl;
        return true;
    }

    return true;
}

// bool MyArea::on_event(GdkEvent* event) {
//     static bool isHeld(false);
//     GdkEventButton* bEvent;
//     GdkEventMotion* mEvent;

//     switch((gint)event->type) {
//         case Gdk::BUTTON_PRESS: {
//             bEvent = (GdkEventButton *) event;
//            // if(event->button == 1) {
//                 isHeld = true;
//                 cout << "left button clicked" << endl;
//                 cout << bEvent->x << " " << bEvent->y << endl;
//                 break;
//             //}
//         }

//         case Gdk::BUTTON_RELEASE: {
//             bEvent = (GdkEventButton *) event;
//             isHeld = false;
//             cout << "left button released" << endl;
//             cout << bEvent->x << " " << bEvent->y << endl;
//             break;
//         }

//         case Gdk::MOTION_NOTIFY: {
//             mEvent = (GdkEventMotion *) event;
//             if(isHeld) {
//                 cout << "im holding" << endl;
//             }
//         }

//         default: break;
//     }

//     // Check if the event is a left(1) button click (right click is 3)
//     return true;
// }

void MyArea::clear() {
    empty = true;
    refresh();
}

void MyArea::draw() {
    empty = false;
    refresh();
}

//Gui class definition:
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

//Click buttons
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

//Edit link togglebutton
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

//Housing radiobutton
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

//Transport radiobutton
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

//Production radiobutton 
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