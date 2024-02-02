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

    // 플레이어를 따라다닐 이펙트이므로, 플레이어 정보를 가져옴
    auto playerInfo = GET_SINGLE(ObjMgr)->GetInfo(L"Player");

    // 이미지를 플립하려면 스케일 행렬의 x축을 음수로
    D3DXMatrixTranslation(&matTrans, info.pos.x + info.size.x * .6f, info.pos.y , info.pos.z);
    D3DXMatrixScaling(&matScale, .8f, .8f, .8f);

    info.world = matScale * matTrans * playerInfo->world;

    // 플레이어 정보 받아오기
    auto player = GET_SINGLE(ObjMgr)->GetObjectInfo(L"Player")->GetObjectInfo();

    if (player->GetStateKey() == (TCHAR*)L"Player_Handgun")
        delay = 300;
    else if (player->GetStateKey() == (TCHAR*)L"Player_Shotgun")
        delay = 500;

    // 총알 발사 입력 쿨타임과 이펙트 표시 시간을 동일하게 맞추기 위해 중복 입력 방지
    bool wasKeyPressed = false;
    bool isKeyPressed = GetAsyncKeyState(VK_LBUTTON) & 0x8000;

    // 이펙트 표시 여부
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

    // 플레이어가 총을 들고 있지 않다면, 즉 idle이거나 run일땐 발사해도 이펙트 표시 안 됨
    if (!isFire || (playerStateKey == (TCHAR*)L"Player_Idle" || playerStateKey == (TCHAR*)L"Player_Run"))
        return;

    // 렌더 상태를 변경하여 다양한 효과를 줄 수 있음
    // 하지만 렌더 상태 변경 시, 기존에 이미 드로우된 스프라이트들도 같이 영향을 받음
    // Draw 가 호출 순간 그려지는게 아니라, 모아서 뒀다가 한번에 출력하기 때문에
    // 기존에 이미 그려진 객체들이 영향을 받지 않게 Flush()를 통해,
    // 기존에 Draw한 스프라이트들을 장치로 전부 제출함.
    GET_SINGLE(Device)->GetSprite()->Flush();

    // src : 그리고자 하는 이미지의 컬러
    // dest : 이미 그려진 이미지의 컬러
    GET_SINGLE(Device)->GetDevice()->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE); // (src * 1(알파))
    GET_SINGLE(Device)->GetDevice()->SetRenderState(D3DRS_DESTBLEND, NULL); // (dest * 1(알파))
    GET_SINGLE(Device)->GetDevice()->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD); // 어떤 연산을 할 것인지 (result = src * 1 + dest * 1)

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
