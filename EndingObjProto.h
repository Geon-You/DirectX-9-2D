#pragma once
#include "ObjProto.h"
class EndingObjProto :
    public ObjProto
{
public:
    // ObjProto��(��) ���� ��ӵ�
    virtual HRESULT Initialize() override;

public:
    EndingObjProto();
    virtual ~EndingObjProto();
};

