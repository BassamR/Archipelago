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

class MyArea: public Gtk::DrawingArea {
private:
    bool empty;
    void refresh();

    int width;
    int height; //dimensions of canvas

    double xMin;
    double xMax;
    double yMin;
    double yMax;

protected:
    // Override default signal handler:
    bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr) override;

    //Override mouse events
    bool on_button_press_event(GdkEventButton* event);
    bool on_button_release_event(GdkEventButton* event);
    //bool on_motion_notify_event(GdkEventMotion* event);

    void convertCoordsToModele(Coords& clickLocation);

    void handleLeftClick(Coords clickLocation);
    void handleRightClick(Coords clickLocation);

public:
    MyArea();
    virtual ~MyArea();

    void clear(); //equivalent de New sur l'interface
    void draw(); //equivalent de Open sur l'interface
};

class MyGui: public Gtk::Window {
private:
    void setupGui();
    void initGeneral();
    void initDisplay();
    void initEditor();
    void initInformations();
    void connectButtons();

    void refreshCriteres();

protected:
    Gtk::Box mBox, mBoxLeft, mBoxRight, mBoxDisplay, mBoxEditor;

    MyArea mArea;

    Gtk::Frame mFrameCanvas, mFrameGeneral, mFrameDisplay, mFrameEditor, mFrameInformations;

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
    void onTButtonReleaseShortest(); //maybe make these 2 static?

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

public:
    MyGui();
    virtual ~MyGui();

};

#endif