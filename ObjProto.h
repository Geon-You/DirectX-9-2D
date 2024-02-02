#pragma once
#include "Prototype.h"
#include "Object.h"

class ObjProto :
    public Prototype
{
public:
	ObjProto(void);
	virtual ~ObjProto(void);

public:
	virtual HRESULT Initialize() abstract;
	virtual void Release();

protected:
	map<const TCHAR*, Object*>	protoObjs;

public:
	Object* GetProto(const TCHAR* key);
};

