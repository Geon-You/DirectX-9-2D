#pragma once
#include "StaticObj.h"

class TileBackGround :
    public StaticObj
{
public:
    TileBackGround();
    TileBackGround(const INFO& info);
    virtual ~TileBackGround();

private:
    bool isDrawTileIndex;
    vector<TILE*> tiles;
    D3DXVECTOR3 scroll;

public:
    vector<TILE*>* GetTiles() { return &tiles; }
    D3DXVECTOR3* GetScroll() { return &scroll; }
    void SetScroll(float scrollX, float scrollY) { this->scroll.x = scrollX, this->scroll.y = scrollY; }

private:
    void LoadTileFromFile();
    void DrawTileIndex(int index);

public:
    HRESULT Initialize() override;
    SceneType Update() override;
    void Render() override;
    void Release() override;
    Object* Clone() override;
};

