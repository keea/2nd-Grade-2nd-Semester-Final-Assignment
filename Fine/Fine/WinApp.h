#pragma once
#include "MyWnd.h"

class WinApp : public TW_WinApp
{
public:
	WinApp(HINSTANCE ah_instance, const wchar_t *ap_class_name)
		: TW_WinApp(ah_instance, ap_class_name) { };

	virtual void InitInstance();
};

