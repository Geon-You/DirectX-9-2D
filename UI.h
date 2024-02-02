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
    float HpRatio; // �÷��̾� ���� ü�� ����
    D3DXMATRIX matScale;
    D3DXMATRIX matTrans;

public:
    // DynamicObj��(��) ���� ��ӵ�
    HRESULT Initialize() override;
    SceneType Update() override;
    void Render() override;
    void Release() override;
    Object* Clone() override;
};

