#include "Ingame.h"
#include "Include.h"

HRESULT Ingame::Initialize()
{
	// 씬이 초기화 될 때, 타임매니저 초기화
	GET_SINGLE(TimeMgr)->Initialize();

	// 원형객체를 생성을 위한 해당 씬의 프로토타입 객체를 생성해둔다.
	proto = new IngameObjProto;
	// 해당 씬의 원형 객체 생성
	if (FAILED(proto->Initialize()))
	{
		ERR_MSG(g_hWnd, L"원형 객체 생성 실패");
		return E_FAIL;
	}

	// 파라미터의 넘긴 키 값의 원형객체를 찾아, 객체를 복제 후 ObjMgr에 등록
	GET_SINGLE(ObjMgr)->AddObject(proto, L"TileBG");
	GET_SINGLE(ObjMgr)->AddObject(proto, L"Player");
	GET_SINGLE(ObjMgr)->AddObject(proto, L"PlayerEffect");
	GET_SINGLE(ObjMgr)->AddObject(proto, L"Obstacle");
	GET_SINGLE(ObjMgr)->AddObject(proto, L"UI");

	// 해당 씬에서 사용할 각종 텍스처 로드
	if (FAILED(GET_SINGLE(TextureMgr)->InsertTexture(L"Texture/Ingame/Tile.png",
		L"TileBG", TEX_SINGLE)))
	{
		ERR_MSG(g_hWnd, L"TileBG 텍스처 추가 실패");
		return E_FAIL;
	}

	if (FAILED(GET_SINGLE(TextureMgr)->InsertTexture(L"Texture/Ingame/Player/Player_Run.png",
		L"Player_Idle", TEX_SINGLE)))
	{
		ERR_MSG(g_hWnd, L"Player_Idle 텍스처 추가 실패");
		return E_FAIL;
	}

	if (FAILED(GET_SINGLE(TextureMgr)->InsertTexture(L"Texture/Ingame/Player/Player_Run.png",
		L"Player_Run", TEX_SINGLE)))
	{
		ERR_MSG(g_hWnd, L"Player_Run 텍스처 추가 실패");
		return E_FAIL;
	}

	if (FAILED(GET_SINGLE(TextureMgr)->InsertTexture(L"Texture/Ingame/Player/Player_Handgun_Idle.png",
		L"Player_Handgun", TEX_SINGLE)))
	{
		ERR_MSG(g_hWnd, L"Player_Handgun 텍스처 추가 실패");
		return E_FAIL;
	}

	if (FAILED(GET_SINGLE(TextureMgr)->InsertTexture(L"Texture/Ingame/Player/Player_Shotgun_Idle.png",
		L"Player_Shotgun", TEX_SINGLE)))
	{
		ERR_MSG(g_hWnd, L"Player_Shotgun 텍스처 추가 실패");
		return E_FAIL;
	}

	if (FAILED(GET_SINGLE(TextureMgr)->InsertTexture(L"Texture/Ingame/Enemy/Enemy.png",
		L"Enemy", TEX_SINGLE)))
	{
		ERR_MSG(g_hWnd, L"Enemy 텍스처 추가 실패");
		return E_FAIL;
	}

	if (FAILED(GET_SINGLE(TextureMgr)->InsertTexture(L"Texture/Ingame/Effect/Bullet.png",
		L"Bullet", TEX_SINGLE)))
	{
		ERR_MSG(g_hWnd, L"Player Effect_Bullet 텍스처 추가 실패");
		return E_FAIL;
	}

	if (FAILED(GET_SINGLE(TextureMgr)->InsertTexture(L"Texture/Ingame/Effect/Muzzle.png",
		L"Effect", TEX_SINGLE)))
	{
		ERR_MSG(g_hWnd, L"Player Effect_Muzzle 텍스처 추가 실패");
		return E_FAIL;
	}

	if (FAILED(GET_SINGLE(TextureMgr)->InsertTexture(L"Texture/Ingame/Obstacles/Building_09.png",
		L"Obstacle", TEX_SINGLE)))
	{
		ERR_MSG(g_hWnd, L"Obstacle 텍스처 추가 실패");
		return E_FAIL;
	}

	if (FAILED(GET_SINGLE(TextureMgr)->InsertTexture(L"Texture/Ingame/Player/Health Bar.png",
		L"UI", TEX_SINGLE)))
	{
		ERR_MSG(g_hWnd, L"Player Health Bar 텍스처 추가 실패");
		return E_FAIL;
	}

	if (FAILED(GET_SINGLE(TextureMgr)->InsertTexture(L"Texture/Ingame/Item/Health Pack.png",
		L"Item", TEX_SINGLE)))
	{
		ERR_MSG(g_hWnd, L"Player Health Pack 텍스처 추가 실패");
		return E_FAIL;
	}

	spawnDelay = 1500;
	healthPackSpawnDelay = 30000;

    return S_OK;
}

