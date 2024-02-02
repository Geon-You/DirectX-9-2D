#pragma once
#include "Texture.h"
class SingleTexture :
	public Texture
{
public:
	SingleTexture();
	virtual ~SingleTexture();

private:
	TEXINFO* texInfo; // 텍스처 한장을 담을 필드

public:
	// Texture을(를) 통해 상속됨
	virtual const TEXINFO* GetTexture(const TCHAR* key = nullptr, const int& cnt = 0) override;
	virtual HRESULT InsertTexture(const TCHAR* path, const TCHAR* key = nullptr, const int& cnt = 0) override;
	virtual void Release() override;
};

