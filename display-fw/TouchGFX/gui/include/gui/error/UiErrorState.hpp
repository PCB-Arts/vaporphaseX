#ifndef UI_ERROR_STATE_HPP
#define UI_ERROR_STATE_HPP

#include <gui/modal/Modal.hpp>
#include <touchgfx/widgets/TextArea.hpp>
#include <vector>

typedef bool (*ErrorStateCheck)();

class UiErrorState {
	struct Error {
		bool errorActive = false;
		bool errorReported = false;
		bool errorQuittable = true;
		ErrorStateCheck checkFunction;
		touchgfx::TypedText title;
		touchgfx::TypedText body;

		Error(ErrorStateCheck checkFunction, touchgfx::TypedText title, touchgfx::TypedText body)
			: checkFunction(checkFunction), title(title), body(body) {}
	};

	std::vector<Error> errorDefinitions;

	modal::Modal* modal;
public:
	UiErrorState();
	void setModal(modal::Modal* modal) { this->modal = modal; }
	void ErrorStateWorker();
};

#endif