void Ingame::Update()
{
	GET_SINGLE(TimeMgr)->Update();
	SceneType t = GET_SINGLE(ObjMgr)->Update();

	if (t != SCENE_NONE)
	{
		GET_SINGLE(SceneMgr)->SetScene(t);
		return;
	}

	// 플레이어 정보 받아오기
	auto player = GET_SINGLE(ObjMgr)->GetObjectInfo(L"Player")->GetObjectInfo();

	// 키 입력 중복을 방지하기 위한 키 입력 쿨타임 세팅
	// 무기에 따라 연사 속도 설정
	if (player->GetStateKey() == (TCHAR*)L"Player_Handgun")
		delay = 300;
	else if (player->GetStateKey() == (TCHAR*)L"Player_Shotgun")
		delay = 500;

	// 중복 입력 방지
	bool wasKeyPressed = false;
	bool isKeyPressed = GetAsyncKeyState(VK_LBUTTON) & 0x8000;

	if ((GET_SINGLE(KeyMgr)->GetKey() & KEY_LM) && (player->GetStateKey() == (TCHAR*)L"Player_Handgun" || player->GetStateKey() == (TCHAR*)L"Player_Shotgun")
		&& isKeyPressed && !wasKeyPressed)
	{
		DWORD currentTime = GetTickCount();

		if (currentTime - lastKeyPressedTime >= delay)
		{
			auto bullet = (Bullet*)GET_SINGLE(ObjMgr)->AddObject(proto, L"Bullet");

			bool isHandgun = player->GetStateKey() == (TCHAR*)L"Player_Handgun";
			bullet->SetDamage(isHandgun ? 30.f : 60.f);

			lastKeyPressedTime = currentTime;
		}
	}

	// 랜덤한 위치에서 enemy가 일정 시간마다 생성 (1.5초)
	DWORD currentSpawnTime = GetTickCount();

	if (currentSpawnTime - lastSpawnTime >= spawnDelay)
	{
		GET_SINGLE(ObjMgr)->AddObject(proto, L"Enemy");

		lastSpawnTime = currentSpawnTime;
	}

	// 랜덤한 위치에서 Item이 일정 시간마다 생성 (30초)
	DWORD HPPackCurrentSpawnTime = GetTickCount();

	if (HPPackCurrentSpawnTime - lastHealthPackSpawnTime >= healthPackSpawnDelay)
	{
		GET_SINGLE(ObjMgr)->AddObject(proto, L"Item");

		lastHealthPackSpawnTime = HPPackCurrentSpawnTime;
	}
}

void Ingame::Render()
{
	GET_SINGLE(ObjMgr)->Render();
}

void Ingame::Release()
{
	GET_SINGLE(TextureMgr)->Destroy();
	GET_SINGLE(ObjMgr)->Destroy();
	GET_SINGLE(TimeMgr)->Destroy();
	SafeDelete(proto);
}

Ingame::Ingame()
{
}

Ingame::~Ingame()
{
    Release();
}
