/*********************************************************************************/
/********** THIS FILE IS GENERATED BY TOUCHGFX DESIGNER, DO NOT MODIFY ***********/
/*********************************************************************************/
#ifndef BOOTSCREENVIEWBASE_HPP
#define BOOTSCREENVIEWBASE_HPP

#include <gui/common/FrontendApplication.hpp>
#include <mvp/View.hpp>
#include <gui/bootscreen_screen/bootscreenPresenter.hpp>
#include <touchgfx/widgets/Box.hpp>
#include <touchgfx/widgets/TextArea.hpp>
#include <touchgfx/widgets/Image.hpp>

class bootscreenViewBase : public touchgfx::View<bootscreenPresenter>
{
public:
    bootscreenViewBase();
    virtual ~bootscreenViewBase() {}
    virtual void setupScreen();
    virtual void handleTickEvent();
    virtual void afterTransition();

protected:
    FrontendApplication& application() {
        return *static_cast<FrontendApplication*>(touchgfx::Application::getInstance());
    }

    /*
     * Member Declarations
     */
    touchgfx::Box __background;
    touchgfx::Box background;
    touchgfx::TextArea bootscreen_sw_version;
    touchgfx::Image logo;

private:

    /*
     * Delay Variable Declarations
     */
    static const uint16_t BOOTWAIT_DURATION = 210;
    uint16_t bootwaitCounter;

};

#endif // BOOTSCREENVIEWBASE_HPP