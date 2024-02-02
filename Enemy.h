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
    D3DXMATRIX matRotZ; // ȸ�� ��� (z�� ȸ��)
    D3DXMATRIX matTrans; // �̵� ���
    // ������ ���� ��ġ
    int randomPosX;
    int randomPosY;
    float health; // ü��
    D3DXVECTOR3 scroll;

public:
    void Hit(float damge);

public:
    // DynamicObj��(��) ���� ��ӵ�
    HRESULT Initialize() override;
    SceneType Update() override;
    void Render() override;
    void Release() override;
    Object* Clone() override;
};

