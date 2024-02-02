#pragma once
#include "Scene.h"

class Object;
class Title :
    public Scene
{
private:
    Object* bg;

public:
    HRESULT Initialize() override;
    void Update() override;
    void Render() override;
    void Release() override;

public:
    Title();
    virtual ~Title();
};

