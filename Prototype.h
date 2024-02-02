#pragma once
#include "Define.h"
#include "Function.h"

class Prototype abstract
{
public:
	Prototype(void);
	virtual ~Prototype(void);

public:
	virtual HRESULT Initialize() abstract;
	virtual void Release() abstract;
};

