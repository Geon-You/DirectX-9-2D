#pragma once
#include "Define.h"

class Texture abstract
{
public:
	Texture(void);
	virtual ~Texture(void);

public:
	virtual const TEXINFO* GetTexture(const TCHAR* key = nullptr
		, const int& cnt = 0) abstract;

	virtual HRESULT InsertTexture(const TCHAR* path
		, const TCHAR* key = nullptr, const int& cnt = 0) abstract;
	virtual void Release() abstract;
};

