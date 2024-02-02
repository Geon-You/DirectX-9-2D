#include "PlayerEffect.h"
#include "Include.h"

PlayerEffect::PlayerEffect()
{
}

PlayerEffect::PlayerEffect(const INFO& info)
    : DynamicObj(info)
{
}

PlayerEffect::~PlayerEffect()
{
    Release();
}

HRESULT PlayerEffect::Initialize()
{
    sortOrder = SORT_THIRD;
    objKey = (TCHAR*)L"Effect";
    stateKey = NULL;
    frame = FRAME(0, 0, 0, 0);
    info.size = D3DXVECTOR3(50.f, 26.f, 0);
    info.pos = D3DXVECTOR3(0, 0, 0);
    info.dir = D3DXVECTOR3(0, 0, 0);
    info.xMoveDir = D3DXVECTOR3(1.f, 0, 0);
    info.yMoveDir = D3DXVECTOR3(0, 1.f, 0);
    info.look = D3DXVECTOR3(1.f, 0, 0);

    info.angle = 0;

    isFire = false;

    return S_OK;
}

SceneType PlayerEffect::Update()
{
    D3DXMATRIX matTrans, matScale;

    // �÷��̾ ����ٴ� ����Ʈ�̹Ƿ�, �÷��̾� ������ ������
    auto playerInfo = GET_SINGLE(ObjMgr)->GetInfo(L"Player");

    // �̹����� �ø��Ϸ��� ������ ����� x���� ������
    D3DXMatrixTranslation(&matTrans, info.pos.x + info.size.x * .6f, info.pos.y , info.pos.z);
    D3DXMatrixScaling(&matScale, .8f, .8f, .8f);

    info.world = matScale * matTrans * playerInfo->world;

    // �÷��̾� ���� �޾ƿ���
    auto player = GET_SINGLE(ObjMgr)->GetObjectInfo(L"Player")->GetObjectInfo();

    if (player->GetStateKey() == (TCHAR*)L"Player_Handgun")
        delay = 300;
    else if (player->GetStateKey() == (TCHAR*)L"Player_Shotgun")
        delay = 500;

    // �Ѿ� �߻� �Է� ��Ÿ�Ӱ� ����Ʈ ǥ�� �ð��� �����ϰ� ���߱� ���� �ߺ� �Է� ����
    bool wasKeyPressed = false;
    bool isKeyPressed = GetAsyncKeyState(VK_LBUTTON) & 0x8000;

    // ����Ʈ ǥ�� ����
    if ((GET_SINGLE(KeyMgr)->GetKey() & KEY_LM) && 
        (player->GetStateKey() == (TCHAR*)L"Player_Handgun" || player->GetStateKey() == (TCHAR*)L"Player_Shotgun")
        && isKeyPressed && !wasKeyPressed)
    {
        DWORD currentTime = GetTickCount();

        if (currentTime - lastKeyPressedTime >= delay)
        {
            isFire = true;

            lastKeyPressedTime = currentTime;
        }
    }
    else
    {
        isFire = false;
    }

    return SCENE_NONE;
}

void PlayerEffect::Render()
{
    auto playerStateKey = GET_SINGLE(ObjMgr)->GetObjectStateKeyInfo(L"Player");

    // �÷��̾ ���� ��� ���� �ʴٸ�, �� idle�̰ų� run�϶� �߻��ص� ����Ʈ ǥ�� �� ��
    if (!isFire || (playerStateKey == (TCHAR*)L"Player_Idle" || playerStateKey == (TCHAR*)L"Player_Run"))
        return;

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

void PlayerEffect::Release()
{
}

Object* PlayerEffect::Clone()
{
    return new PlayerEffect(*this);
}
