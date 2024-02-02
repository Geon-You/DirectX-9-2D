#include "SingleTexture.h"
#include "Device.h"

SingleTexture::SingleTexture()
	: texInfo(nullptr)
{
}

SingleTexture::~SingleTexture()
{
	Release();
}

const TEXINFO* SingleTexture::GetTexture(const TCHAR* key, const int& cnt)
{
	return texInfo;
}

HRESULT SingleTexture::InsertTexture(const TCHAR* path, const TCHAR* key, const int& cnt)
{
	texInfo = new TEXINFO;
	ZeroMemory(texInfo, sizeof(TEXINFO));

	if (FAILED(D3DXGetImageInfoFromFile(path, &texInfo->ImgInfo)))
	{
		return E_FAIL;
	}

	if (FAILED(D3DXCreateTextureFromFileEx(
		GET_SINGLE(Device)->GetDevice() // 장치 객체 전달
		, path // 이미지 파일 경로
		, texInfo->ImgInfo.Width // 이미지 너비
		, texInfo->ImgInfo.Height // 이미지 높이
		, 1 // 이미지 밉맵 레벨
		, 0 // 텍스쳐 사용 방식 설정
		, texInfo->ImgInfo.Format // 이미지 포맷
		, D3DPOOL_MANAGED // 텍스쳐에 대한 메모리 관리 방식 설정
		, D3DX_DEFAULT // 텍스쳐 필터링 방식 설정
		, D3DX_DEFAULT // 밉맵 텍스쳐 필터링 방식 설정
		,  D3DCOLOR_ARGB(255, 255, 0, 255)	// 투명하게 만들 색상값 지정
		, &texInfo->ImgInfo // 이미지 정보 전달
		, NULL // 해당 이미지가 사용하는 r,g,b,a 채널 별 색상 강도 값을 받을 구조체 포인터
		, &texInfo->texture))) // 생성된 텍스쳐 객체를 받을 구조체 포인터
	{
		return E_FAIL;
	}

	return S_OK;
}

void SingleTexture::Release()
{
	if (texInfo)
	{
		texInfo->texture->Release();
		delete texInfo;
		texInfo = nullptr;
	}
}
