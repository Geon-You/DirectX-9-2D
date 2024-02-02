#include "Object.h"
#include "Include.h"

Object::Object()
{
}

Object::Object(const INFO& info)
    : info(info)
{
}

Object::~Object()
{
}

void Object::UpdateFrame()
{
    frame.current += frame.time * GET_SINGLE(TimeMgr)->GetDeltaTime();
    
    if (frame.current > frame.last)
        frame.current = 0;
}

int Object::GetTileIndex(const D3DXVECTOR3& pos)
{
    auto tiles = GET_SINGLE(ObjMgr)->GetTiles();

    for (auto i = 0; i < tiles->size(); ++i)
    {
        if (CollPosToTile(pos, (*tiles)[i]))
        {
            return i;
        }
    }

    return -1;
}

const D3DXVECTOR3 Object::GetTilePos(int idx)
{
    auto tiles = GET_SINGLE(ObjMgr)->GetTiles();

    return D3DXVECTOR3((*tiles)[idx]->posX, (*tiles)[idx]->posY, 0);
}

bool Object::CollPosToTile(const D3DXVECTOR3& pos, const TILE* tileInfo)
{
    if (pos.x < tileInfo->posX - tileInfo->sizeX * .5f
        || pos.x > tileInfo->posX + tileInfo->sizeX * .5f)
        return false;

    if (pos.y < tileInfo->posY - tileInfo->sizeY * .5f
        || pos.y > tileInfo->posY + tileInfo->sizeY * .5f)
        return false;

    return true;
}