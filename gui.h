/**
* \name gui.h
* \author Hugo Masson, Bassam El Rawas (Sciper 314886, 310635)
* \date May 2020
* \version 1.0
*/

//Architecture: Fig 11 b1

#ifndef GUI_HEADER_H
#define GUI_HEADER_H

#include <gtkmm/window.h>
#include <gtkmm/box.h>
#include <gtkmm/button.h>
#include <gtkmm/label.h>
#include <gtkmm/frame.h>
#include <gtkmm/buttonbox.h>
#include <gtkmm/togglebutton.h>
#include <gtkmm/radiobutton.h>
#include <gtkmm/drawingarea.h>
#include "tools.h"

struct Frame {
    int width;
    int height; //dimensions of canvas

    double xMin;
    double xMax;
    double yMin;
    double yMax; //dimensions of model space
};

class Canvas: public Gtk::DrawingArea {
private:
    bool empty;
    void refresh();

    Frame frame;

    bool shortestPathPressed;
    bool editLinkPressed;
    bool housingPressed;
    bool transportPressed;
    bool productionPressed;

    Coords pressPoint;
    Coords releasePoint;

    // Override default signal handler:
    bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr) override;

    //Override mouse events
    bool on_button_press_event(GdkEventButton* event) override;
    bool on_button_release_event(GdkEventButton* event) override;

    void convertCoordsToModele(Coords& clickLocation);

    void handleLeftClick();
    void handleRightClick(Coords clickLocation);

    void createNode();

public:
    Canvas();
    virtual ~Canvas();

    void clear();
    void draw();

    void setShortestPathPressed(bool value);
    void setEditLinkPressed(bool value);

    void setHousingButtonPressed(bool value);
    void setTransportButtonPressed(bool value);
    void setProductionButtonPressed(bool value);
};

class Gui: public Gtk::Window {
private:
    Gtk::Box mBox, mBoxLeft, mBoxRight, mBoxDisplay, mBoxEditor;

    Canvas mArea;

    Gtk::Frame mFrameCanvas, mFrameGeneral, mFrameDisplay, 
            mFrameEditor, mFrameInformations;

    Gtk::Button mButtonExit, mButtonNew, mButtonOpen, mButtonSave, mButtonZoomIn,
            mButtonZoomOut, mButtonZoomR;

    Gtk::ToggleButton mTButtonShortest, mTButtonEditLink;

    Gtk::Label mLabelCriteres, mLabelZoom;

    Gtk::ButtonBox mBBGeneral, mBBDisplay, mBBEditor, mBBInformations;

    Gtk::RadioButton mRButtonH, mRButtonT, mRButtonP;

    void onButtonClickExit();
    void onButtonClickNew();
    void onButtonClickOpen();
    void onButtonClickSave();
    void onButtonClickZoomIn();
    void onButtonClickZoomOut();
    void onButtonClickZoomR();

    void onTButtonClickShortest();
    void onTButtonPressShortest();
    void onTButtonReleaseShortest();

    void onTButtonClickEditLink();
    void onTButtonPressEditLink();
    void onTButtonReleaseEditLink();

    void onRButtonClickH();
    void onRButtonPressH();
    void onRButtonReleaseH();

    void onRButtonClickT();
    void onRButtonPressT();
    void onRButtonReleaseT();

    void onRButtonClickP();
    void onRButtonPressP();
    void onRButtonReleaseP();

    void setupGui();
    void initGeneral();
    void initDisplay();
    void initEditor();
    void initInformations();
    void connectButtons();

public:
    Gui();
    virtual ~Gui();

    void refreshCriteres();
};

#endif