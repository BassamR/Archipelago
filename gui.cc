/**
* \name gui.cc
* \author Hugo Masson, Bassam El Rawas (Sciper 314886, 310635)
* \date May 2020
* \version 1.0
*/

//Architecture: Fig 11 b1

#include <iostream>
#include <string>
#include <sstream>
#include <cairomm/context.h>
#include <gtkmm/filechooser.h>
#include <gtkmm/filechooserdialog.h>
#include "gui.h"
#include "ville.h"
#include "graphic_gui.h"
#include "constantes.h"
using namespace std;

//Static functions and variables
static string convertCritereToString(const double& critere);
static Gui* guiObject(nullptr);
static Ville* villeObject(Ville::getVilleInstance());
static int uidCounter(0); //used for rendu3 to properly manage uid's of created nodes

//Canvas constructor/destructor:
Canvas::Canvas(): empty(false), shortestPathPressed(false), editLinkPressed(false) {
    add_events(Gdk::BUTTON_PRESS_MASK);
    add_events(Gdk::BUTTON_RELEASE_MASK);
    
    frame.width = 0;
    frame.height = 0;
    frame.xMin = -dim_max;
    frame.xMax = dim_max;
    frame.yMin = -dim_max;
    frame.yMax = dim_max;
}

Canvas::~Canvas() {}

//Canvas refresh and event override methods:
void Canvas::refresh() {
    auto win = get_window();
    if(win) {
        Gdk::Rectangle r(0,0, get_allocation().get_width(), 
            get_allocation().get_height());
        win->invalidate_rect(r, false); //forces a call to on_draw
    }
}

bool Canvas::on_draw(const Cairo::RefPtr<Cairo::Context>& cr) {
    Gtk::Allocation allocation = get_allocation();
    frame.width = allocation.get_width();
    frame.height = allocation.get_height();

    int xc, yc; //center of the gtk window
    xc = frame.width/2;
    yc = frame.height/2;

    cr->translate(xc, yc);
    cr->scale(frame.width/(frame.xMax - frame.xMin), 
        -frame.height/(frame.yMax - frame.yMin));
	cr->translate(-(frame.xMin + frame.xMax)/2, -(frame.yMin + frame.yMax)/2);

    graphicSetContext(cr);

    if(not empty) {
        makeBgWhite();
        villeObject->drawLinks();
        villeObject->drawNodes();
        if(shortestPathPressed) villeObject->drawShortestPath();
    } else {
        makeBgWhite();
        villeObject->resetVille();
    }

    return true;
}

bool Canvas::on_button_press_event(GdkEventButton* event) {
    //Left click = 1, right click = 3
    if((gint)event->type == Gdk::BUTTON_PRESS and event->button == 1) {
        double x = event->x;
        double y = event->y;

        Coords leftClickLocation{x, y};
        convertCoordsToModele(leftClickLocation);
        pressPoint = leftClickLocation;
        return true;
    }

    if((gint)event->type == Gdk::BUTTON_PRESS and event->button == 3) {
        double x1 = event->x;
        double y1 = event->y;

        Coords rightClickLocation{x1, y1};
        convertCoordsToModele(rightClickLocation);
        handleRightClick(rightClickLocation);

        return true;
    }

    return true;
}

bool Canvas::on_button_release_event(GdkEventButton* event) {
    if((gint)event->type == Gdk::BUTTON_RELEASE and event->button == 1) {
        double x = event->x;
        double y = event->y;

        Coords leftClickLocation{x, y};
        convertCoordsToModele(leftClickLocation);
        releasePoint = leftClickLocation;
        handleLeftClick();
        return true;
    }

    if((gint)event->type == Gdk::BUTTON_RELEASE and event->button == 3) {
        return true;
    }

    return true;
} //mouse event stub for rendu 3

//Canvas utility methods, event handling:
void Canvas::convertCoordsToModele(Coords& clickLocation) {
    double newX = (clickLocation.x/frame.width) 
        * (frame.xMax-frame.xMin) + frame.xMin;
    double newY = frame.yMax - (clickLocation.y/frame.height) 
        * (frame.yMax-frame.yMin);

    clickLocation.x = newX;
    clickLocation.y = newY;
}

