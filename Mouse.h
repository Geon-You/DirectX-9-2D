#pragma once
#include "Define.h"

class Mouse
{
public:
	static POINT GetPos()
	{
		POINT pt;

		GetCursorPos(&pt);
		ScreenToClient(g_hWnd, &pt);

		return pt;
	}
};

