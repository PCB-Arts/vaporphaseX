#ifndef MENUVIEW_HPP
#define MENUVIEW_HPP

#include <core-communication.hpp>
#include <gui_generated/menu_screen/menuViewBase.hpp>
#include <gui/menu_screen/menuPresenter.hpp>
#include <gui/modal/Modal.hpp>
#include <gui/error/UiErrorState.hpp>
#include <unitConverter.hpp>
#include <string>
#include "stm32f4xx.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_uart.h"
#include <sd-card/sd.hpp>
#include <ProfileParser.hpp>
#include <DynamicDrawableListItems.hpp>



class menuView : public menuViewBase
{
public:
    menuView();
    virtual ~menuView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();

    virtual void handleTickEvent();
    virtual void TempUnitToggled();
    virtual void StartButtonPush();
    virtual void StopReflowPush();
    virtual void OpenLidPush();
    virtual void CloseLidPush();
    virtual void MountPush();
    virtual void EjectPush();
    virtual void ModalInformationClose();
    virtual void ModalDecisionYes();
    virtual void ModalDecisionNo();
    virtual void SetSolderProfile();
    virtual void btn_eco_open_temp_clicked();
	virtual void btn_standard_open_temp_clicked();
	virtual void btn_fast_open_temp_clicked();
	virtual void current_state();

    void adaptUiToUnits();
    void adaptGraphToUnits();
    void drawSetHeatProfileGraph();
    void adaptGraphTimeAxis();

    virtual void options_solder_profile_wheelUpdateItem(custom_container_profile_select& item, int16_t itemIndex);

protected:
    TemperatureUnit currentUnit = CELSIUS;
    float galdenTemperatureCelsius = 0.0f;
    float coolantTemperatureCelsius = 0.0f;
    uint32_t lastUiUpdateMS = 0;
    const uint32_t uiUpdateIntervallMS = 30;
    bool solderingActive = true;

    constexpr static int maxProfileDuration = 600;
    int profileDurationS = 600;
    float profileTemperaturesS[maxProfileDuration];
    float measuredTemperaturesS[maxProfileDuration];
    int measureIndex = 0;

    const uint32_t coolantTempUpdateIntervalMs = 3000;
    const uint32_t pcbTempUpdateIntervalMs = 1000;

    modal::Modal modalWrapper;
    UiErrorState errorState;

    bool startReflowEnabled = true;
    bool openLidEnabled = true;
    bool selftestActiveOpened = false;
private:
    void updateButtonStates();
    void updateCoolantTemperatureLabel();
    void updatePCBTemperatureLabel();
    void updateGaldenTemperatureLabel();
    void updateTemperatureGraph();
    void updateSolderProfiles();
    void updateLogging();
    void updateSelftest();

    void updateScrollWheel();

    int SolderProfilesVersion = -1;
    std::vector<std::string> solderProfiles;
    DynamicDrawableListItems<custom_container_profile_select>* solderProfileWheelItems = nullptr;

    bool isProfileSelected = false;
    SolderProfile selectedProfile;

    touchgfx::Callback<menuView, touchgfx::DrawableListItemsInterface*, int16_t, int16_t> updateItemCallback;
//    touchgfx::Callback<menuView, touchgfx::AnimationTextureMapper&> TextureMapperAnimationEndedCallback;

    void updateItemCallbackHandler(touchgfx::DrawableListItemsInterface* items, int16_t containerIndex, int16_t itemIndex);
//    void textureMapperAnimationEndedCallbackHandler(touchgfx::AnimationTextureMapper& src);

};

#endif // MENUVIEW_HPP
