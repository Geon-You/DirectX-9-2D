#pragma once
#include "Object.h"

class StaticObj abstract :
    public Object
{
public:
    StaticObj();
    StaticObj(const INFO& info);
    virtual ~StaticObj();

public:
    virtual HRESULT Initialize() abstract;
    virtual SceneType Update() abstract;
    virtual void Render() abstract;
    virtual void Release() abstract;
    virtual Object* Clone() abstract;
};

