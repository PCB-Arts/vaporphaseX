#ifndef MODAL_HPP
#define MODAL_HPP

#include <queue>
#include <touchgfx/widgets/Button.hpp>
#include <touchgfx/widgets/TextArea.hpp>
#include <touchgfx/containers/ModalWindow.hpp>

namespace modal {

struct ModalElements {
    touchgfx::ModalWindow* modalWindowInformation;
    touchgfx::Button* modalWindowsInformationClose;
    touchgfx::TextArea* modalWindowInformationBody;
    touchgfx::TextArea* modalWindowInformationTitle;
    touchgfx::ModalWindow* modalWindowDecision;
    touchgfx::Button* modalWindowDecisionYes;
    touchgfx::Button* modalWindowDecisionNo;
    touchgfx::TextArea* modalWindowDecisionBody;
    touchgfx::TextArea* modalWindowDecisionTitle;
};

typedef void (*ModalInformationCallback)();
typedef void (*ModalDecisionCallback)(bool answer);

enum class ShowInformationType {
	INFO,
	INFO_NON_QUITTABLE,
	DECISION,
};

struct ShowInformation {
	ShowInformationType type;
	touchgfx::TypedText title;
	touchgfx::TypedText body;

	ModalInformationCallback informationCallback;
	ModalDecisionCallback decisionCallback;
};

class Modal {
	ModalElements elements;
	std::queue<ShowInformation> sheduledModalDisplays;

	bool showing = false;
	ShowInformation activeModalInformation;

	void showModal(const ShowInformation& info) {
		showing = true;
		activeModalInformation = info;

		elements.modalWindowDecision->setVisible(false);
		elements.modalWindowInformation->setVisible(false);

		if(info.type == ShowInformationType::INFO || info.type == ShowInformationType::INFO_NON_QUITTABLE) {
			elements.modalWindowInformationTitle->setTypedText(activeModalInformation.title);
			elements.modalWindowInformationBody->setTypedText(activeModalInformation.body);
			elements.modalWindowInformation->setVisible(true);
			elements.modalWindowsInformationClose->setVisible(info.type == ShowInformationType::INFO);
		} else {
			elements.modalWindowDecisionTitle->setTypedText(activeModalInformation.title);
			elements.modalWindowDecisionBody->setTypedText(activeModalInformation.body);
			elements.modalWindowDecision->setVisible(true);
		}

		elements.modalWindowDecision->invalidate();
		elements.modalWindowInformation->invalidate();
	}

	void pushInfo(const ShowInformation& info) {
		if (showing) {
			sheduledModalDisplays.push(info);
			return;
		}

		showModal(info);
	}

	void executeCallback(bool answer) {
		if (activeModalInformation.type == ShowInformationType::DECISION) {
			if (activeModalInformation.decisionCallback != NULL) {
				activeModalInformation.decisionCallback(answer);
			}
		} else {
			if (activeModalInformation.informationCallback != NULL) {
				activeModalInformation.informationCallback();
			}
		}
	}

public:
	Modal() {

	}

	Modal(ModalElements elements) {
		this->elements = elements;
	}

	void sheduleInformationModal(touchgfx::TypedText title, touchgfx::TypedText body, ModalInformationCallback callback = NULL) {
		ShowInformation info = {
			.type = ShowInformationType::INFO,
			.title = title,
			.body = body,
			.informationCallback = callback,
			.decisionCallback = NULL
		};

		pushInfo(info);
	}

	void sheduleDecisionModal(touchgfx::TypedText title, touchgfx::TypedText body, ModalDecisionCallback callback = NULL) {
		ShowInformation info = {
			.type = ShowInformationType::DECISION,
			.title = title,
			.body = body,
			.informationCallback = NULL,
			.decisionCallback = callback
		};

		pushInfo(info);
	}

	void sheduleNonQuittableModal(touchgfx::TypedText title, touchgfx::TypedText body) {
		ShowInformation info = {
			.type = ShowInformationType::INFO_NON_QUITTABLE,
			.title = title,
			.body = body,
			.informationCallback = NULL,
			.decisionCallback = NULL
		};

		pushInfo(info);
	}

	void closeModal(bool answer) {
		elements.modalWindowDecision->setVisible(false);
		elements.modalWindowDecision->invalidate();

		elements.modalWindowInformation->setVisible(false);
		elements.modalWindowInformation->invalidate();

		executeCallback(answer);

		if (sheduledModalDisplays.empty()) {
			showing = false;
			return;
		}

		showModal(sheduledModalDisplays.front());
		sheduledModalDisplays.pop();
	}

	/* display info, but only if no other modal is open, not quittable by user */
	void showOverlayInfo(touchgfx::TypedText title, touchgfx::TypedText body) {
		if(showing) {
			return;
		}

		elements.modalWindowInformationTitle->setTypedText(title);
		elements.modalWindowInformationBody->setTypedText(body);
		elements.modalWindowInformation->setVisible(true);
		elements.modalWindowsInformationClose->setVisible(false);

		elements.modalWindowInformation->invalidate();
	}

	void hideOverlayInfo() {
		if(showing) {
			return;
		}


		elements.modalWindowInformation->setVisible(false);
		elements.modalWindowInformation->invalidate();
	}
};

}

#endif
