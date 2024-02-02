#include "EndingBackGround.h"
#include "Include.h"

EndingBackGround::EndingBackGround()
{
}

EndingBackGround::EndingBackGround(const INFO& info)
	: StaticObj(info)
{
}

EndingBackGround::~EndingBackGround()
{
	this->Release();
}

HRESULT EndingBackGround::Initialize()
{
	info.pos = D3DXVECTOR3(WIN_SIZE_X * .5f, WIN_SIZE_Y * .5f, 0);

	return S_OK;
}

SceneType EndingBackGround::Update()
{
	D3DXMatrixTranslation(&info.world, info.pos.x, info.pos.y, info.pos.z);

	return SCENE_NONE;
}

void EndingBackGround::Render()
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

void EndingBackGround::Release()
{

}

Object* EndingBackGround::Clone()
{
	return new EndingBackGround(*this);
}