void Canvas::handleLeftClick() {
    if(pressPoint.x != releasePoint.x or pressPoint.y != releasePoint.y) {
        if(villeObject->getActiveNode() != noActiveNode) {
            //change size
            cout << "im changing a size of a node" << endl;
            return;
        }
        return;
    }

    if(editLinkPressed) {
        if(villeObject->getActiveNode() == noActiveNode) {
            cout << "need an activeNode to create links" << endl;
            return;
        } else {
            if(villeObject->clickOnNode(pressPoint)) {
                villeObject->handleLink(pressPoint, dist_min);
                guiObject->refreshCriteres();
            } else {
                cout << "cannot create a link with nothingness" << endl;
            }
            draw();
            return;
        }
        return;
    }

    if(villeObject->getActiveNode() == noActiveNode) {
        if(villeObject->clickOnNode(pressPoint)) {
            villeObject->setActiveNode(pressPoint);
        } else {
            createNode();
            guiObject->refreshCriteres();
            villeObject->setActiveNode(pressPoint);
        }
        draw();
        return;
    }

    if(villeObject->getActiveNode() != noActiveNode) {
        if(villeObject->clickOnNode(pressPoint)) {
            if(not villeObject->clickOnActiveNode(pressPoint)) {
                villeObject->setActiveNode(pressPoint);
            } else {
                villeObject->deleteNode(pressPoint);
                guiObject->refreshCriteres();
            }
        } else {
            villeObject->resetActiveNode();
        }
    }

    draw();
}

void Canvas::handleRightClick(Coords clickLocation) {
    int activeNode = villeObject->getActiveNode();
    if(activeNode != noActiveNode) {
        villeObject->getNode(activeNode)->setCoords(clickLocation.x, 
            clickLocation.y);
        guiObject->refreshCriteres();
    }
    draw();
}

void Canvas::createNode() {
    if(housingPressed) {
        villeObject->createHousing(uidCounter, pressPoint.x, pressPoint.y, 
            min_capacity, dist_min);
        ++uidCounter;
        return;
    }
    if(transportPressed) {
        villeObject->createTransport(uidCounter, pressPoint.x, pressPoint.y, 
            min_capacity, dist_min);
        ++uidCounter;
        return;
    }
    if(productionPressed) {
        villeObject->createProduction(uidCounter, pressPoint.x, pressPoint.y, 
            min_capacity, dist_min);
        ++uidCounter;
        return;
    }
}

//Canvas public methods, setters:
void Canvas::clear() {
    empty = true;
    refresh();
}

void Canvas::draw() {
    empty = false;
    refresh();
}

void Canvas::setShortestPathPressed(bool value) {
    shortestPathPressed = value;
}

void Canvas::setEditLinkPressed(bool value) {
    editLinkPressed = value;
}

void Canvas::setHousingButtonPressed(bool value) {
    housingPressed = value;
}

void Canvas::setTransportButtonPressed(bool value) {
    transportPressed = value;
}

void Canvas::setProductionButtonPressed(bool value) {
    productionPressed = value;
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
    uidCounter = villeObject->findBiggestUid();
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
    hide(); //exits the app
}

void Gui::onButtonClickNew() {
    villeObject->resetVille();
    mArea.clear();
    refreshCriteres();
    uidCounter = villeObject->findBiggestUid();
}

void Gui::onButtonClickOpen() {
    Gtk::FileChooserDialog dialog("Please choose a file",
            Gtk::FILE_CHOOSER_ACTION_OPEN);
    //dialog.set_transient_for(*this); 
    //if i add this, Open works on VM but not on windows...

    //Add response buttons the the dialog:
    dialog.add_button("_Cancel", Gtk::RESPONSE_CANCEL);
    dialog.add_button("_Open", Gtk::RESPONSE_OK);

    //Show the dialog and wait for a user response:
    int result = dialog.run();

    //Handle the response:
    switch(result) {
        case(Gtk::RESPONSE_OK): {
            string filename = dialog.get_filename();
            
            char* filenameChar = const_cast<char*>(filename.c_str());
            lecture(filenameChar);
            
            mArea.draw();
            refreshCriteres();
            uidCounter = villeObject->findBiggestUid();
            break;
        }

        case(Gtk::RESPONSE_CANCEL): {
            break;
        }

        default: {
            cout << "Unexpected button clicked." << endl;
            break;
        }
    }
}

