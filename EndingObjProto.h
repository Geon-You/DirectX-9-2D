#pragma once
#include "ObjProto.h"
class EndingObjProto :
    public ObjProto
{
public:
    // ObjProto을(를) 통해 상속됨
    virtual HRESULT Initialize() override;

public:
    EndingObjProto();
    virtual ~EndingObjProto();
};

