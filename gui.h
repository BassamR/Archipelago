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

class MyArea: public Gtk::DrawingArea {
protected:
    bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr) override;

public:
    MyArea();
    virtual ~MyArea();
};

class MyGui: public Gtk::Window {
private:
    //stuff

protected:
    Gtk::Box mBox, mBoxLeft, mBoxRight, mBoxDisplay, mBoxEditor;
    MyArea mArea;

    Gtk::Frame mFrameCanvas, mFrameGeneral, mFrameDisplay, mFrameEditor, mFrameInformations;

    Gtk::Button mButtonExit, mButtonNew, mButtonOpen, mButtonSave, mButtonZoomIn, 
            mButtonZoomOut, mButtonZoomR;

    Gtk::ToggleButton mTButtonShortest, mTButtonEditLink;

    Gtk::Label mLabelENJ, mLabelCI, mLabelMTA, mLabelZoom;

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