void Gui::onButtonClickSave() {
    Gtk::FileChooserDialog dialog("Please choose a file",
            Gtk::FILE_CHOOSER_ACTION_SAVE);
    //dialog.set_transient_for(*this);
    //if i add this, Save works on VM but not on windows...

    dialog.add_button("_Cancel", Gtk::RESPONSE_CANCEL);
    dialog.add_button("_Save", Gtk::RESPONSE_OK);

    int result = dialog.run();

    switch(result) {
        case(Gtk::RESPONSE_OK): {
            string filename = dialog.get_filename();
            villeObject->saveVille(filename);
            break;
        }

        case(Gtk::RESPONSE_CANCEL): {
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
    mArea.setShortestPathPressed(true);
    mArea.draw();
}

void Gui::onTButtonReleaseShortest() {
    cout << "shortest path button released" << endl;
    mArea.setShortestPathPressed(false);
    mArea.draw();
}

//Gui edit link togglebutton
void Gui::onTButtonClickEditLink() {
    if(mTButtonEditLink.get_active()) onTButtonPressEditLink();
    else onTButtonReleaseEditLink();
}

void Gui::onTButtonPressEditLink() {
    cout << "edit link button pressed" << endl;
    mArea.setEditLinkPressed(true);
}

void Gui::onTButtonReleaseEditLink() {
    cout << "edit link button released" << endl;
    mArea.setEditLinkPressed(false);
}

//Gui housing radiobutton
void Gui::onRButtonClickH() {
    if(mRButtonH.get_active()) onRButtonPressH();
    else onRButtonReleaseH();
}

void Gui::onRButtonPressH() {
    cout << "housing radiobutton pressed" << endl;
    mArea.setHousingButtonPressed(true);
}

void Gui::onRButtonReleaseH() {
    cout << "housing radiobutton released" << endl;
    mArea.setHousingButtonPressed(false);
}

//Gui transport radiobutton
void Gui::onRButtonClickT() {
    if(mRButtonT.get_active()) onRButtonPressT();
    else onRButtonReleaseT();
}

void Gui::onRButtonPressT() {
    cout << "transport radiobutton pressed" << endl;
    mArea.setTransportButtonPressed(true);
}

void Gui::onRButtonReleaseT() {
    cout << "transport radiobutton released" << endl;
    mArea.setTransportButtonPressed(false);
}

//Gui production radiobutton 
void Gui::onRButtonClickP() {
    if(mRButtonP.get_active()) onRButtonPressP();
    else onRButtonReleaseP();
}

void Gui::onRButtonPressP() {
    cout << "production radiobutton pressed" << endl;
    mArea.setProductionButtonPressed(true);
}

void Gui::onRButtonReleaseP() {
    cout << "production radiobutton released" << endl;
    mArea.setProductionButtonPressed(false);
}

//Gui methods to better organize gui constructor:
void Gui::setupGui() {
    //Basic definitions
    set_title("Archipelago");
    set_border_width(5);
    set_position(Gtk::WIN_POS_CENTER);

    //Add main box
    add(mBox);

    //Fill main box
    mBox.pack_start(mBoxLeft);
    mBox.pack_start(mBoxRight);

    //Start canvas
    mArea.set_size_request(default_drawing_size, default_drawing_size);
    mBoxRight.pack_start(mFrameCanvas, false, false);
    mFrameCanvas.add(mArea);
}

void Gui::initGeneral() {
    //Add general frame to box
    mBoxLeft.pack_start(mFrameGeneral, false, false);

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

    string enj = convertCritereToString(villeObject->critereENJ());
    string ci = convertCritereToString(villeObject->critereCI());
    string mta = convertCritereToString(villeObject->critereMTA());

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

//Misc MyGui methods:
void Gui::refreshCriteres() {
    string enj = convertCritereToString(villeObject->critereENJ());
    string ci = convertCritereToString(villeObject->critereCI());
    string mta = convertCritereToString(villeObject->critereMTA());

    mLabelCriteres.set_text(string("ENJ: ") + enj + string("\nCI: ") + ci 
            + string("\nMTA: ") + mta);
}

//Static functions definition
string convertCritereToString(const double& critere) {
    stringstream temp;
    temp << critere;
    string convertedCritere = temp.str();
    return convertedCritere;
}