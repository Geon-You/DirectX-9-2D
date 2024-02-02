#include "UI.h"
#include "Include.h"

UI::UI()
{
}

UI::UI(const INFO& info)
    : DynamicObj(info)
{
}

UI::~UI()
{
    Release();
}

HRESULT UI::Initialize()
{
    sortOrder = SORT_THIRD;
    objKey = (TCHAR*)L"UI";
    stateKey = (TCHAR*)L"Health Bar";
    frame = FRAME(0, 0, 0, 0);
    info.size = D3DXVECTOR3(66.f, 10.f, 0);
    info.pos = D3DXVECTOR3(0, 0, 0);
    info.dir = D3DXVECTOR3(0, 0, 0);
    info.xMoveDir = D3DXVECTOR3(1.f, 0, 0);
    info.yMoveDir = D3DXVECTOR3(0, 1.f, 0);
    info.look = D3DXVECTOR3(1.f, 0, 0);
    info.angle = 0;

    HpRatio = Player::playerHP * .01f;

    D3DXMatrixIdentity(&matScale);
    D3DXMatrixIdentity(&matTrans);

    return S_OK;
}

SceneType UI::Update()
{
    // �÷��̾ ����ٴ� ����Ʈ�̹Ƿ�, �÷��̾� ������ ������
    auto playerInfo = GET_SINGLE(ObjMgr)->GetInfo(L"Player");

    auto scroll = GET_SINGLE(ObjMgr)->GetScroll();

    // �̹����� �ø��Ϸ��� ������ ����� x���� ������
    D3DXMatrixTranslation(&matTrans, (playerInfo->pos.x - scroll->x) - 33.f, (playerInfo->pos.y - scroll->y) - 30.f, info.pos.z);
    D3DXMatrixScaling(&matScale, HpRatio, 1.f, 1.f); // �÷��̾� ü�� ������ ���� ü�¹ٰ� �ٰų� �þ

    info.world = matScale * matTrans;

    HpRatio = Player::playerHP * .01f;

    return SCENE_NONE;
}

void UI::Render()
{
    // ���� ���¸� �����Ͽ� �پ��� ȿ���� �� �� ����
    // ������ ���� ���� ���� ��, ������ �̹� ��ο�� ��������Ʈ�鵵 ���� ������ ����
    // Draw �� ȣ�� ���� �׷����°� �ƴ϶�, ��Ƽ� �״ٰ� �ѹ��� ����ϱ� ������
    // ������ �̹� �׷��� ��ü���� ������ ���� �ʰ� Flush()�� ����,
    // ������ Draw�� ��������Ʈ���� ��ġ�� ���� ������.
    GET_SINGLE(Device)->GetSprite()->Flush();

    // src : �׸����� �ϴ� �̹����� �÷�
    // dest : �̹� �׷��� �̹����� �÷�
    GET_SINGLE(Device)->GetDevice()->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE); // (src * 1(����))
    GET_SINGLE(Device)->GetDevice()->SetRenderState(D3DRS_DESTBLEND, NULL); // (dest * 1(����))
    GET_SINGLE(Device)->GetDevice()->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD); // � ������ �� ������ (result = src * 1 + dest * 1)

    const TEXINFO* texInfo = GET_SINGLE(TextureMgr)->GetTexture(objKey, stateKey, (int)frame.current);
    if (texInfo == nullptr)
        return;

    info.center = D3DXVECTOR3(0, texInfo->ImgInfo.Height * .5f, 0.f);

    GET_SINGLE(Device)->GetSprite()->SetTransform(&info.world);
    GET_SINGLE(Device)->GetSprite()->Draw(texInfo->texture, NULL, &info.center, NULL, D3DCOLOR_ARGB(255, 255, 255, 255));

}

void UI::Release()
{
}

Object* UI::Clone()
{
    return new UI(*this);
}
