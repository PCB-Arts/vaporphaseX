#include <core-communication.hpp>
#include <gui/error/UiErrorState.hpp>

#include <texts/TextKeysAndLanguages.hpp>


UiErrorState::UiErrorState() {
	auto lidBlockedError = Error(
			[](){
				auto coreState = CoreCom_CoreState();
				return coreState.error_lid_ocp;
			},
			touchgfx::TypedText(T_LIDBLOCKEDTITLE),
			touchgfx::TypedText(T_LIDBLOCKEDBODY)
	);
	lidBlockedError.errorQuittable = false;

	errorDefinitions.push_back(lidBlockedError);

	auto heaterOtpError = Error(
			[](){
				auto coreState = CoreCom_CoreState();
				return coreState.error_heater_otp;
			},
			touchgfx::TypedText(T_HEATEROTPERRORTITLE),
			touchgfx::TypedText(T_HEATEROTPERRORBODY)
	);
	heaterOtpError.errorQuittable = false;

	errorDefinitions.push_back(heaterOtpError);

	auto coolantOtpError = Error(
			[](){
				auto coreState = CoreCom_CoreState();
				return coreState.error_coolant_otp;
			},
			touchgfx::TypedText(T_COOLANTOTPERRORTITLE),
			touchgfx::TypedText(T_COOLANTOTPERRORBODY)
	);
	coolantOtpError.errorQuittable = false;

	errorDefinitions.push_back(coolantOtpError);

	auto comError = Error(
			[](){
				auto errorState = CoreCom_GetErrorState();
				return errorState.communication_error;
			},
			touchgfx::TypedText(T_COMERRORTITLE),
			touchgfx::TypedText(T_COMERRORBODY)
	);

	errorDefinitions.push_back(comError);
}

void UiErrorState::ErrorStateWorker() {
	for(Error& error : errorDefinitions) {
		error.errorActive = error.checkFunction();

		if(!error.errorActive) {
			error.errorReported = false; // clear reported error
			continue;
		}

		if(error.errorActive && error.errorReported) {
			continue;
		}

		if(error.errorQuittable) {
			modal->sheduleInformationModal(error.title, error.body);
		} else {
			modal->sheduleNonQuittableModal(error.title, error.body);
		}
		error.errorReported = true;
	}
}
