#pragma once
#include "Define.h"
#include "Function.h"

class TimeMgr
{
public:
	DECLARE_SINGLETON(TimeMgr);

private:
	TimeMgr();
public:
	~TimeMgr();

private:
	LARGE_INTEGER			cpuTick;
	LARGE_INTEGER			currentTime;
	LARGE_INTEGER			prevTime;

private:
	float deltaTime;
public:
	float GetDeltaTime(void) { return deltaTime; }
public:
	void Initialize(void);
	void Update(void);
};

