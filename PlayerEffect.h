#pragma once
#include "DynamicObj.h"

class PlayerEffect :
    public DynamicObj
{
public:
    PlayerEffect();
    PlayerEffect(const INFO& info);
    virtual ~PlayerEffect();

private:
    bool isFire;
    float delay;
    float lastKeyPressedTime = 0;

    D3DXMATRIX matRotZ; // ȸ�� ��� (z�� ȸ��)

public:
    // DynamicObj��(��) ���� ��ӵ�
    HRESULT Initialize() override;
    SceneType Update() override;
    void Render() override;
    void Release() override;
    Object* Clone() override;
};

