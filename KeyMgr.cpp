#include "KeyMgr.h"

void KeyMgr::CheckKey()
{
	dwKey = 0;

	// 0 (0x0000)	- ������ ���� ���� ����, ȣ�� �������� �� ���� ����
	// 0x8000		- ������ ���� ���� ����, ȣ�� �������� ���� ����
	// 0x8001		- ������ ���� ���� �ְ�, ȣ�� �������� ���� ����
	// 1 (0x0001)	- ������ ���� ���� �ְ�, ȣ�� �������� �� ���� ����

	if (GetAsyncKeyState('W') & 0x8000)
	{
		dwKey |= KEY_UP;
	}

	if (GetAsyncKeyState('S') & 0x8000)
	{
		dwKey |= KEY_DOWN;
	}

	if (GetAsyncKeyState('A') & 0x8000)
	{
		dwKey |= KEY_LEFT;
	}

	if (GetAsyncKeyState('D') & 0x8000)
	{
		dwKey |= KEY_RIGHT;
	}

	if (GetAsyncKeyState(VK_SPACE) & 0x8000)
	{
		dwKey |= KEY_SPACE;
	}

	if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
	{
		dwKey |= KEY_LM;
	}

	if (GetAsyncKeyState('1') & 0x8000)
	{
		dwKey |= KEY_1;
	}

	if (GetAsyncKeyState('2') & 0x8000)
	{
		dwKey |= KEY_2;
	}
}

KeyMgr::KeyMgr()
{
}

KeyMgr::~KeyMgr()
{
}
