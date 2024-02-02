#pragma once
#include "Scene.h"

class Ending :
    public Scene
{
public:
    // Scene��(��) ���� ��ӵ�
    HRESULT Initialize() override;
    void Update() override;
    void Render() override;
    void Release() override;

public:
    Ending();
    Ending(const INFO& info);
    virtual ~Ending();
};

