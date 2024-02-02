#pragma once
#include "DynamicObj.h"
class UI :
    public DynamicObj
{
public:
    UI();
    UI(const INFO& info);
    virtual ~UI();

private:
    float HpRatio; // 플레이어 현재 체력 비율
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

