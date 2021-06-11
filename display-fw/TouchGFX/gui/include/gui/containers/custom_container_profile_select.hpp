#ifndef CUSTOM_CONTAINER_PROFILE_SELECT_HPP
#define CUSTOM_CONTAINER_PROFILE_SELECT_HPP

#include <gui_generated/containers/custom_container_profile_selectBase.hpp>

#include <string>

class custom_container_profile_select : public custom_container_profile_selectBase
{
public:
    custom_container_profile_select();
    virtual ~custom_container_profile_select() {}

    void setText(std::string& text){
    	int targetLength = text.length();
    	if(targetLength >= TEXTAREA1_SIZE){
    		targetLength = TEXTAREA1_SIZE - 1;
    	}
    	Unicode::UnicodeChar unicodeText[TEXTAREA1_SIZE];
    	int i = 0;
    	for(; i < targetLength;i++){
    		unicodeText[i] = static_cast<Unicode::UnicodeChar>(text[i]);
    	}
    	for(;i<TEXTAREA1_SIZE;i++){
    		unicodeText[i] = 0;
    	}
    	Unicode::snprintf(textArea1Buffer, TEXTAREA1_SIZE, "%s", unicodeText);
    	textArea1.resizeToCurrentText();
    	textArea1.invalidate();
    }

    virtual void initialize();
};

#endif // CUSTOM_CONTAINER_PROFILE_SELECT_HPP
