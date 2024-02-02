#include "SceneMgr.h"
#include "Scene.h"
#include "Title.h"
#include "Ingame.h"
#include "Ending.h"

SceneMgr::SceneMgr()
	:currentScene(nullptr)
{
}

SceneMgr::~SceneMgr()
{
	Release();
}

HRESULT SceneMgr::SetScene(SceneType type)
{
	// 이전 씬이 존재한다면, 해제
	SafeDelete(currentScene);

	// 씬 타입에 따른 실제 씬 객체 할당
	switch (type)
	{
	case SCENE_TITLE:
		currentScene = new Title;
		break;
	case SCENE_INGAME:
		currentScene = new Ingame;
		break;
	case SCENE_END:
		currentScene = new Ending;
		break;
	}

	// 씬 초기화
	if (FAILED(currentScene->Initialize()))
	{
		ERR_MSG(g_hWnd, L"씬 초기화 실패");
		return E_FAIL;
	}

	return S_OK;
}

void SceneMgr::Update()
{
	currentScene->Update();
}

void SceneMgr::Render()
{
	currentScene->Render();
}

void SceneMgr::Release()
{
	SafeDelete(currentScene);
}

