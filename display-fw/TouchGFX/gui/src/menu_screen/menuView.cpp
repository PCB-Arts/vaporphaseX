#include <gui/menu_screen/menuView.hpp>

#include <sstream>

#include "BitmapDatabase.hpp"
#include <texts/TextKeysAndLanguages.hpp>

#include "log.h"
#include "main.h"

#include <regex>

menuView::menuView():
	updateItemCallback(this, &menuView::updateItemCallbackHandler)
{
	modal::ModalElements elements = {
		.modalWindowInformation = &modalWindowInformation,
		.modalWindowsInformationClose = &modalWindowsInformationClose,
		.modalWindowInformationBody = &modalWindowInformationBody,
		.modalWindowInformationTitle = &modalWindowInformationTitle,
		.modalWindowDecision = &modalWindowDecision,
		.modalWindowDecisionYes = &modalWindowDecisionYes,
		.modalWindowDecisionNo = &modalWindowDecisionNo,
		.modalWindowDecisionBody = &modalWindowDecisionBody,
		.modalWindowDecisionTitle = &modalWindowDecisionTitle,
	};
	modalWrapper = modal::Modal(elements);

	errorState.setModal(&modalWrapper);

	options_solder_profile_wheel.setNumberOfItems(0);

	dynamicGraph_ist.clear();
	dynamicGraph_ist.invalidate();
	dynamicGraph_soll.clear();
	dynamicGraph_soll.invalidate();

}


void menuView::setupScreen()
{
	dynamicGraph_ist.setTouchable(false);
	dynamicGraph_soll.setTouchable(false);

	adaptGraphTimeAxis();

    adaptUiToUnits();

    updateButtonStates();
}

void menuView::tearDownScreen()
{
}

bool showingSelftestQuestion = false;

void startReflowDecisionCallback(bool answer) {
	showingSelftestQuestion = false;
	if(answer) {
		CoreCom_StartSelftest();
	}
}
int x;
void menuView::handleTickEvent()
{
	if (textureMapper.isTextureMapperAnimationRunning()==false){
		x+=101.5;
		textureMapper.setupAnimation(AnimationTextureMapper::Z_ROTATION, x, 500, 0, EasingEquations::linearEaseNone);
		textureMapper.startAnimation();
	}


	uint32_t currentTimeMS = HAL_GetTick();
	if(currentTimeMS - lastUiUpdateMS < uiUpdateIntervallMS){
		return;
	}
	lastUiUpdateMS = currentTimeMS;

	updateButtonStates();
	updateCoolantTemperatureLabel();
	updatePCBTemperatureLabel();
	updateGaldenTemperatureLabel();
	updateTemperatureGraph();
	updateSolderProfiles();
	updateLogging();

	errorState.ErrorStateWorker();

	updateSelftest();

	if(HAL_GetTick() % 100 == 0){
	current_state();		// muss immer wieder abgefragt werden, um state zu bekommen, allerdings nicht unbedingt jeden Tick(gerade jeden 100sten). Falls zu langsam 100 runterstellen
	}



	}

inline static void updateVisibility(touchgfx::Widget* widget, bool desiredVisibility) {
	if(widget->isVisible() != desiredVisibility) {
		widget->setVisible(desiredVisibility);
		widget->invalidate();
	}
}

inline static void updateEnabled(touchgfx::Button* button, bool shouldBeEnabled,
		const touchgfx::Bitmap& bmpReleased, const touchgfx::Bitmap& bmpPressed,
		const touchgfx::Bitmap& bmpDisabled) {

	if(button->isTouchable() == shouldBeEnabled) {
		return;
	}

	button->setTouchable(shouldBeEnabled);
	button->setBitmaps(shouldBeEnabled ?  bmpReleased : bmpDisabled,
						bmpPressed);
	button->invalidate();
}

