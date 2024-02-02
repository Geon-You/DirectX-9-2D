#include "EndingObjProto.h"
#include "Include.h"

HRESULT EndingObjProto::Initialize()
{
	INFO info;
	ZeroMemory(&info, sizeof(info));

	D3DXMatrixIdentity(&info.world); // 단위행렬로 만들어주는 함수
	info.pos = D3DXVECTOR3(WIN_SIZE_X / 2.f, WIN_SIZE_Y / 2.f, 0.f);
	info.dir = D3DXVECTOR3(0.f, 0.f, 0.f);
	info.look = D3DXVECTOR3(1.f, 0.f, 0.f);

	protoObjs.insert(make_pair(L"EndingBG", new EndingBackGround(info)));

	return S_OK;
}

EndingObjProto::EndingObjProto()
{
}

EndingObjProto::~EndingObjProto()
{
}
