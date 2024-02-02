#include "MainGame.h"


HRESULT MainGame::Initialize()
{
	if (FAILED(GET_SINGLE(Device)->InitDevice(WINMODE_WIN)))
	{
		ERR_MSG(g_hWnd, L"장치 초기화 실패");
		return E_FAIL;
	}

	if (FAILED(GET_SINGLE(SceneMgr)->SetScene(SCENE_TITLE)))
	{
		ERR_MSG(g_hWnd, L"씬 변경 실패");
		return E_FAIL;
	}

	return S_OK;
}

void MainGame::Update()
{
	GET_SINGLE(KeyMgr)->CheckKey();
	GET_SINGLE(SceneMgr)->Update();
}

void MainGame::Render()
{
	GET_SINGLE(Device)->RenderBegin();
	GET_SINGLE(SceneMgr)->Render();
	GET_SINGLE(Device)->RenderEnd();
}

void MainGame::Release()
{
	GET_SINGLE(SceneMgr)->Destroy();
	GET_SINGLE(Device)->Destroy();
	GET_SINGLE(KeyMgr)->Destroy();
}

MainGame::MainGame()
{
}

MainGame::~MainGame()
{
	Release();
}
