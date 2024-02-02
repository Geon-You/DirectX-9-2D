#pragma once
#include "ObjProto.h"
class TitleObjProto :
    public ObjProto
{
public:
    HRESULT Initialize() override;

public:
    TitleObjProto();
    virtual ~TitleObjProto();
};

