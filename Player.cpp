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
    // ���콺�� ���� ĳ���� ȸ��
    POINT mouse = Mouse::GetPos();
    D3DXVECTOR3 mousePos(mouse.x, mouse.y, 0);

    auto scroll = GET_SINGLE(ObjMgr)->GetScroll();
    GET_SINGLE(ObjMgr)->SetScroll(info.pos.x - WIN_SIZE_X * .5f, info.pos.y - WIN_SIZE_Y * .5f);

    // ������ ���� ����
    leftLimit = GetTilePos(0).x + scroll->x;
    rightLimit = GetTilePos(TILE_CNT_X - 1).x + scroll->x;
    upperLimit = GetTilePos(0).y + scroll->y;
    bottomLimit = GetTilePos(TILE_CNT_X * TILE_CNT_Y - 1).y + scroll->y;

    auto obstacleInfo = GET_SINGLE(ObjMgr)->GetInfo(L"Obstacle");

    // �� ƽ ��, �̵� ����
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

    // ��ֹ��� ���� �̵� ���� ���� �˻�
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

    // ĳ���Ͱ� �ٶ󺸴� ���� ����
    info.dir = (mousePos - (info.pos - (*scroll)));

    D3DXVec3Normalize(&info.dir, &info.dir);

    // �ٶ󺸴� ���� ���
    info.angle = -acosf(D3DXVec3Dot(&info.dir, &info.look));
    // 180�� �̻��� ȸ���� ó��
    if (mousePos.y > info.pos.y - scroll->y)
    {
        info.angle = 2 * D3DX_PI - info.angle;
    }

    // Ű �Է� �ߺ��� �����ϱ� ���� Ű �Է� ��Ÿ�� ����
    const DWORD delay = 500;
    bool wasKey1Pressed = false;
    bool wasKey2Pressed = false;

    bool isKey1Pressed = (GetAsyncKeyState('1') & 0x8000) || (GetAsyncKeyState('2') & 0x8000);
    bool isKey2Pressed = (GetAsyncKeyState('W') & 0x8000) || (GetAsyncKeyState('A') & 0x8000) ||
                            (GetAsyncKeyState('S') & 0x8000) || (GetAsyncKeyState('D') & 0x8000);

    // �̵� ��� ó��
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

    // ���� ��� ó��
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

    // ���� ��� ó��
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

    // ȸ�� ��� (�Ķ���ͷ� �ѱ� ������ ����)
    D3DXMatrixRotationZ(&matRotZ, info.angle);
    D3DXMatrixTranslation(&matTrans, info.pos.x - scroll->x, info.pos.y - scroll->y, info.pos.z);

    info.world = matRotZ * matTrans;

    // ��� ����
    UpdateMotion();
    // ������ ����
    UpdateFrame();
    // ���� �浹 �˻�
    HitEnemy();
    // �����۰� �浹 �˻�
    GetItem();

    // ü���� �� ������ ���� ����
    if (playerHP <= 0)
    {
        return SCENE_END;
    }

    return SCENE_NONE;
}

void Player::Render()
{
    // ���� �÷��̾ ���� ���� �ٸ� �̹����� ����ϹǷ�,
    // stateKey <- ��Ƽ�ؽ�ó �뵵�̳�, �ش� ���� �ӽ÷� drawKey ó�� ���
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
    // ���º� ��������Ʈ ������� ������ ����
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

    // �÷��̾�� ������ ���� �˻�
    const DWORD hitDelay = 400;
    // ObjMgr�� �ִ� ����Ʈ �ּ� ��������
    auto enemyList = GET_SINGLE(ObjMgr)->GetInfoList((TCHAR*)L"Enemy");
    // ����Ʈ�� ����ִٸ� ����
    if (enemyList == nullptr)
        return;

    // ������� �ʴٸ� ����Ʈ�� ��� ��ü��� �浹 �˻�
    for (auto i = enemyList->begin(); i != enemyList->end(); ++i)
    {
        if (info.pos.x - scroll->x < ((*i)->GetInfo()->pos.x + scroll->x) - (*i)->GetInfo()->size.x * .3f
            || info.pos.x - scroll->x > ((*i)->GetInfo()->pos.x + scroll->x) + (*i)->GetInfo()->size.x * .3f)
            continue;

        if (info.pos.y - scroll->y < ((*i)->GetInfo()->pos.y + scroll->y) - (*i)->GetInfo()->size.y * .3f
            || info.pos.y - scroll->y >((*i)->GetInfo()->pos.y + scroll->y) +(*i)->GetInfo()->size.y * .3f)
            continue;

        // �浹���� ��
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

    // ������� �ʴٸ� ����Ʈ�� ��� ��ü��� �浹 �˻�
    for (auto i = itemList->begin(); i != itemList->end(); ++i)
    {
        if (info.pos.x - scroll->x < ((*i)->GetInfo()->pos.x + scroll->x) - (*i)->GetInfo()->size.x * .5f
            || info.pos.x - scroll->x >((*i)->GetInfo()->pos.x + scroll->x) + (*i)->GetInfo()->size.x * .5f)
            continue;

        if (info.pos.y - scroll->y < ((*i)->GetInfo()->pos.y + scroll->y) - (*i)->GetInfo()->size.y * .5f
            || info.pos.y - scroll->y >((*i)->GetInfo()->pos.y + scroll->y) + (*i)->GetInfo()->size.y * .5f)
            continue;

        // �浹���� ��
        else
        {
            Player::playerHP += 20.f;
            (*i)->Dead();
            if (Player::playerHP >= 100.f)
                Player::playerHP = 100.f;
        }
    }
}
