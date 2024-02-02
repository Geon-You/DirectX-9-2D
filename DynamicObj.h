#pragma once
#include "Object.h"
class DynamicObj abstract :
    public Object
{
public:
    DynamicObj();
    DynamicObj(const INFO& info);
    virtual ~DynamicObj();

protected:
    float moveSpeed;

public:
    virtual HRESULT Initialize() abstract;
    virtual SceneType Update() abstract;
    virtual void Render() abstract;
    virtual void Release() abstract;
    virtual Object* Clone() abstract;
};

