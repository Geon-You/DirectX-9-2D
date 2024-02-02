#pragma once
#include "ObjProto.h"
class IngameObjProto :
    public ObjProto
{
public:
    HRESULT Initialize() override;

public:
    IngameObjProto();
    virtual ~IngameObjProto();
};

