#pragma once
#include "DynamicObj.h"

class Bullet :
    public DynamicObj
{
public:
    Bullet();
    Bullet(const INFO& info);
    virtual ~Bullet();

private:
    float damage;

private:
    void Hit();

public:
    void SetDamage(float damage) { this->damage = damage; }

public:
    // DynamicObj을(를) 통해 상속됨
    HRESULT Initialize() override;
    SceneType Update() override;
    void Render() override;
    void Release() override;
    Object* Clone() override;
};

