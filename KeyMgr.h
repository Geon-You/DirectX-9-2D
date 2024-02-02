#pragma once
#include "Define.h"
#include "Function.h"

class KeyMgr
{
private:
	// 현재 누른 키 값들을 담을 변수
	DWORD dwKey;

public:
	DWORD GetKey() { return dwKey; }

public:
	void CheckKey();

public:
	DECLARE_SINGLETON(KeyMgr);

private:
	KeyMgr();

public:
	~KeyMgr();
};

