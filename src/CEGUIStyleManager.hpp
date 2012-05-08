#pragma once

//! Since we may eventually change our CEGUI scheme, access scheme-based
//! information through this static class.
namespace CEGUIStyleManager
{
	inline const char* getScheme() { return "WindowsLook.scheme"; }
	inline const char* getButtonName() { return "WindowsLook/Button"; }
	inline const char* getCheckboxName() { return "WindowsLook/Checkbox"; }
	inline const char* getStaticTextName() { return "WindowsLook/StaticText"; }
	inline const char* getSliderName() { return "WindowsLook/Slider"; }
	inline const char* getDropListName() { return "WindowsLook/Combobox"; }
	inline const char* getRadioButtonName() { return "WindowsLook/RadioButton"; }
};
