#pragma once
#include "StaticObj.h"
class EndingBackGround :
    public StaticObj
{
public:
    EndingBackGround();
    EndingBackGround(const INFO& info);
    virtual ~EndingBackGround();

public:
    // StaticObj을(를) 통해 상속됨
    virtual HRESULT Initialize() override;
    virtual SceneType Update() override;
    virtual void Render() override;
    virtual void Release() override;
    virtual Object* Clone() override;
};

