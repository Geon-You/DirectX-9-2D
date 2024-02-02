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
    // 플레이어를 따라다닐 이펙트이므로, 플레이어 정보를 가져옴
    auto playerInfo = GET_SINGLE(ObjMgr)->GetInfo(L"Player");

    auto scroll = GET_SINGLE(ObjMgr)->GetScroll();

    // 이미지를 플립하려면 스케일 행렬의 x축을 음수로
    D3DXMatrixTranslation(&matTrans, (playerInfo->pos.x - scroll->x) - 33.f, (playerInfo->pos.y - scroll->y) - 30.f, info.pos.z);
    D3DXMatrixScaling(&matScale, HpRatio, 1.f, 1.f); // 플레이어 체력 비율에 따라 체력바가 줄거나 늘어남

    info.world = matScale * matTrans;

    HpRatio = Player::playerHP * .01f;

    return SCENE_NONE;
}

void UI::Render()
{
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

void UI::Release()
{
}

Object* UI::Clone()
{
    return new UI(*this);
}
