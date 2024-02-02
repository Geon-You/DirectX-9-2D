#include "Title.h"
#include "Include.h"

HRESULT Title::Initialize()
{
	// 씬이 초기화 될 때, 타임매니저 초기화
	GET_SINGLE(TimeMgr)->Initialize();

	// 원형객체를 생성을 위한 해당 씬의 프로토타입 객체를 생성해둔다.
	proto = new TitleObjProto;
	// 해당 씬의 원형 객체 생성
	if (FAILED(proto->Initialize()))
	{
		ERR_MSG(g_hWnd, L"원형 객체 생성 실패");
		return E_FAIL;
	}

	// 파라미터의 넘긴 키 값의 원형객체를 찾아, 객체를 복제 후 ObjMgr에 등록
	GET_SINGLE(ObjMgr)->AddObject(proto, L"TitleBG")->SetRenderInfo(L"TitleBG");

	// 해당 씬에서 사용할 각종 텍스처 로드
	if (FAILED(GET_SINGLE(TextureMgr)->InsertTexture(L"Texture/Title/TitleBG.png",
		L"TitleBG", TEX_SINGLE)))
	{
		ERR_MSG(g_hWnd, L"TitleBG 텍스처 추가 실패");
		return E_FAIL;
	}

	return S_OK;
}

void Title::Update()
{
	// 델타타임 갱신
	GET_SINGLE(TimeMgr)->Update();
	// 오브젝트 매니저를 통해 현재 씬의 모든 객체를 갱신한다.
	// 이 때, 객체 업데이트 함수의 반환 값으로 SceneType을 받아
	// 원한다면 특정 객체를 갱신 도중, 다른 씬으로 전환할 수 있게끔 설계했음
	SceneType t = GET_SINGLE(ObjMgr)->Update();

	if (GET_SINGLE(KeyMgr)->GetKey() & KEY_SPACE)
		t = SCENE_INGAME;

	if (t != SCENE_NONE)
	{
		GET_SINGLE(SceneMgr)->SetScene(t);
		return;
	}
}

void Title::Render()
{
	GET_SINGLE(ObjMgr)->Render();
}

void Title::Release()
{
	GET_SINGLE(TextureMgr)->Destroy();
	GET_SINGLE(ObjMgr)->Destroy();
	GET_SINGLE(TimeMgr)->Destroy();
	SafeDelete(proto);
}

Title::Title()
{
}

Title::~Title()
{
	Release();
}
