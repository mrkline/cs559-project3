#pragma once

//! Since we may eventually change our CEGUI scheme, access scheme-based
//! information through this static class.
class CEGUIStyleManager
{
public:
	static const char* getScheme() { return "WindowsLook.scheme"; }
	static const char* getButtonName() { return "WindowsLook/Button"; }
	static const char* getCheckboxName() { return "WindowsLook/Checkbox"; }
	static const char* getStaticTextName() { return "WindowsLook/StaticText"; }
	static const char* getSliderName() { return "WindowsLook/Slider"; }
	static const char* getDropListName() { return "WindowsLook/Combobox"; }
	static const char* getRadioButtonName() { return "WindowsLook/RadioButton"; }
};
