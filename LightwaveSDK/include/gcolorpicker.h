#ifndef LW_GLOB_COLOR_PICKER
#define LW_GLOB_COLOR_PICKER

#define GLOB_COLOR_PICKER_NAME "LW_GCOLOR_PICKER"

typedef struct st_ColorPick_Local {
	const char      *title;
	int              red, green, blue;
	int              result;
} ColorPick_Local;

typedef struct st_gColor_Picker {
	int	(*Multi_Picker)(ColorPick_Local	*);
	int	(*HWB_Picker)(ColorPick_Local	*);
	int	(*HSV_Picker)(ColorPick_Local	*);
	int	(*QuickPicker)(ColorPick_Local	*);
	int	(*Lambda_Picker)(ColorPick_Local	*);
	int	(*Kelvin_Picker)(ColorPick_Local	*);
#ifdef _MACOS
	void (*MacOs_Picker)(ColorPick_Local	*);
#endif
} gColor_Picker;

#endif
