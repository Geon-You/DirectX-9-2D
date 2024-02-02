#include "Bullet.h"
#include "Include.h"

Bullet::Bullet()
{
}

Bullet::Bullet(const INFO& info)
	: DynamicObj(info)
{
}

Bullet::~Bullet()
{
	Release();
}

HRESULT Bullet::Initialize()
{
    // 플레이어를 따라다닐 이펙트이므로, 플레이어 정보를 가져옴
    auto playerInfo = GET_SINGLE(ObjMgr)->GetInfo(L"Player");
    auto scroll = GET_SINGLE(ObjMgr)->GetScroll();

    sortOrder = SORT_THIRD;
    objKey = (TCHAR*)L"Bullet";
    stateKey = NULL;
    frame = FRAME(0, 0, 0, 0);

    info.size = D3DXVECTOR3(40.f, 26.f, 0);
    info.angle = playerInfo->angle;
    info.pos = playerInfo->pos;
    info.dir = playerInfo->dir;
    info.xMoveDir = D3DXVECTOR3(1.f, 0, 0);
    info.yMoveDir = D3DXVECTOR3(0, 1.f, 0);
    info.look = D3DXVECTOR3(1.f, 0, 0);
    info.isHit = false;

    moveSpeed = 1.f;

	return S_OK;
}

SceneType Bullet::Update()
{
    D3DXMATRIX matTrans, matScale, matRotZ;

    auto scroll = GET_SINGLE(ObjMgr)->GetScroll();

    auto playerStateKey = GET_SINGLE(ObjMgr)->GetObjectStateKeyInfo(L"Player");

    // 회전 행렬 (파라미터로 넘긴 각도는 라디안)
    D3DXMatrixRotationZ(&matRotZ, info.angle);

    // 이미지를 플립하려면 스케일 행렬의 x축을 음수로
    D3DXMatrixTranslation(&matTrans, info.pos.x - scroll->x, info.pos.y - scroll->y, info.pos.z);
    D3DXMatrixScaling(&matScale, .7f, .7f, .7f);

    info.pos += info.dir * moveSpeed;

    auto calculatePos = info.pos - (*scroll);

    // 총알이 화면 밖으로 나가면 메모리 해제
    if (calculatePos.x  < 0 || calculatePos.x > WIN_SIZE_X
        || calculatePos.y < 0 || calculatePos.y > WIN_SIZE_Y)
    {
        Dead();
        return SCENE_NONE;
    }
    else
    {
        // 아니라면 총알이 적과 닿았는 지 충돌 검사 수행
        Hit();
    }

    info.world = matScale * matRotZ * matTrans;

	return SCENE_NONE;
}

void Bullet::Render()
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

void Bullet::Release()
{
}

Object* Bullet::Clone()
{
    return new Bullet(*this);
}

void Bullet::Hit()
{
    // ObjMgr에 있는 리스트 주소 가져오기
    auto enemyList = GET_SINGLE(ObjMgr)->GetInfoList((TCHAR*)L"Enemy");
    // 리스트가 비어있다면 리턴
    if (enemyList == nullptr)
        return;

    auto scroll = GET_SINGLE(ObjMgr)->GetScroll();
    auto calculatePos = info.pos - (*scroll); 

    // 리스트에 enemy객체가 존재한다면 순회하면서 충돌 검사
    for (auto i = enemyList->begin(); i != enemyList->end(); ++i)
    {
        auto enemyInfo = (*i)->GetInfo();

        if (calculatePos.x < (enemyInfo->pos.x + scroll->x) - enemyInfo->size.x * .5f
            || calculatePos.x > (enemyInfo->pos.x + scroll->x) + enemyInfo->size.x * .5f)
            continue;

        if (calculatePos.y < (enemyInfo->pos.y + scroll->y) - enemyInfo->size.y * .5f
            || calculatePos.y > (enemyInfo->pos.y + scroll->y) + enemyInfo->size.y * .5f)
            continue;

        // 데미지를 주고 총알 메모리 해제
        ((Enemy*)*i)->Hit(damage);
        Dead();
        return;
    }
}