#pragma once
#include "Define.h"
#include "Function.h"

class Scene;
class SceneMgr
{
private:
	// 현재 씬 객체를 담을 필드
	Scene* currentScene; 

public:
	DECLARE_SINGLETON(SceneMgr);
	Scene* GetScene() { return currentScene; }

public:
	// 파라미터로 받은 씬 타입으로 현재 씬을 변경하는 기능
	HRESULT SetScene(SceneType type);
	void Update(); // 현재 씬 업데이트
	void Render(); // 현재 씬 그리기
	void Release();

private:
	SceneMgr();

public:
	~SceneMgr();
};

