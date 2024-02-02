#include "Device.h"

Device::Device(void)
	:direct(NULL)
	, device(NULL)
	, sprite(NULL)
	, font(NULL)
	, line(NULL)
{
}

Device::~Device(void)
{
	this->Release();
}

HRESULT Device::InitDevice(WINMODE mode)
{
	// 1. 다이렉트 객체 생성
	// 2. 다이렉트 객체를 통한 장치 성능 조사
	// 3. 조사한 장치 정보를 토대로 렌더 처리에 대한 각종 설정
	// 4. 설정 값을 토대로 최종적인 디바이스 장치 생성

	direct = Direct3DCreate9(D3D_SDK_VERSION);

	// 장치 성능에 대한 정보를 담을 구조체
	D3DCAPS9 devicecaps;
	ZeroMemory(&devicecaps, sizeof(D3DCAPS9));

	// 장치의 성능을 조사한다.
	if (FAILED(direct->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &devicecaps)))
	{
		ERR_MSG(g_hWnd, L"장치 정보얻어오기 실패");
		return E_FAIL;
	}

	DWORD BehaviorFlag = 0;
	// 하드웨어 버텍스 프로세싱 지원 여부 확인
	if (devicecaps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
	{
		// 지원한다면 하드웨어 버텍스 프로세싱 사용
		BehaviorFlag = D3DCREATE_HARDWARE_VERTEXPROCESSING;
	}
	else
	{
		// 아니라면 소프트웨어 버텍스 프로세싱 사용
		BehaviorFlag = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
	}

	// 서피스는 아래의 버퍼들의 갖음
	// 1. 지오메트리 or 이미지 처리에 의해 생성된 픽셀 (컬러 버퍼 -> 전면/후면 버퍼)
	// 2. 깊이/스텐실 정보 (깊이/스텐실 버퍼)

	// 전면(프론트) 버퍼 : 현재 비디오에 장치에 출력되고 있는 메모리 영역
	// 후면(백) 버퍼 : 출력 속도를 향상을 위해 프론트 버퍼와 동일한 색상 정보를
	//					저장하고 있는 메모리 영역
	//					컬러 버퍼 -> 픽셀 저장 
	//					깊이 버퍼 -> 새로운 픽셀을 컬러 버퍼에 덮어 쓸 것인가에 대한
	//								 판별 값  (기준 z 값, 따라서 z buffer 라고도 함)
	//					스텐실 버퍼 -> 깊이 버퍼와 비슷, 특정 영역을 그릴지에 대한 여부 (기준 masking 값)

	// 렌더에 사용할 전면/후면 및 깊이/스텐실 버퍼에 대한 설정 값을 담을 구조체
	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));

	// 후면 버퍼 설정
	d3dpp.BackBufferWidth = WIN_SIZE_X;
	d3dpp.BackBufferHeight = WIN_SIZE_Y;
	d3dpp.BackBufferCount = 1;
	d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;

	// 깊이 및 스텐실 버퍼 설정
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;
	d3dpp.EnableAutoDepthStencil = true;

	// 최종적으로 렌더할 윈도우의 핸들을 설정 
	d3dpp.hDeviceWindow = g_hWnd;
	// 윈도우 모드 (창, 풀)
	d3dpp.Windowed = mode;
	
	// 스왑체인 -> 복수의 서피스를 하나의 컬렉션으로 관리
	// 서피스1    서피스2
	// 전면 버퍼  후면 버퍼
	// 서피스2    서피스1
	// 전면 버퍼  후면 버퍼

	// 스왑 체인 (더블 버퍼링) 교환 방식을 설정
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	
	// 화면 재생률 설정(화면을 새로 고치는 속도) (창모드면 0, 풀스크린이면 지원하는 hz 설정)
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	// 후면 버퍼를 전면 버퍼에 표시할 수 있는 최대 속도
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

	// 장치 생성
	if (FAILED(direct->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL
		, g_hWnd, BehaviorFlag, &d3dpp, &device)))
	{
		ERR_MSG(g_hWnd, L"장치생성 실패");
		return E_FAIL;
	}

	// 스프라이트 객체 생성
	if (FAILED(D3DXCreateSprite(device, &sprite)))
	{
		return E_FAIL;
	}

	// 폰트 정보를 담을 구조체
	D3DXFONT_DESC tFontInfo;
	ZeroMemory(&tFontInfo, sizeof(tFontInfo));

	tFontInfo.Height = 20;
	tFontInfo.Width = 10;
	tFontInfo.Weight = FW_HEAVY;
	tFontInfo.CharSet = HANGUL_CHARSET;
	lstrcpy(tFontInfo.FaceName, L"궁서");

	// 폰트 생성
	if (FAILED(D3DXCreateFontIndirect(device, &tFontInfo, &font)))
	{
		ERR_MSG(g_hWnd, L"폰트생성 실패");
	}

	if (FAILED(D3DXCreateLine(device, &line)))
	{
		ERR_MSG(g_hWnd, L"라인생성 실패");
	}

	return S_OK;
}

//1. 화면을 Clear() 함수로 초기화 한다.
//2. 장면을 후면 버퍼에 그리기 위해 디바이스의 BeginScene() 함수를 호출한다.
//3. 장면을 그린다.
//4. 장면을 다 그렸다는 것을 EndScene() 함수로 알린다.
//5. Present() 함수로 전면 버퍼와 후면 버퍼를 교체한다.
//6. 1부터 5를 반복한다
void Device::RenderBegin()
{
	device->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL
		, D3DCOLOR_ARGB(255, 0, 0, 0), 1.f, 0);
	device->BeginScene();
	sprite->Begin(D3DXSPRITE_ALPHABLEND);
}

void Device::RenderEnd()
{
	sprite->End();
	device->EndScene();
	device->Present(NULL, NULL, NULL, NULL);
}

void Device::Release()
{
	sprite->Release();
	device->Release();
	direct->Release();
}
