#pragma once
#include "Texture.h"
class MultiTexture :
	public Texture
{
public:
	MultiTexture();
	virtual ~MultiTexture();

private:
	// 연관된 여러 장의 텍스쳐를 담을 맵
	// ex) 슬라임 애니메이션이 모션 및 각 프레임 별로 분리되었을 경우
	// key - 슬라임 대기, value - 슬라임 대기 프레임 1~5
	// key - 슬라임 공격, value - 슬라임 공격 프레임 1~7
	map<const TCHAR*, vector<TEXINFO*>> texMap; 

public:
	// Texture을(를) 통해 상속됨
	virtual const TEXINFO* GetTexture(const TCHAR* key = nullptr, const int& cnt = 0) override;
	virtual HRESULT InsertTexture(const TCHAR* path, const TCHAR* key = nullptr, const int& cnt = 0) override;
	virtual void Release() override;
};

