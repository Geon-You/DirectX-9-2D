#pragma once
#include "Define.h"
#include "Function.h"

class Texture;
class TextureMgr
{
public:
	DECLARE_SINGLETON(TextureMgr);

private:
	TextureMgr();
public:
	~TextureMgr();

private:
	map<const TCHAR*, Texture*>	texMap;

public:
	HRESULT InsertTexture(const TCHAR* path, const TCHAR* objKey, TexType texType
		, const TCHAR* stateKey = nullptr, const int& cnt = 0);
	const TEXINFO* GetTexture(const TCHAR* objKey, const TCHAR* stateKey = nullptr, const int& iCnt = 0);
	void Release();
};

