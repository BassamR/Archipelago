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
#include "gui_mouse_buttons.h"
#include "ville.h"
#include "graphic_gui.h"
#include "constantes.h"
using namespace std;

//Static functions and variables
static string convertCritereToString(const double& critere);
static Gui* guiObject(nullptr);
static Ville* villeObject(Ville::getVilleInstance());
static unsigned int uidCounter(0); //used to manage uid's of created nodes
static constexpr double initialScale(1.0);

//Canvas constructor/destructor:
Canvas::Canvas(): empty(false), shortestPathPressed(false), editLinkPressed(false),
    housingPressed(true), transportPressed(false), productionPressed(false),
    scale(initialScale) {
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

    if(allocation.get_width() != allocation.get_height()) {
        int width(allocation.get_width()), height(allocation.get_width());

        if(allocation.get_width() > allocation.get_height()) {
            width = allocation.get_height();
            height = allocation.get_height();
        }

        frame.width = width;
        frame.height = height;
        xc = frame.width/2;
        yc = frame.height/2;
    }

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
    if(event->type == GDK_BUTTON_PRESS and event->button == GTK_MOUSE_LEFT) {
        double x = event->x;
        double y = event->y;

        Coords leftClickLocation{x, y};
        convertCoordsToModele(leftClickLocation);
        pressPoint = leftClickLocation;
        return true;
    }

    return true;
}

