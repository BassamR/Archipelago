#include <iostream>
#include <string>
#include <cairomm/context.h>
// #include <gtkmm/filechooser.h>
// #include <gtkmm/filechooserdialog.h>
#include <gtkmm.h>
#include "gui.h"
#include "ville.h"
#include "tools.h"
#include "graphic_gui.h"
#include "constantes.h"
using namespace std;

//MyArea class definition:
MyArea::MyArea(): empty(false) {}
MyArea::~MyArea() {}

void MyArea::clear() {
    empty = true;
    refresh();
}

void MyArea::draw() {
    empty = false;
    refresh();
}

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
        drawCity(); //this function properly draws a city
    } else {
        makeBgWhite();
        deleteCity();
        cout << "canvas cleared!" << endl;
    }

    cout << "ondraw called" << endl;

    return true;
}

//Gui class definition:
MyGui::MyGui():
    mBox(Gtk::ORIENTATION_HORIZONTAL, 10), mBoxLeft(Gtk::ORIENTATION_VERTICAL, 10), 
    mBoxRight(Gtk::ORIENTATION_HORIZONTAL, 10), 
    mBoxDisplay(Gtk::ORIENTATION_VERTICAL, 4), mBoxEditor(Gtk::ORIENTATION_VERTICAL),

    mFrameGeneral("General"), mFrameDisplay("Display"), mFrameEditor("Editor"), 
    mFrameInformations("Informations"), 

    mButtonExit("exit"), mButtonNew("new"), mButtonOpen("open"), mButtonSave("save"), 
    mButtonZoomIn("zoom in"), mButtonZoomOut("zoom out"), mButtonZoomR("zoom reset"),

    mTButtonShortest("shortest path"), mTButtonEditLink("edit link"),

    mLabelENJ("ENJ: \nCI: \nMTA: "), mLabelZoom("zoom:"),

    mBBGeneral(Gtk::ORIENTATION_VERTICAL), mBBDisplay(Gtk::ORIENTATION_VERTICAL), 
    mBBEditor(Gtk::ORIENTATION_VERTICAL), mBBInformations(Gtk::ORIENTATION_VERTICAL),

    mRButtonH("housing"), mRButtonT("transport"), mRButtonP("production") {
    //Basic definitions
    set_title("Archipelago");
    set_border_width(5);

    //Add main box
    add(mBox);

    //Fill main box
    mBox.pack_start(mBoxLeft);
    mBox.pack_start(mBoxRight);

    //Start canvas
    mArea.set_size_request(800, 800);
    mBoxRight.pack_start(mFrameCanvas, false, false);
    mFrameCanvas.add(mArea); //if i remove this, Open button works...

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

    //Add Editor Frame
    mBoxLeft.pack_start(mFrameEditor, false, false);
    mFrameEditor.add(mBoxEditor);
    mBoxEditor.pack_start(mTButtonEditLink, false, false);
    mRButtonT.join_group(mRButtonH);
    mRButtonP.join_group(mRButtonH); //RadioButtons work in "groups"
    mBoxEditor.pack_start(mRButtonH);
    mBoxEditor.pack_start(mRButtonT);
    mBoxEditor.pack_start(mRButtonP);
    mRButtonH.set_active();

    //Add Informations Frame
    mBoxLeft.pack_start(mFrameInformations, false, false);
    mFrameInformations.add(mLabelENJ);

    //Connect buttons to signal handlers
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

    //show all
    show_all_children();
}

MyGui::~MyGui() {}

//Click buttons
void MyGui::onButtonClickExit() {
    cout << "exit button clicked" << endl;
    hide(); //exits the app
}

void MyGui::onButtonClickNew() {
    deleteCity();
    cout << "new button clicked" << endl;
    mArea.clear();
}

void MyGui::onButtonClickOpen() {
    cout << "open button clicked" << endl;
    //mArea.clear();

    Gtk::FileChooserDialog dialog("Please choose a file",
            Gtk::FILE_CHOOSER_ACTION_OPEN);
    //dialog.set_transient_for(*this); //if i remove this, Open works...

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
            std::cout << "Open clicked." << std::endl;

            //Notice that this is a std::string, not a Glib::ustring.
            std::string filename = dialog.get_filename();
            std::cout << "File selected: " <<  filename << std::endl;
            
            char* fileChar = const_cast<char*>(filename.c_str());
            
            lecture(fileChar); //this does not work!
            mArea.draw();
            //delete fileChar;
            break;
        }

        case(Gtk::RESPONSE_CANCEL): {
            std::cout << "Cancel clicked." << std::endl;
            break;
        }
        
        default: {
            std::cout << "Unexpected button clicked." << std::endl;
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
            std::cout << "Save clicked." << std::endl;

            std::string filename = dialog.get_filename();
            std::cout << "File selected for save: " <<  filename << std::endl;
            
            saveCity(filename);
            break;
        }

        case(Gtk::RESPONSE_CANCEL): {
            std::cout << "Cancel clicked." << std::endl;
            break;
        }

        default: {
            std::cout << "Unexpected button clicked." << std::endl;
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