#ifndef BOOTSCREENVIEW_HPP
#define BOOTSCREENVIEW_HPP

#include <gui_generated/bootscreen_screen/bootscreenViewBase.hpp>
#include <gui/bootscreen_screen/bootscreenPresenter.hpp>

class bootscreenView : public bootscreenViewBase
{
public:
    bootscreenView();
    virtual ~bootscreenView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();
protected:
};

#endif // BOOTSCREENVIEW_HPP