bool Canvas::on_button_release_event(GdkEventButton* event) {
    if(event->type == GDK_BUTTON_RELEASE and event->button == GTK_MOUSE_LEFT) {
        double x = event->x;
        double y = event->y;

        Coords leftClickLocation{x, y};
        convertCoordsToModele(leftClickLocation);
        releasePoint = leftClickLocation;
        handleLeftClick();
        return true;
    }

    if(event->type == GDK_BUTTON_RELEASE and event->button == GTK_MOUSE_RIGHT) {
        double x = event->x;
        double y = event->y;

        Coords rightClickLocation{x, y};
        convertCoordsToModele(rightClickLocation);
        handleRightClick(rightClickLocation);
        return true;
    }

    return true;
}

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
            villeObject->changeActiveNodeSize(pressPoint, releasePoint);
            guiObject->refreshCriteres();
            draw();
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
        }
        draw();
        return;
    }

    if(villeObject->getActiveNode() != noActiveNode) {
        if(villeObject->clickOnNode(pressPoint)) {
            if(not villeObject->clickOnActiveNode(pressPoint)) {
                villeObject->setActiveNode(pressPoint);
            } else {
                villeObject->deleteActiveNode();
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
        villeObject->changeActiveNodeCoords(clickLocation);
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

//Canvas zoom handling
void Canvas::setScale(double value) {
    scale = value;
}

double Canvas::getScale() {
    return scale;
}

void Canvas::multiplieFrame() {
    frame.xMin = -dim_max/scale;
    frame.xMax = dim_max/scale;
    frame.yMin = -dim_max/scale;
    frame.yMax = dim_max/scale;
}

void Canvas::resetFrame() {
    scale = initialScale;

    frame.xMin = -dim_max;
    frame.xMax = dim_max;
    frame.yMin = -dim_max;
    frame.yMax = dim_max;
}

//Gui constructor/destructor:
Gui::Gui():
    mBox(Gtk::ORIENTATION_HORIZONTAL, 10), mBoxLeft(Gtk::ORIENTATION_VERTICAL, 10),
    mBoxRight(Gtk::ORIENTATION_HORIZONTAL, 10), 
    mBoxDisplay(Gtk::ORIENTATION_VERTICAL, 4), mBoxEditor(Gtk::ORIENTATION_VERTICAL),

    mFrameGeneral("General"), mFrameDisplay("Display"), mFrameEditor("Editor"),
    mFrameInformations("Informations"), 

    mButtonExit("Exit"), mButtonNew("New"), mButtonOpen("Open"), mButtonSave("Save"), 
    mButtonZoomIn("Zoom In (i)"), mButtonZoomOut("Zoom Out (o)"), 
    mButtonZoomR("Zoom Reset (r)"),

    mTButtonShortest("Shortest Path"), mTButtonEditLink("Edit Link"),

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
    mArea.resetFrame();
    mLabelZoom.set_text(string("zoom: ") + string("x") + to_string(initialScale));
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
            
            mArea.resetFrame();
            mLabelZoom.set_text(string("zoom: ") + string("x") + to_string(initialScale));
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
    double currentScale = mArea.getScale();

    if(currentScale >= max_zoom) {
        mLabelZoom.set_text(string("zoom: ") + string("x") + to_string(currentScale));
        return;
    }

    mArea.setScale(currentScale+delta_zoom);
    mArea.multiplieFrame();
    decrementLineWidth();
    mArea.draw();
    mLabelZoom.set_text(string("zoom: ") + string("x") 
        + to_string(currentScale+delta_zoom));
}

void Gui::onButtonClickZoomOut() {
    double currentScale = mArea.getScale();
    
    if(currentScale >= min_zoom-epsil_zero and currentScale <= min_zoom+epsil_zero) {
        mLabelZoom.set_text(string("zoom: ") + string("x") + to_string(currentScale));
        return;
    }

    mArea.setScale(currentScale-delta_zoom);
    mArea.multiplieFrame();
    incrementLineWidth();
    mArea.draw();
    mLabelZoom.set_text(string("zoom: ") + string("x") 
        + to_string(currentScale-delta_zoom));
}

void Gui::onButtonClickZoomR() {
    mArea.resetFrame();
    resetLineWidth();
    mArea.draw();
    mLabelZoom.set_text(string("zoom: ") + string("x") + to_string(mArea.getScale()));
}

//Shortest path togglebutton
void Gui::onTButtonClickShortest() {
    if(mTButtonShortest.get_active()) onTButtonPressShortest();
    else onTButtonReleaseShortest();
}

void Gui::onTButtonPressShortest() {
    mArea.setShortestPathPressed(true);
    mArea.draw();
}

void Gui::onTButtonReleaseShortest() {
    mArea.setShortestPathPressed(false);
    mArea.draw();
}

//Gui edit link togglebutton
void Gui::onTButtonClickEditLink() {
    if(mTButtonEditLink.get_active()) onTButtonPressEditLink();
    else onTButtonReleaseEditLink();
}

void Gui::onTButtonPressEditLink() {
    mArea.setEditLinkPressed(true);
}

void Gui::onTButtonReleaseEditLink() {
    mArea.setEditLinkPressed(false);
}

//Gui housing radiobutton
void Gui::onRButtonClickH() {
    if(mRButtonH.get_active()) onRButtonPressH();
    else onRButtonReleaseH();
}

void Gui::onRButtonPressH() {
    mArea.setHousingButtonPressed(true);
}

void Gui::onRButtonReleaseH() {
    mArea.setHousingButtonPressed(false);
}

//Gui transport radiobutton
void Gui::onRButtonClickT() {
    if(mRButtonT.get_active()) onRButtonPressT();
    else onRButtonReleaseT();
}

void Gui::onRButtonPressT() {
    mArea.setTransportButtonPressed(true);
}

void Gui::onRButtonReleaseT() {
    mArea.setTransportButtonPressed(false);
}

//Gui production radiobutton 
void Gui::onRButtonClickP() {
    if(mRButtonP.get_active()) onRButtonPressP();
    else onRButtonReleaseP();
}

void Gui::onRButtonPressP() {
    mArea.setProductionButtonPressed(true);
}

void Gui::onRButtonReleaseP() {
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
    mBox.pack_start(mBoxLeft, Gtk::PACK_SHRINK);
    mBox.pack_start(mBoxRight);

    //Start canvas
    mArea.set_size_request(default_drawing_size, default_drawing_size);
    mBoxRight.pack_start(mFrameCanvas, true, true); //bool expand, bool fill
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
    mLabelZoom.set_text(string("zoom: ") + string("x") + to_string(mArea.getScale()));
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

//Keyboard handling:
bool Gui::on_key_press_event(GdkEventKey* keyEvent) {
    if(keyEvent->type == GDK_KEY_PRESS) {
        switch(gdk_keyval_to_unicode(keyEvent->keyval)) {
            case 'i':
                onButtonClickZoomIn();
                break;

            case 'o':
                onButtonClickZoomOut();
                break;

            case 'r':
                onButtonClickZoomR();
                break;

            default: break;
        }
    }
    return Gtk::Window::on_key_press_event(keyEvent);
}

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