inline static void updateEnabledBitmapsOnly(touchgfx::Button* button, bool shouldBeEnabled,
		const touchgfx::Bitmap& bmpReleased, const touchgfx::Bitmap& bmpPressed,
		const touchgfx::Bitmap& bmpDisabled, bool* enabledState) {

	if(*enabledState == shouldBeEnabled) {
		return;
	}

	button->setBitmaps(	shouldBeEnabled ?  bmpReleased : bmpDisabled,
						shouldBeEnabled ?  bmpPressed : bmpDisabled);
	button->invalidate();

	*enabledState = shouldBeEnabled;
}

void menuView::updateButtonStates() {
	Vpo_CoreStateTypeDef coreState = CoreCom_CoreState();

	updateVisibility(&Start_Reflow, !coreState.reflow_active);
	updateEnabledBitmapsOnly(&Start_Reflow, coreState.can_start_reflow && isProfileSelected,
			touchgfx::Bitmap(BITMAP_START_REFLOW_ID), touchgfx::Bitmap(BITMAP_START_REFLOW_PUSH_ID),
						touchgfx::Bitmap(BITMAP_START_REFLOW_DIABLED_ID),
						&startReflowEnabled);

	updateEnabled(&Stop_Reflow, coreState.can_stop_reflow,
			touchgfx::Bitmap(BITMAP_STOP_REFLOW_ID), touchgfx::Bitmap(BITMAP_STOP_REFLOW_PUSH_ID),
									touchgfx::Bitmap(BITMAP_STOP_REFLOW_DISABLED_ID));

	updateVisibility(&Open_Lid, coreState.lid_state != LID_STATE_OPEN && coreState.lid_state != LID_STATE_CLOSING);
	updateEnabledBitmapsOnly(&Open_Lid, coreState.can_open_lid && coreState.lid_state == LID_STATE_CLOSED,
			touchgfx::Bitmap(BITMAP_OPEN_LID_ID), touchgfx::Bitmap(BITMAP_OPEN_LID_PUSH_ID),
			touchgfx::Bitmap(BITMAP_OPEN_LID_DISABLED_ID),
			&openLidEnabled);

	updateEnabled(&Close_Lid, coreState.lid_state == LID_STATE_OPEN,
			touchgfx::Bitmap(BITMAP_CLOSE_LID_ID), touchgfx::Bitmap(BITMAP_CLOSE_LID_PUSH_ID),
			touchgfx::Bitmap(BITMAP_CLOSE_LID_DISABLED_ID));

	updateEnabled(&options_solder_button_set, !coreState.reflow_active,
			touchgfx::Bitmap(BITMAP_BUTTON_SET_ID), touchgfx::Bitmap(BITMAP_BUTTON_SET_PUSH_ID),
			touchgfx::Bitmap(BITMAP_BUTTON_SET_DISABLE_ID));

	updateVisibility(&options_sd_card_button, SD_Mounted());
	updateVisibility(&options_sd_card_button_mount, !SD_Mounted());
}

void menuView::updateCoolantTemperatureLabel() {
	static uint32_t lastUpdateTime = 0;
	uint32_t currentTimeMS = HAL_GetTick();
	if(currentTimeMS - lastUpdateTime < coolantTempUpdateIntervalMs){
		return;
	}
	lastUpdateTime = currentTimeMS;

	DegreeCelcius coolant_temperature_celsius = CoreCom_CoolantTemperature() / 1000;
	float temp = celsiusToUnit(coolant_temperature_celsius, currentUnit);

	Unicode::snprintfFloat(watersystem_tmpBuffer1, WATERSYSTEM_TMPBUFFER1_SIZE,"%.0f", temp);
	watersystem_tmp.resizeToCurrentText();
	watersystem_tmp.invalidate();
}

