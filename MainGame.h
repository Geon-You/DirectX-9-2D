#pragma once
#include "Include.h"

class MainGame
{
public:
	HRESULT Initialize(); // 초기화
	void Update(); // 게임 내 각종 연산
	void Render(); // 게임 내 그리기
	void Release(); // 메모리 해제

public:
	MainGame();
	~MainGame();
};

