#include <gui/bootscreen_screen/bootscreenView.hpp>

bootscreenView::bootscreenView()
{

}

void bootscreenView::setupScreen()
{
	this->setFirmwareVersion();
    bootscreenViewBase::setupScreen();
}

void bootscreenView::tearDownScreen()
{
    bootscreenViewBase::tearDownScreen();
}

void bootscreenView::setFirmwareVersion(){
	touchgfx::Unicode::snprintf(bootscreen_sw_versionBuffer, BOOTSCREEN_SW_VERSION_SIZE,
			"%i.%i", FW_VER_MAJOR, FW_VER_MINOR);
	bootscreen_sw_version.resizeToCurrentTextWithAlignment();
	bootscreen_sw_version.invalidate();
}