void menuView::updatePCBTemperatureLabel() {
	static uint32_t lastUpdateTime = 0;
	uint32_t currentTimeMS = HAL_GetTick();
	if(currentTimeMS - lastUpdateTime < pcbTempUpdateIntervalMs){
		return;
	}
	lastUpdateTime = currentTimeMS;

	DegreeCelcius pcb_temperature_celsius = CoreCom_PcbTemperature() / 1000;
	float temp = celsiusToUnit(pcb_temperature_celsius, currentUnit);

	Unicode::snprintfFloat(pcb_tmpBuffer1, PCB_TMPBUFFER1_SIZE,"%.0f", temp);
	pcb_tmp.resizeToCurrentText();
	pcb_tmp.invalidate();
}

void menuView::updateGaldenTemperatureLabel() {
	DegreeCelcius galden_temperature_celcius = CoreCom_LiquidTemperature() / 1000;
	float temp = celsiusToUnit(galden_temperature_celcius, currentUnit);

	Unicode::snprintfFloat(galden_tmpBuffer1, GALDEN_TMPBUFFER1_SIZE,"%.0f", temp);
	galden_tmp.resizeToCurrentText();
	galden_tmp.invalidate();

}

/*
 * To update the graph values, we get the time from the core controller.
 * We then fill all unknown values up until that time with interpolated values.
 * In normal operation we sample more often, so there should be just samples values.
 */
void menuView::updateTemperatureGraph() {
	DegreeCelcius interpolate_to = CoreCom_PcbTemperature() / 1000;

	int time = CoreCom_CoreOperationTime();

	// negative times are invalid no update
	if(time < 0) {
		return;
	}

	int target_index = CoreCom_CoreOperationTime() / 1000;
	if (target_index < 0) {
		return;
	}

	if (target_index >= profileDurationS) {
		target_index = profileDurationS - 1;
	}

	// turnaround happend
	if (target_index < measureIndex - 1) {
		measureIndex = 0;
		dynamicGraph_ist.clear();
		dynamicGraph_ist.invalidate();
	}

	float interpolate_from = measureIndex <= 0 ? interpolate_to : measuredTemperaturesS[measureIndex - 1];

	int number_of_steps = (target_index - measureIndex) + 1;

	float step = number_of_steps <= 0 ? 0 : (interpolate_to - interpolate_from) / number_of_steps;

	for(; measureIndex <= target_index; measureIndex++) {
		interpolate_from += step;

		measuredTemperaturesS[measureIndex] = interpolate_from;
		dynamicGraph_ist.addDataPoint(celsiusToUnit(interpolate_from, currentUnit));
	}
}

void menuView::updateLogging(){
	if(CoreCom_HasNewMonitor() && !CoreCom_CoreState().selftest_active){
		auto monitor = CoreCom_GetMonitorState();

		CoreCom_ReadMonitor();

		/*
		std::stringstream stream;
		stream.precision(2);

		stream << "HEATER1_TEMP:" << monitor.heater1_temp << ";";
		stream << "HEATER2_TEMP:" << monitor.heater2_temp << ";";
		stream << "GALDEN_TEMP:" << monitor.galden_temp << ";";
		stream << "PCB_TEMP:" << monitor.pcb_temp << ";";
		stream << "COOLANT_TEMP:" << monitor.coolant_temp << ";";

		vpo_log(stream.str().c_str());
		*/

		vpo_log("HEATER1_TEMP:%d", monitor.heater1_temp);
		vpo_log("HEATER2_TEMP:%d", monitor.heater2_temp);
		vpo_log("GALDEN_TEMP:%d", monitor.galden_temp);
		vpo_log("PCB_TEMP:%d", monitor.pcb_temp);
		vpo_log("COOLANT_TEMP:%d", monitor.coolant_temp);

		vpo_log("LID_COOLING_FAN:%d", monitor.lid_cooling_fan_on);
		vpo_log("COOLANT_PUMP:%d", monitor.coolant_pump_on);
		vpo_log("QC_FANS:%d", monitor.quick_cool_fan_on);
		vpo_log("RADIATOR_FANS:%d", monitor.radiator_fan_speed);


		vpo_log("LID_POS:%d", monitor.lid_position);
		vpo_log("LIFT_POS:%d", monitor.lift_position);

		vpo_log("HEATER:%d", monitor.heater_on);
		vpo_log("HEATER_OTP:%d", monitor.heater_otp_active);
		vpo_log("COOLANT_OTP:%d", monitor.coolant_otp_active);
		vpo_log("LID_LOCK:%d", monitor.lid_lock_active);

	}
}

