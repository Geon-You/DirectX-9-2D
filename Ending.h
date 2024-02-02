#pragma once
#include "Scene.h"

class Ending :
    public Scene
{
public:
    // Scene을(를) 통해 상속됨
    HRESULT Initialize() override;
    void Update() override;
    void Render() override;
    void Release() override;

public:
    Ending();
    Ending(const INFO& info);
    virtual ~Ending();
};

