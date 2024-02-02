#include "Item.h"
#include "Include.h"

Item::Item()
{
}

Item::Item(const INFO& info)
	: DynamicObj(info)
{
}

Item::~Item()
{
	Release();
}

HRESULT Item::Initialize()
{
    scroll = *GET_SINGLE(ObjMgr)->GetScroll();
    // ���� ��ġ���� ����
    int randomNum = rand() % 900;
    randomPos = GetTilePos(randomNum);

    sortOrder = SORT_THIRD;
    objKey = (TCHAR*)L"Item";
    stateKey = (TCHAR*)L"Item";
    frame = FRAME(0, 0, 0, 0);
    info.size = D3DXVECTOR3(30.f, 30.f, 0);
    info.pos = randomPos;
    info.dir = D3DXVECTOR3(0, 0, 0);
    info.xMoveDir = D3DXVECTOR3(1.f, 0, 0);
    info.yMoveDir = D3DXVECTOR3(0, 1.f, 0);
    info.look = D3DXVECTOR3(1.f, 0, 0);
    info.angle = 0;

    D3DXMatrixIdentity(&matScale);
    D3DXMatrixIdentity(&matTrans);

	return S_OK;
}

SceneType Item::Update()
{
    scroll = *GET_SINGLE(ObjMgr)->GetScroll();

    D3DXMatrixTranslation(&matTrans, info.pos.x + scroll.x, info.pos.y + scroll.y, info.pos.z);

    info.world = matTrans;

	return SCENE_NONE;
}

void Item::Render()
{
    // ���� �÷��̾ ���� ���� �ٸ� �̹����� ����ϹǷ�,
    // stateKey <- ��Ƽ�ؽ�ó �뵵�̳�, �ش� ���� �ӽ÷� drawKey ó�� ���
    auto texInfo = GET_SINGLE(TextureMgr)->GetTexture(stateKey);
    if (texInfo == NULL)
        return;

    info.center = D3DXVECTOR3(info.size.x * .5f, info.size.y * .5f, 0.f);

    RECT rc = { info.size.x * (int)frame.current,
        info.size.y * (int)frame.animType,
        info.size.x * (int)frame.current + info.size.x,
        info.size.y * (int)frame.animType + info.size.y };

    GET_SINGLE(Device)->GetSprite()->SetTransform(&info.world);
    GET_SINGLE(Device)->GetSprite()->Draw(texInfo->texture, &rc, &info.center, NULL, D3DCOLOR_ARGB(255, 255, 255, 255));
}

void Item::Release()
{
}

Object* Item::Clone()
{
	return new Item(*this);
}