void menuView::updateSelftest() {
	Vpo_CoreStateTypeDef state = CoreCom_CoreState();

	if(HAL_GPIO_ReadPin(SELFTEST_GPIO_Port, SELFTEST_Pin) == GPIO_PIN_RESET && !showingSelftestQuestion) {
		showingSelftestQuestion = true;

	    modalWrapper.sheduleDecisionModal(
	    		touchgfx::TypedText(T_SELFTESTTITLE),
				touchgfx::TypedText(T_SELFTESTQUESTIONBODY),
				startReflowDecisionCallback);
	}

	if(state.selftest_active && !selftestActiveOpened) {
		modalWrapper.showOverlayInfo(
				touchgfx::TypedText(T_SELFTESTTITLE),
				touchgfx::TypedText(T_SELFTESTRUNNINGBODY));

		selftestActiveOpened = true;
	} else if(!state.selftest_active && selftestActiveOpened) {
		modalWrapper.hideOverlayInfo();

		selftestActiveOpened = false;
	}
}


void menuView::TempUnitToggled(){
	//Attention: hacky solution as I cannot find a way to get the displayed state
	if(currentUnit == CELSIUS){
		vpo_log("Fahrenheit selected");
		currentUnit = FAHRENHEIT;
	}else{
		vpo_log("Celsius selected");
		currentUnit = CELSIUS;
	}
	adaptUiToUnits();
}

void menuView::adaptUiToUnits(){
	if(currentUnit == CELSIUS){
		touchgfx::Unicode::snprintf(watersystem_tmpBuffer2, WATERSYSTEM_TMPBUFFER2_SIZE, "%s", "C");
		touchgfx::Unicode::snprintf(galden_tmpBuffer2, GALDEN_TMPBUFFER2_SIZE, "%s", "C");
		touchgfx::Unicode::snprintf(pcb_tmpBuffer2, PCB_TMPBUFFER2_SIZE, "%s", "C");
		touchgfx::Unicode::snprintf(y_axis_namingBuffer, Y_AXIS_NAMING_SIZE, "%s", "C");
	}else{
		touchgfx::Unicode::snprintf(watersystem_tmpBuffer2, WATERSYSTEM_TMPBUFFER2_SIZE, "%s", "F" );
		touchgfx::Unicode::snprintf(galden_tmpBuffer2, GALDEN_TMPBUFFER2_SIZE, "%s", "F");
		touchgfx::Unicode::snprintf(pcb_tmpBuffer2, PCB_TMPBUFFER2_SIZE, "%s", "F");
		touchgfx::Unicode::snprintf(y_axis_namingBuffer, Y_AXIS_NAMING_SIZE, "%s", "F");
	}
	y_axis_naming.invalidate();

	Unicode::snprintfFloat(watersystem_tmpBuffer1, WATERSYSTEM_TMPBUFFER1_SIZE,"%.0f", celsiusToUnit(coolantTemperatureCelsius, currentUnit));
	watersystem_tmp.resizeToCurrentText();
	watersystem_tmp.invalidate();

	Unicode::snprintfFloat(galden_tmpBuffer1, GALDEN_TMPBUFFER1_SIZE,"%.0f", celsiusToUnit(galdenTemperatureCelsius, currentUnit));
	galden_tmp.resizeToCurrentText();
	galden_tmp.invalidate();

	adaptGraphToUnits();
	drawSetHeatProfileGraph();
}

void menuView::drawSetHeatProfileGraph(){
	dynamicGraph_soll.clear();
	for (int i = 0; i < profileDurationS; i++)
	{
		dynamicGraph_soll.addDataPoint(celsiusToUnit(profileTemperaturesS[i], currentUnit));
	}
}

