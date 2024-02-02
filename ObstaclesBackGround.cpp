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
	// Ű ���� ���� �ؽ�ó�� �ҷ���
	auto texInfo = GET_SINGLE(TextureMgr)->GetTexture(objKey);

	// �ؽ�ó�� ���ٸ� ����
	if (texInfo == nullptr)
		return;

	// �ؽ�ó�� �ǹ�
	info.center = D3DXVECTOR3(texInfo->ImgInfo.Width * .5f, texInfo->ImgInfo.Height * .5f, 0);

	// ��������Ʈ�� ������� ����
	GET_SINGLE(Device)->GetSprite()->SetTransform(&info.world);

	GET_SINGLE(Device)->GetSprite()->Draw(texInfo->texture, // �ؽ�ó
		NULL, // �ؽ�ó���� ��� �κ��� ��Ÿ�� ������ ���� rect
		&info.center, // �ؽ�ó �ǹ� (�߽���)
		NULL, // �ؽ�ó �߰� ��ġ (imagePos)
		D3DCOLOR_ARGB(255, 255, 255, 255)); // ���� ����
}

void ObstaclesBackGround::Release()
{
}

Object* ObstaclesBackGround::Clone()
{
	return new ObstaclesBackGround(*this);
}
