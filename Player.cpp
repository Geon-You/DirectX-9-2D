#include "Player.h"
#include "Include.h"

float Player::playerHP = 100.f;

Player::Player()
{
}

Player::Player(const INFO& info)
    :DynamicObj(info)
{
}

Player::~Player()
{
    Release();
}

HRESULT Player::Initialize()
{
    isHandgun = false;
    isShotgun = false;
    lastKeyPressedTime = 0;
    lastHitTime = 0;
    sortOrder = SORT_THIRD;
    objKey = (TCHAR*)L"Player";
    stateKey = (TCHAR*)L"Player_Idle";
    frame = FRAME(0, 0, 0, 0);

    info.pos = GetTilePos(125);
    GET_SINGLE(ObjMgr)->SetScroll(info.pos.x + WIN_SIZE_X * .5f, info.pos.y + WIN_SIZE_Y * .5f);

    info.dir = D3DXVECTOR3(0, 0, 0);
    info.xMoveDir = D3DXVECTOR3(1.f, 0, 0);
    info.yMoveDir = D3DXVECTOR3(0, 1.f, 0);
    info.look = D3DXVECTOR3(1.f, 0, 0);
    info.size = D3DXVECTOR3(66.f, 60.f, 0);

    info.angle = 0;
    info.isHit = false;
    moveSpeed = .2f;
    D3DXMatrixIdentity(&matTrans);
    D3DXMatrixIdentity(&matRotZ);

    upperLimit = GetTilePos(0).y;
    leftLimit = GetTilePos(0).x;
    bottomLimit = GetTilePos(869).y;
    rightLimit = GetTilePos(899).x;

    return S_OK;
}