void menuView::adaptGraphToUnits(){
	// Set the range for the x and y axis of the graph. That is
	// the max and min x/y value that can be displayed inside the
	// dimension of the graph.
	dynamicGraph_ist.setGraphRangeY(celsiusToUnit(0, currentUnit), celsiusToUnit(250, currentUnit));
	dynamicGraph_ist.invalidate();

	dynamicGraph_soll.setGraphRangeY(celsiusToUnit(0, currentUnit), celsiusToUnit(250, currentUnit));
	dynamicGraph_soll.invalidate();

	dynamicGraph_ist.clear();
	for(int i = 0; i< measureIndex;i++){
		dynamicGraph_ist.addDataPoint(celsiusToUnit(measuredTemperaturesS[i], currentUnit));
	}
}

void menuView::adaptGraphTimeAxis(){
	dynamicGraph_ist.setGraphRangeX(0, profileDurationS);
	dynamicGraph_ist.invalidate();

	dynamicGraph_soll.setGraphRangeX(0, profileDurationS);
	dynamicGraph_soll.invalidate();
}


void menuView::updateSolderProfiles(){
	int currentSolderProfileVersion = GetFileLoadCounter();
	if(SolderProfilesVersion != currentSolderProfileVersion){
		solderProfiles = GetAvailableCSVFiles();
		SolderProfilesVersion = currentSolderProfileVersion;

		updateScrollWheel();
		isProfileSelected = false;
	}
}

void menuView::updateScrollWheel(){
	if(solderProfileWheelItems != nullptr){
		delete solderProfileWheelItems;
	}
	solderProfileWheelItems = new DynamicDrawableListItems<custom_container_profile_select>(solderProfiles.size());

    volatile int16_t solderProfileSize = solderProfiles.size();
    options_solder_profile_wheel.setNumberOfItems(solderProfileSize);
	options_solder_profile_wheel.setDrawables(*solderProfileWheelItems, updateItemCallback);
	options_solder_profile_wheel.animateToItem(0, 0);

	options_solder_profile_wheel.initialize();
	for (int i = 0; i < solderProfileWheelItems->getNumberOfDrawables(); i++)
	{
		(*solderProfileWheelItems)[i].initialize();
	}
	options_solder_profile_wheel.invalidate();
}

void startErrorCallback() {
	vpo_log("Failure message 'start conditions not met' acknowledged");
}

void menuView::StartButtonPush()
{
	Vpo_CoreStateTypeDef coreState = CoreCom_CoreState();
	vpo_log("Start reflow pushed");

	if(!(coreState.can_start_reflow && isProfileSelected)) {
		modalWrapper.sheduleInformationModal(
				touchgfx::TypedText(T_STARTCONDITIONERRORTITLE),
				touchgfx::TypedText(T_STARTCONDITIONERRORBODY),
				startErrorCallback);
		return;
	}

	CoreCom_StartProfile(selectedProfile);
}

void stopReflowDecisionCallback(bool answer) {
	if(answer) {
		CoreCom_StopReflow();
	}
}

void menuView::StopReflowPush()
{
	vpo_log("Stop reflow pushed");
    modalWrapper.sheduleDecisionModal(
    		touchgfx::TypedText(T_PROCESSCANCELTITLE),
			touchgfx::TypedText(T_PROCESSCANCELBODY),
			stopReflowDecisionCallback);
}

void openLidErrorCallback() {
	vpo_log("Failure message 'open lid conditions not met' acknowledged");
}

void menuView::OpenLidPush()
{
	Vpo_CoreStateTypeDef coreState = CoreCom_CoreState();
	vpo_log("Open lid pushed");

	if(!(coreState.can_open_lid && coreState.lid_state == LID_STATE_CLOSED)) {
		modalWrapper.sheduleInformationModal(
				touchgfx::TypedText(T_LIDCONDITIONERRORTITLE),
				touchgfx::TypedText(T_LIDCONDITIONERRORBODY),
				openLidErrorCallback);
		return;
	}

	CoreCom_OpenLid();
}

