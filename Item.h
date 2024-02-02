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
    D3DXVECTOR3 randomPos; // ������ ������ ��ġ�� ����
    D3DXVECTOR3 scroll;
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

