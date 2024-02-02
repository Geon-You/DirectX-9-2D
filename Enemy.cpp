#include "Enemy.h"
#include "Include.h"

Enemy::Enemy()
{
}

Enemy::Enemy(const INFO& info)
    : DynamicObj(info)
{
}

Enemy::~Enemy()
{
    Release();
}

HRESULT Enemy::Initialize()
{
    scroll = *GET_SINGLE(ObjMgr)->GetScroll();

    sortOrder = SORT_THIRD;
    objKey = (TCHAR*)L"Enemy";
    stateKey = (TCHAR*)L"Enemy";
    frame = FRAME(0, 0, 5, 0.01f);

    // 맵의 가장자리 위치를 담을 벡터
    vector<int> tileLocation;

    // i는 y축, j는 x축, 각 변의 인덱스를 전부 벡터에 담음
    for (int i = 0; i < 30; ++i)
    {
        for (int j = 0; j < 30; ++j)
        {
            if (i == 0)
            {
                tileLocation.push_back(i + j);
            }
            else if (i > 0 && i < 29)
            {
                if (j == 0 || j == 29)
                    tileLocation.push_back(i * 30 + j);
            }
            else if (i == 29)
                tileLocation.push_back(i * 30 + j);
        }
    }

    // 저장된 가장자리 위치 중에서 랜덤하게 뽑아서 그 위치에서 enemy 스폰
    srand((unsigned int)time(nullptr));
    int randomIdx = tileLocation[rand() % 116];

    auto playerInfo = GET_SINGLE(ObjMgr)->GetInfo(L"Player");

    info.pos = GetTilePos(randomIdx);
    info.dir = D3DXVECTOR3(0, 0, 0);
    info.xMoveDir = D3DXVECTOR3(1.f, 0, 0);
    info.yMoveDir = D3DXVECTOR3(0, 1.f, 0);
    info.look = D3DXVECTOR3(1.f, 0, 0);
    info.size = D3DXVECTOR3(70.f, 70.f, 0);

    info.angle = 0;
    info.isHit = false;
    moveSpeed = .1f;
    health = 100.f;

    return S_OK;
}

SceneType Enemy::Update()
{
    // 프레임 갱신
    UpdateFrame();

    scroll = *GET_SINGLE(ObjMgr)->GetScroll();

    // 플레이어 정보 가져오기
    auto playerInfo = GET_SINGLE(ObjMgr)->GetInfo(L"Player");

    // 바라볼 방향 벡터
    info.dir = (playerInfo->pos - scroll) - (info.pos + scroll);
    D3DXVec3Normalize(&info.dir, &info.dir);

    // 바라볼 각도
    info.angle = acosf(D3DXVec3Dot(&info.dir, &info.look));
    // 180도 이상의 회전을 처리
    if (info.pos.y + scroll.y > playerInfo->pos.y - scroll.y)
    {
        info.angle = 2 * D3DX_PI - info.angle;
    }

    // 회전 행렬 (파라미터로 넘긴 각도는 라디안)
    D3DXMatrixRotationZ(&matRotZ, info.angle + D3DX_PI * .5f);

    auto obstacleInfo = GET_SINGLE(ObjMgr)->GetInfo(L"Obstacle");

    // 한 틱 당, 이동 변위
    D3DXVECTOR3 deltaDisp = D3DXVECTOR3(0, 0, 0);

    deltaDisp += info.dir * moveSpeed;

    // 장애물에 의한 이동 제한 범위 검사
    if (deltaDisp != D3DXVECTOR3(0, 0, 0))
    {
        auto nextPosX = info.pos.x + scroll.x + deltaDisp.x;
        auto nextPosY = info.pos.y + scroll.y + deltaDisp.y;

        auto left = obstacleInfo->pos.x - obstacleInfo->size.x * .5f + scroll.x;
        auto right = obstacleInfo->pos.x + obstacleInfo->size.x * .5f + scroll.x;
        auto top = obstacleInfo->pos.y - obstacleInfo->size.y * .5f + scroll.y;
        auto bot = obstacleInfo->pos.y + obstacleInfo->size.y * .5f + scroll.y;

        auto isCollX = (nextPosX + info.size.x * .4f >= left) &&
            (nextPosX - info.size.x * .4f <= right);

        auto isCollY = (nextPosY + info.size.y * .4f >= top) &&
            (nextPosY - info.size.y * .4f <= bot);

        auto isColl = isCollX && isCollY;

        if (!isColl)
            info.pos += deltaDisp;
    }

    D3DXMATRIX matTrans;
    D3DXMatrixTranslation(&matTrans, info.pos.x + scroll.x, info.pos.y + scroll.y, info.pos.z);

    info.world = matRotZ * matTrans;

    // 체력이 다 닳으면 enemy 사망
    if (health <= 0)
    {
        Dead();
        return SCENE_NONE;
    }

    return SCENE_NONE;
}

void Enemy::Render()
{
    // 현재 플레이어가 방향 별로 다른 이미지를 사용하므로,
    // stateKey <- 멀티텍스처 용도이나, 해당 값을 임시로 drawKey 처럼 사용
    auto texInfo = GET_SINGLE(TextureMgr)->GetTexture(stateKey);
    if (texInfo == NULL)
        return;

    info.center = D3DXVECTOR3(info.size.x * .5f, info.size.y * .5f, 0.f);

    RECT rc = { info.size.x * (int)frame.current,
        info.size.y * (int)frame.animType,
        info.size.x * (int)frame.current + info.size.x ,
        info.size.y * (int)frame.animType + info.size.y };

    GET_SINGLE(Device)->GetSprite()->SetTransform(&info.world);
    GET_SINGLE(Device)->GetSprite()->Draw(texInfo->texture, &rc, &info.center, NULL, D3DCOLOR_ARGB(255, 255, 255, 255));
}

void Enemy::Release()
{
}

Object* Enemy::Clone()
{
    return new Enemy(*this);
}

void Enemy::Hit(float damage)
{
    health -= damage;
}