void menuView::CloseLidPush()
{
	vpo_log("Closed lid pushed");
    CoreCom_CloseLid();
}

void menuView::EjectPush()
{
    SD_SheduleUnmount();
}

void menuView::MountPush(){
	SD_Mount();
}

void menuView::ModalInformationClose()
{
	modalWrapper.closeModal(false);
}

void menuView::ModalDecisionYes()
{
	modalWrapper.closeModal(true);
}

void menuView::ModalDecisionNo()
{
	modalWrapper.closeModal(false);
}

void menuView::SetSolderProfile(){
	int itemIndex = options_solder_profile_wheel.getSelectedItem();
	if (itemIndex >= solderProfiles.size()){
		updateScrollWheel();
		return;
	}
	isProfileSelected = false;

	dynamicGraph_ist.clear();
	dynamicGraph_ist.invalidate();

	auto profilePath = solderProfiles[itemIndex];
	selectedProfile = loadSolderCsv(profilePath);
	if(!selectedProfile.valid){
		SD_SheduleUnmount();
		return;
	}
	vpo_log("Profile %s selected", profilePath.c_str());
	isProfileSelected = true;

	int profileLength = selectedProfile.timeTemperatureData.size();

	for(int i = 0; i<maxProfileDuration;i++){
		profileTemperaturesS[i] = 0;
	}

	int maxTime = 0;
	int currentTime = 0;
	for(int i = 0; i < selectedProfile.timeTemperatureData.size() - 1; i++){
		float delta_temperature = selectedProfile.timeTemperatureData[i+1].temperature - selectedProfile.timeTemperatureData[i].temperature;
		float delta_time = selectedProfile.timeTemperatureData[i+1].time - selectedProfile.timeTemperatureData[i].time;
		float a = delta_temperature / delta_time;
		float b = selectedProfile.timeTemperatureData[i].temperature;

		for(int j = 0;currentTime < selectedProfile.timeTemperatureData[i+1].time;currentTime++, j++){
			profileTemperaturesS[currentTime] = a * j + b;
		}
	}
	profileTemperaturesS[currentTime] = selectedProfile.timeTemperatureData[selectedProfile.timeTemperatureData.size() - 1].temperature;
	profileDurationS = currentTime;

    CoreCom_SendGalden(selectedProfile);		//Core communication to send Galden Boilingtemperature of .csv
	CoreCom_SendACT(selectedProfile);
    drawSetHeatProfileGraph();
	adaptGraphTimeAxis();

	menu_container.setSelectedPage(1);


}

void menuView::options_solder_profile_wheelUpdateItem(custom_container_profile_select& item, int16_t itemIndex)
{
	if(itemIndex>=solderProfiles.size()){
		std::string errorText = "no solder profile";
		item.setText(errorText);
	}else{
		item.setText(solderProfiles[itemIndex]);
	}
	item.invalidate();
}

void menuView::updateItemCallbackHandler(touchgfx::DrawableListItemsInterface* items, int16_t containerIndex, int16_t itemIndex)
{
    if (items == solderProfileWheelItems)
    {
        touchgfx::Drawable* d = items->getDrawable(containerIndex);
        custom_container_profile_select* cc = (custom_container_profile_select*)d;
        options_solder_profile_wheelUpdateItem(*cc, itemIndex);
    }
}


