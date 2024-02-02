#pragma once
#include "DynamicObj.h"

class Item :
    public DynamicObj
{
public:
    Item();
    Item(const INFO& info);
    virtual ~Item();

private:
    D3DXVECTOR3 randomPos; // 스폰될 랜덤한 위치를 저장
    D3DXVECTOR3 scroll;
    D3DXMATRIX matScale;
    D3DXMATRIX matTrans;

public:
    // DynamicObj을(를) 통해 상속됨
    HRESULT Initialize() override;
    SceneType Update() override;
    void Render() override;
    void Release() override;
    Object* Clone() override;
};

