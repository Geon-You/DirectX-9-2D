#include "ObstaclesBackGround.h"
#include "Include.h"

ObstaclesBackGround::ObstaclesBackGround()
{
}

ObstaclesBackGround::ObstaclesBackGround(const INFO& info)
	: StaticObj(info)
{
}

ObstaclesBackGround::~ObstaclesBackGround()
{
	this->Release();
}

HRESULT ObstaclesBackGround::Initialize()
{
	info.pos = GetTilePos(140);
	objKey = (TCHAR*)L"Obstacle";
	stateKey = (TCHAR*)L"House_06";
	sortOrder = SORT_THIRD;
	info.size = D3DXVECTOR3(301.f, 306.f, 0);

	return S_OK;
}

SceneType ObstaclesBackGround::Update()
{
	auto scroll = GET_SINGLE(ObjMgr)->GetScroll();

	D3DXMATRIX matTrans;
    D3DXMatrixTranslation(&matTrans, info.pos.x + scroll->x, info.pos.y + scroll->y, info.pos.z);

	info.world = matTrans;

	return SCENE_NONE;
}

void ObstaclesBackGround::Render()
{
	// 키 값을 통해 텍스처를 불러옴
	auto texInfo = GET_SINGLE(TextureMgr)->GetTexture(objKey);

	// 텍스처가 없다면 리턴
	if (texInfo == nullptr)
		return;

	// 텍스처의 피벗
	info.center = D3DXVECTOR3(texInfo->ImgInfo.Width * .5f, texInfo->ImgInfo.Height * .5f, 0);

	// 스프라이트에 월드행렬 적용
	GET_SINGLE(Device)->GetSprite()->SetTransform(&info.world);

	GET_SINGLE(Device)->GetSprite()->Draw(texInfo->texture, // 텍스처
		NULL, // 텍스처에서 어느 부분을 나타낼 건지에 대한 rect
		&info.center, // 텍스처 피벗 (중심점)
		NULL, // 텍스처 추가 위치 (imagePos)
		D3DCOLOR_ARGB(255, 255, 255, 255)); // 변조 색상
}

void ObstaclesBackGround::Release()
{
}

Object* ObstaclesBackGround::Clone()
{
	return new ObstaclesBackGround(*this);
}
