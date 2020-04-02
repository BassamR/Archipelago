#ifndef GUI_HEADER_H
#define GUI_HEADER_H

#include <gtkmm/window.h>
#include <gtkmm/box.h>
#include <gtkmm/button.h>
#include <gtkmm/label.h>
#include <gtkmm/frame.h>
#include <gtkmm/buttonbox.h>
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
    Gtk::Box mBox, mBoxLeft, mBoxRight, mBoxDisplay;
    MyArea mArea;

    Gtk::Frame mFrameCanvas, mFrameGeneral, mFrameDisplay, mFrameEditor, mFrameInformations;

    Gtk::Button mButtonExit, mButtonNew, mButtonOpen, mButtonSave, mButtonShortest, 
        mButtonZoomIn, mButtonZoomOut, mButtonZoomR, mButtonEditLink;

    Gtk::Label mLabelENJ, mLabelCI, mLabelMTA, mLabelZoom;

    Gtk::ButtonBox mBBGeneral, mBBDisplay, mBBEditor, mBBInformations;

public:
    MyGui();
    virtual ~MyGui();
};

#endif