SceneType Player::Update()
{
    // 마우스에 따라 캐릭터 회전
    POINT mouse = Mouse::GetPos();
    D3DXVECTOR3 mousePos(mouse.x, mouse.y, 0);

    auto scroll = GET_SINGLE(ObjMgr)->GetScroll();
    GET_SINGLE(ObjMgr)->SetScroll(info.pos.x - WIN_SIZE_X * .5f, info.pos.y - WIN_SIZE_Y * .5f);

    // 움직임 범위 제한
    leftLimit = GetTilePos(0).x + scroll->x;
    rightLimit = GetTilePos(TILE_CNT_X - 1).x + scroll->x;
    upperLimit = GetTilePos(0).y + scroll->y;
    bottomLimit = GetTilePos(TILE_CNT_X * TILE_CNT_Y - 1).y + scroll->y;

    auto obstacleInfo = GET_SINGLE(ObjMgr)->GetInfo(L"Obstacle");

    // 한 틱 당, 이동 변위
    D3DXVECTOR3 deltaDisp = D3DXVECTOR3(0,0,0);

    if ((GET_SINGLE(KeyMgr)->GetKey() & KEY_UP) && info.pos.y - scroll->y > upperLimit 
        )
    {
        deltaDisp -= info.yMoveDir * moveSpeed;
    }
    
    if  ((GET_SINGLE(KeyMgr)->GetKey() & KEY_DOWN) && info.pos.y - scroll->y < bottomLimit 
        )
    {
        deltaDisp += info.yMoveDir * moveSpeed;
    }

    if ((GET_SINGLE(KeyMgr)->GetKey() & KEY_LEFT) && info.pos.x - scroll->x > leftLimit 
        )
    {
        deltaDisp -= info.xMoveDir * moveSpeed;
    }
    
    if ((GET_SINGLE(KeyMgr)->GetKey() & KEY_RIGHT) && info.pos.x - scroll->x < rightLimit 
        )
    {
        deltaDisp += info.xMoveDir * moveSpeed;
    }

    // 장애물에 의한 이동 제한 범위 검사
    if (deltaDisp != D3DXVECTOR3(0, 0, 0))
    {
        auto nextPosX = info.pos.x - scroll->x + deltaDisp.x;
        auto nextPosY = info.pos.y - scroll->y + deltaDisp.y;

        auto left   = obstacleInfo->pos.x - obstacleInfo->size.x * .5f + scroll->x;
        auto right = obstacleInfo->pos.x + obstacleInfo->size.x * .5f + scroll->x;
        auto top = obstacleInfo->pos.y - obstacleInfo->size.y * .5f + scroll->y;
        auto bot = obstacleInfo->pos.y + obstacleInfo->size.y * .5f + scroll->y;

        auto isCollX = (nextPosX + info.size.x * .3f >= left) &&
            (nextPosX - info.size.x * .3f <= right);

        auto isCollY = (nextPosY + info.size.y * .2f >= top) &&
            (nextPosY - info.size.y * .2f <= bot);

        auto isColl = isCollX && isCollY;
            
        if (!isColl)
            info.pos -= deltaDisp;
    }

    // 캐릭터가 바라보는 방향 벡터
    info.dir = (mousePos - (info.pos - (*scroll)));

    D3DXVec3Normalize(&info.dir, &info.dir);

    // 바라보는 각도 계산
    info.angle = -acosf(D3DXVec3Dot(&info.dir, &info.look));
    // 180도 이상의 회전을 처리
    if (mousePos.y > info.pos.y - scroll->y)
    {
        info.angle = 2 * D3DX_PI - info.angle;
    }

    // 키 입력 중복을 방지하기 위한 키 입력 쿨타임 세팅
    const DWORD delay = 500;
    bool wasKey1Pressed = false;
    bool wasKey2Pressed = false;

    bool isKey1Pressed = (GetAsyncKeyState('1') & 0x8000) || (GetAsyncKeyState('2') & 0x8000);
    bool isKey2Pressed = (GetAsyncKeyState('W') & 0x8000) || (GetAsyncKeyState('A') & 0x8000) ||
                            (GetAsyncKeyState('S') & 0x8000) || (GetAsyncKeyState('D') & 0x8000);

    // 이동 모션 처리
    if (isKey2Pressed && !wasKey2Pressed && !isHandgun && !isShotgun)
    {
        stateKey = (TCHAR*)L"Player_Run";
        wasKey2Pressed = true;
    }

    if (!isKey2Pressed && !isHandgun && !isShotgun)
    {
        stateKey = (TCHAR*)L"Player_Idle";
        wasKey2Pressed = false;
    }

    // 권총 모션 처리
    if ((GET_SINGLE(KeyMgr)->GetKey() & KEY_1) && isKey1Pressed && !wasKey1Pressed)
    {
        DWORD currentTime = GetTickCount();

        if (currentTime - lastKeyPressedTime >= delay)
        {
            if (isHandgun == false)
            {
                stateKey = (TCHAR*)L"Player_Handgun";
                isHandgun = true;
            }
            else
            {
                stateKey = (TCHAR*)L"Player_Idle";
                isHandgun = false;
            }

            if (isShotgun == true)
                isShotgun = false;

            lastKeyPressedTime = currentTime;
        }
    }

    // 샷건 모션 처리
    if ((GET_SINGLE(KeyMgr)->GetKey() & KEY_2) && isKey1Pressed && !wasKey1Pressed)
    {
        DWORD currentTime = GetTickCount();

        if (currentTime - lastKeyPressedTime >= delay)
        {
            if (isShotgun == false)
            {
                stateKey = (TCHAR*)L"Player_Shotgun";
                isShotgun = true;
            }
            else
            {
                stateKey = (TCHAR*)L"Player_Idle";
                isShotgun = false;
            }

            if (isHandgun == true)
                isHandgun = false;

            lastKeyPressedTime = currentTime;
        }
    }

    wasKey1Pressed = isKey1Pressed;
    wasKey2Pressed = isKey2Pressed;

    // 회전 행렬 (파라미터로 넘긴 각도는 라디안)
    D3DXMatrixRotationZ(&matRotZ, info.angle);
    D3DXMatrixTranslation(&matTrans, info.pos.x - scroll->x, info.pos.y - scroll->y, info.pos.z);

    info.world = matRotZ * matTrans;

    // 모션 갱신
    UpdateMotion();
    // 프레임 갱신
    UpdateFrame();
    // 적과 충돌 검사
    HitEnemy();
    // 아이템과 충돌 검사
    GetItem();

    // 체력이 다 닳으면 게임 오버
    if (playerHP <= 0)
    {
        return SCENE_END;
    }

    return SCENE_NONE;
}

