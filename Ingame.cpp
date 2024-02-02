#include "Ingame.h"
#include "Include.h"

HRESULT Ingame::Initialize()
{
	// ���� �ʱ�ȭ �� ��, Ÿ�ӸŴ��� �ʱ�ȭ
	GET_SINGLE(TimeMgr)->Initialize();

	// ������ü�� ������ ���� �ش� ���� ������Ÿ�� ��ü�� �����صд�.
	proto = new IngameObjProto;
	// �ش� ���� ���� ��ü ����
	if (FAILED(proto->Initialize()))
	{
		ERR_MSG(g_hWnd, L"���� ��ü ���� ����");
		return E_FAIL;
	}

	// �Ķ������ �ѱ� Ű ���� ������ü�� ã��, ��ü�� ���� �� ObjMgr�� ���
	GET_SINGLE(ObjMgr)->AddObject(proto, L"TileBG");
	GET_SINGLE(ObjMgr)->AddObject(proto, L"Player");
	GET_SINGLE(ObjMgr)->AddObject(proto, L"PlayerEffect");
	GET_SINGLE(ObjMgr)->AddObject(proto, L"Obstacle");
	GET_SINGLE(ObjMgr)->AddObject(proto, L"UI");

	// �ش� ������ ����� ���� �ؽ�ó �ε�
	if (FAILED(GET_SINGLE(TextureMgr)->InsertTexture(L"Texture/Ingame/Tile.png",
		L"TileBG", TEX_SINGLE)))
	{
		ERR_MSG(g_hWnd, L"TileBG �ؽ�ó �߰� ����");
		return E_FAIL;
	}

	if (FAILED(GET_SINGLE(TextureMgr)->InsertTexture(L"Texture/Ingame/Player/Player_Run.png",
		L"Player_Idle", TEX_SINGLE)))
	{
		ERR_MSG(g_hWnd, L"Player_Idle �ؽ�ó �߰� ����");
		return E_FAIL;
	}

	if (FAILED(GET_SINGLE(TextureMgr)->InsertTexture(L"Texture/Ingame/Player/Player_Run.png",
		L"Player_Run", TEX_SINGLE)))
	{
		ERR_MSG(g_hWnd, L"Player_Run �ؽ�ó �߰� ����");
		return E_FAIL;
	}

	if (FAILED(GET_SINGLE(TextureMgr)->InsertTexture(L"Texture/Ingame/Player/Player_Handgun_Idle.png",
		L"Player_Handgun", TEX_SINGLE)))
	{
		ERR_MSG(g_hWnd, L"Player_Handgun �ؽ�ó �߰� ����");
		return E_FAIL;
	}

	if (FAILED(GET_SINGLE(TextureMgr)->InsertTexture(L"Texture/Ingame/Player/Player_Shotgun_Idle.png",
		L"Player_Shotgun", TEX_SINGLE)))
	{
		ERR_MSG(g_hWnd, L"Player_Shotgun �ؽ�ó �߰� ����");
		return E_FAIL;
	}

	if (FAILED(GET_SINGLE(TextureMgr)->InsertTexture(L"Texture/Ingame/Enemy/Enemy.png",
		L"Enemy", TEX_SINGLE)))
	{
		ERR_MSG(g_hWnd, L"Enemy �ؽ�ó �߰� ����");
		return E_FAIL;
	}

	if (FAILED(GET_SINGLE(TextureMgr)->InsertTexture(L"Texture/Ingame/Effect/Bullet.png",
		L"Bullet", TEX_SINGLE)))
	{
		ERR_MSG(g_hWnd, L"Player Effect_Bullet �ؽ�ó �߰� ����");
		return E_FAIL;
	}

	if (FAILED(GET_SINGLE(TextureMgr)->InsertTexture(L"Texture/Ingame/Effect/Muzzle.png",
		L"Effect", TEX_SINGLE)))
	{
		ERR_MSG(g_hWnd, L"Player Effect_Muzzle �ؽ�ó �߰� ����");
		return E_FAIL;
	}

	if (FAILED(GET_SINGLE(TextureMgr)->InsertTexture(L"Texture/Ingame/Obstacles/Building_09.png",
		L"Obstacle", TEX_SINGLE)))
	{
		ERR_MSG(g_hWnd, L"Obstacle �ؽ�ó �߰� ����");
		return E_FAIL;
	}

	if (FAILED(GET_SINGLE(TextureMgr)->InsertTexture(L"Texture/Ingame/Player/Health Bar.png",
		L"UI", TEX_SINGLE)))
	{
		ERR_MSG(g_hWnd, L"Player Health Bar �ؽ�ó �߰� ����");
		return E_FAIL;
	}

	if (FAILED(GET_SINGLE(TextureMgr)->InsertTexture(L"Texture/Ingame/Item/Health Pack.png",
		L"Item", TEX_SINGLE)))
	{
		ERR_MSG(g_hWnd, L"Player Health Pack �ؽ�ó �߰� ����");
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

	// �÷��̾� ���� �޾ƿ���
	auto player = GET_SINGLE(ObjMgr)->GetObjectInfo(L"Player")->GetObjectInfo();

	// Ű �Է� �ߺ��� �����ϱ� ���� Ű �Է� ��Ÿ�� ����
	// ���⿡ ���� ���� �ӵ� ����
	if (player->GetStateKey() == (TCHAR*)L"Player_Handgun")
		delay = 300;
	else if (player->GetStateKey() == (TCHAR*)L"Player_Shotgun")
		delay = 500;

	// �ߺ� �Է� ����
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

	// ������ ��ġ���� enemy�� ���� �ð����� ���� (1.5��)
	DWORD currentSpawnTime = GetTickCount();

	if (currentSpawnTime - lastSpawnTime >= spawnDelay)
	{
		GET_SINGLE(ObjMgr)->AddObject(proto, L"Enemy");

		lastSpawnTime = currentSpawnTime;
	}

	// ������ ��ġ���� Item�� ���� �ð����� ���� (30��)
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
