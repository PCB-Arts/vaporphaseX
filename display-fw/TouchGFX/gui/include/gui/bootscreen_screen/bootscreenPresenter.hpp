#ifndef BOOTSCREENPRESENTER_HPP
#define BOOTSCREENPRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class bootscreenView;

class bootscreenPresenter : public touchgfx::Presenter, public ModelListener
{
public:
    bootscreenPresenter(bootscreenView& v);

    /**
     * The activate function is called automatically when this screen is "switched in"
     * (ie. made active). Initialization logic can be placed here.
     */
    virtual void activate();

    /**
     * The deactivate function is called automatically when this screen is "switched out"
     * (ie. made inactive). Teardown functionality can be placed here.
     */
    virtual void deactivate();

    virtual ~bootscreenPresenter() {};

private:
    bootscreenPresenter();

    bootscreenView& view;
};

#endif // BOOTSCREENPRESENTER_HPP