void Player::Render()
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

void Player::Release()
{
}

Object* Player::Clone()
{
    return new Player(*this);
}

void Player::UpdateMotion()
{
    // 상태별 스프라이트 사이즈와 프레임 지정
    if (stateKey == (TCHAR*)L"Player_Run")
    {
        frame.animType = 0;
        frame.last = 5;
        frame.time = 0.007f;
        info.size = D3DXVECTOR3(80.f, 87.f, 0);
    }

    if (stateKey == (TCHAR*)L"Player_Idle")
    {
        frame.animType = 0;
        frame.current = 0;
        frame.last = 0;
        frame.time = 0;
        info.size = D3DXVECTOR3(80.f, 87.f, 0);
    }

    if (stateKey == (TCHAR*)L"Player_Handgun")
    {
        frame.animType = 0;
        frame.current = 0;
        frame.last = 0;
        frame.time = 0;
        info.size = D3DXVECTOR3(66.f, 60.f, 0);
    }

    if (stateKey == (TCHAR*)L"Player_Shotgun")
    {
        frame.animType = 0;
        frame.current = 0;
        frame.last = 0;
        frame.time = 0;
        info.size = D3DXVECTOR3(63.f, 57.f, 0);
    }
}

void Player::HitEnemy()
{
    auto scroll = GET_SINGLE(ObjMgr)->GetScroll();

    // 플레이어와 적과의 층돌 검사
    const DWORD hitDelay = 400;
    // ObjMgr에 있는 리스트 주소 가져오기
    auto enemyList = GET_SINGLE(ObjMgr)->GetInfoList((TCHAR*)L"Enemy");
    // 리스트가 비어있다면 리턴
    if (enemyList == nullptr)
        return;

    // 비어있지 않다면 리스트내 모든 객체들과 충돌 검사
    for (auto i = enemyList->begin(); i != enemyList->end(); ++i)
    {
        if (info.pos.x - scroll->x < ((*i)->GetInfo()->pos.x + scroll->x) - (*i)->GetInfo()->size.x * .3f
            || info.pos.x - scroll->x > ((*i)->GetInfo()->pos.x + scroll->x) + (*i)->GetInfo()->size.x * .3f)
            continue;

        if (info.pos.y - scroll->y < ((*i)->GetInfo()->pos.y + scroll->y) - (*i)->GetInfo()->size.y * .3f
            || info.pos.y - scroll->y >((*i)->GetInfo()->pos.y + scroll->y) +(*i)->GetInfo()->size.y * .3f)
            continue;

        // 충돌했을 때
        else
        {
            DWORD currentHitTime = GetTickCount();

            if (currentHitTime - lastHitTime >= hitDelay)
            {
                Player::playerHP -= 10.f;

                lastHitTime = currentHitTime;
            }
        }
    }
}

void Player::GetItem()
{
    auto scroll = GET_SINGLE(ObjMgr)->GetScroll();

    auto itemList = GET_SINGLE(ObjMgr)->GetInfoList((TCHAR*)L"Item");
    if (itemList == nullptr)
        return;

    // 비어있지 않다면 리스트내 모든 객체들과 충돌 검사
    for (auto i = itemList->begin(); i != itemList->end(); ++i)
    {
        if (info.pos.x - scroll->x < ((*i)->GetInfo()->pos.x + scroll->x) - (*i)->GetInfo()->size.x * .5f
            || info.pos.x - scroll->x >((*i)->GetInfo()->pos.x + scroll->x) + (*i)->GetInfo()->size.x * .5f)
            continue;

        if (info.pos.y - scroll->y < ((*i)->GetInfo()->pos.y + scroll->y) - (*i)->GetInfo()->size.y * .5f
            || info.pos.y - scroll->y >((*i)->GetInfo()->pos.y + scroll->y) + (*i)->GetInfo()->size.y * .5f)
            continue;

        // 충돌했을 때
        else
        {
            Player::playerHP += 20.f;
            (*i)->Dead();
            if (Player::playerHP >= 100.f)
                Player::playerHP = 100.f;
        }
    }
}