void menuView::btn_eco_open_temp_clicked()
{
	vpo_log("Open temperature mode: Eco");
	CoreCom_SetLidMode(VPO_ECO);

	options_openlid_button_high.setBitmaps(touchgfx::Bitmap(BITMAP_BUTTON_FAST_ID), touchgfx::Bitmap(BITMAP_BUTTON_FAST_PUSHED_ID));
	options_openlid_button_medium.setBitmaps(touchgfx::Bitmap(BITMAP_BUTTON_STANDARD_ID), touchgfx::Bitmap(BITMAP_BUTTON_STANDARD_PUSHED_ID));
	options_openlid_button_low.setBitmaps(touchgfx::Bitmap(BITMAP_BUTTON_ECO_PUSHED_ID), touchgfx::Bitmap(BITMAP_BUTTON_ECO_PUSHED_ID));

	options_openlid_button_high.invalidate();
	options_openlid_button_medium.invalidate();
	options_openlid_button_low.invalidate();
}

void menuView::btn_standard_open_temp_clicked()
{
	vpo_log("Open temperature mode: Standard");
	CoreCom_SetLidMode(VPO_STANDARD);

	options_openlid_button_high.setBitmaps(touchgfx::Bitmap(BITMAP_BUTTON_FAST_ID), touchgfx::Bitmap(BITMAP_BUTTON_FAST_PUSHED_ID));
	options_openlid_button_medium.setBitmaps(touchgfx::Bitmap(BITMAP_BUTTON_STANDARD_PUSHED_ID), touchgfx::Bitmap(BITMAP_BUTTON_STANDARD_PUSHED_ID));
	options_openlid_button_low.setBitmaps(touchgfx::Bitmap(BITMAP_BUTTON_ECO_ID), touchgfx::Bitmap(BITMAP_BUTTON_ECO_PUSHED_ID));

	options_openlid_button_high.invalidate();
	options_openlid_button_medium.invalidate();
	options_openlid_button_low.invalidate();
}

void menuView::btn_fast_open_temp_clicked()
{
	vpo_log("Open temperature mode: Fast");
	CoreCom_SetLidMode(VPO_FAST);

	options_openlid_button_high.setBitmaps(touchgfx::Bitmap(BITMAP_BUTTON_FAST_PUSHED_ID), touchgfx::Bitmap(BITMAP_BUTTON_FAST_PUSHED_ID));
	options_openlid_button_medium.setBitmaps(touchgfx::Bitmap(BITMAP_BUTTON_STANDARD_ID), touchgfx::Bitmap(BITMAP_BUTTON_STANDARD_PUSHED_ID));
	options_openlid_button_low.setBitmaps(touchgfx::Bitmap(BITMAP_BUTTON_ECO_ID), touchgfx::Bitmap(BITMAP_BUTTON_ECO_PUSHED_ID));

	options_openlid_button_high.invalidate();
	options_openlid_button_medium.invalidate();
	options_openlid_button_low.invalidate();
}


/*
 * set current state on display
 * gets state of Core via usart corecom
 * print string as wildcard which state is currently used, size should be +1
 */
void menuView::current_state(){
	Vpo_CoreStateTypeDef coreState = CoreCom_CoreState();
	if(coreState.current_state == CS_Preheat){
		Unicode::strncpy(current_stateTBuffer,"Preheat", 8);
		current_stateT.resizeToCurrentText();
		current_stateT.invalidate();
	}
	else if(coreState.current_state == CS_Soldering){
		Unicode::strncpy(current_stateTBuffer,"Soldering", 10);
		current_stateT.resizeToCurrentText();
		current_stateT.invalidate();
		}
	else if(coreState.current_state == CS_AntiCondensation){
		Unicode::strncpy(current_stateTBuffer,"Quick cool",11);
		current_stateT.resizeToCurrentText();
		current_stateT.invalidate();
		}
	else if(coreState.current_state == CS_Calibration){
		Unicode::strncpy(current_stateTBuffer,"Calibrating",12);
				current_stateT.resizeToCurrentText();
				current_stateT.invalidate();
	}
	else{
		Unicode::strncpy(current_stateTBuffer, "\0" , 12);
		current_stateT.invalidate();
		Unicode::strncpy(current_stateTBuffer, "Idle", 5);
		current_stateT.resizeToCurrentText();
		current_stateT.invalidate();

	}
}




