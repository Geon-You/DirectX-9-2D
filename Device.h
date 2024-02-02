#pragma once
#include "Define.h"
#include "Function.h"

class Device
{
public:
	DECLARE_SINGLETON(Device);

private:
	Device(void);
public:
	~Device(void);

private:
	// 장치 성능 조사를 위한 다이렉트 객체
	LPDIRECT3D9	direct;

	// 장치를 대표하는 객체
	LPDIRECT3DDEVICE9 device;

	// 이미지 렌더를 대표하는 sprite객체gh
	LPD3DXSPRITE	sprite;

	// 폰트
	LPD3DXFONT		font;
	// 라인
	LPD3DXLINE      line;

public:
	HRESULT InitDevice(WINMODE mode);
	void RenderBegin();
	void RenderEnd();
	void Release();
public:
	LPDIRECT3DDEVICE9 GetDevice() { return device; }
	LPD3DXSPRITE	  GetSprite() { return sprite; }
	LPD3DXFONT		  GetFont() { return font; }
	LPD3DXLINE	      GetLine() { return line; }
};
