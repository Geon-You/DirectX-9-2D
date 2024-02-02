#include "Ending.h"
#include "Include.h"

HRESULT Ending::Initialize()
{
	// ���� �ʱ�ȭ �� ��, Ÿ�ӸŴ��� �ʱ�ȭ
	GET_SINGLE(TimeMgr)->Initialize();

	// ������ü�� ������ ���� �ش� ���� ������Ÿ�� ��ü�� �����صд�.
	proto = new EndingObjProto;
	// �ش� ���� ���� ��ü ����
	if (FAILED(proto->Initialize()))
	{
		ERR_MSG(g_hWnd, L"���� ��ü ���� ����");
		return E_FAIL;
	}

	// �Ķ������ �ѱ� Ű ���� ������ü�� ã��, ��ü�� ���� �� ObjMgr�� ���
	GET_SINGLE(ObjMgr)->AddObject(proto, L"EndingBG")->SetRenderInfo(L"EndingBG");

	// �ش� ������ ����� ���� �ؽ�ó �ε�
	if (FAILED(GET_SINGLE(TextureMgr)->InsertTexture(L"Texture/Ending/EndingBG.png",
		L"EndingBG", TEX_SINGLE)))
	{
		ERR_MSG(g_hWnd, L"EndingBG �ؽ�ó �߰� ����");
		return E_FAIL;
	}

	return S_OK;
}

void Ending::Update()
{
	// ��ŸŸ�� ����
	GET_SINGLE(TimeMgr)->Update();
	// ������Ʈ �Ŵ����� ���� ���� ���� ��� ��ü�� �����Ѵ�.
	// �� ��, ��ü ������Ʈ �Լ��� ��ȯ ������ SceneType�� �޾�
	// ���Ѵٸ� Ư�� ��ü�� ���� ����, �ٸ� ������ ��ȯ�� �� �ְԲ� ��������
	SceneType t = GET_SINGLE(ObjMgr)->Update();

	if (GET_SINGLE(KeyMgr)->GetKey() & KEY_SPACE)
		t = SCENE_INGAME;
	else if (GET_SINGLE(KeyMgr)->GetKey() & KEY_LM)
		// ���α׷� ���� ����

	if (t != SCENE_NONE)
	{
		GET_SINGLE(SceneMgr)->SetScene(t);
		return;
	}
}

void Ending::Render()
{
	GET_SINGLE(ObjMgr)->Render();
}

void Ending::Release()
{
	GET_SINGLE(TextureMgr)->Destroy();
	GET_SINGLE(ObjMgr)->Destroy();
	GET_SINGLE(TimeMgr)->Destroy();
	SafeDelete(proto);
}

Ending::Ending()
{
}

Ending::~Ending()
{
    Release();
}
