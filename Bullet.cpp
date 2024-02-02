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
    // �÷��̾ ����ٴ� ����Ʈ�̹Ƿ�, �÷��̾� ������ ������
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

    // ȸ�� ��� (�Ķ���ͷ� �ѱ� ������ ����)
    D3DXMatrixRotationZ(&matRotZ, info.angle);

    // �̹����� �ø��Ϸ��� ������ ����� x���� ������
    D3DXMatrixTranslation(&matTrans, info.pos.x - scroll->x, info.pos.y - scroll->y, info.pos.z);
    D3DXMatrixScaling(&matScale, .7f, .7f, .7f);

    info.pos += info.dir * moveSpeed;

    auto calculatePos = info.pos - (*scroll);

    // �Ѿ��� ȭ�� ������ ������ �޸� ����
    if (calculatePos.x  < 0 || calculatePos.x > WIN_SIZE_X
        || calculatePos.y < 0 || calculatePos.y > WIN_SIZE_Y)
    {
        Dead();
        return SCENE_NONE;
    }
    else
    {
        // �ƴ϶�� �Ѿ��� ���� ��Ҵ� �� �浹 �˻� ����
        Hit();
    }

    info.world = matScale * matRotZ * matTrans;

	return SCENE_NONE;
}

void Bullet::Render()
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

void Bullet::Release()
{
}

Object* Bullet::Clone()
{
    return new Bullet(*this);
}

void Bullet::Hit()
{
    // ObjMgr�� �ִ� ����Ʈ �ּ� ��������
    auto enemyList = GET_SINGLE(ObjMgr)->GetInfoList((TCHAR*)L"Enemy");
    // ����Ʈ�� ����ִٸ� ����
    if (enemyList == nullptr)
        return;

    auto scroll = GET_SINGLE(ObjMgr)->GetScroll();
    auto calculatePos = info.pos - (*scroll); 

    // ����Ʈ�� enemy��ü�� �����Ѵٸ� ��ȸ�ϸ鼭 �浹 �˻�
    for (auto i = enemyList->begin(); i != enemyList->end(); ++i)
    {
        auto enemyInfo = (*i)->GetInfo();

        if (calculatePos.x < (enemyInfo->pos.x + scroll->x) - enemyInfo->size.x * .5f
            || calculatePos.x > (enemyInfo->pos.x + scroll->x) + enemyInfo->size.x * .5f)
            continue;

        if (calculatePos.y < (enemyInfo->pos.y + scroll->y) - enemyInfo->size.y * .5f
            || calculatePos.y > (enemyInfo->pos.y + scroll->y) + enemyInfo->size.y * .5f)
            continue;

        // �������� �ְ� �Ѿ� �޸� ����
        ((Enemy*)*i)->Hit(damage);
        Dead();
        return;
    }
}