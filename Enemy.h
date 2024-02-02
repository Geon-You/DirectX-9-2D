#pragma once
#include "DynamicObj.h"

class Enemy :
    public DynamicObj
{
public:
    Enemy();
    Enemy(const INFO& info);
    virtual ~Enemy();

private:
    D3DXMATRIX matRotZ; // 회전 행렬 (z축 회전)
    D3DXMATRIX matTrans; // 이동 행렬
    // 스폰될 랜덤 위치
    int randomPosX;
    int randomPosY;
    float health; // 체력
    D3DXVECTOR3 scroll;

public:
    void Hit(float damge);

public:
    // DynamicObj을(를) 통해 상속됨
    HRESULT Initialize() override;
    SceneType Update() override;
    void Render() override;
    void Release() override;
    Object* Clone() override;
};

