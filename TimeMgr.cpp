#include "TimeMgr.h"

TimeMgr::TimeMgr()
	:deltaTime(0)
{
}

TimeMgr::~TimeMgr()
{
}

void TimeMgr::Initialize(void)
{
	// 1초에 cpu가 카운팅하는 틱 수를 가져옴
	QueryPerformanceFrequency(&cpuTick);

	// 컴퓨터가 부팅된 후, 현재까지 카운팅된 틱 수
	// -> 여기서 저장한 틱 수는 큰 의미가 없음
	//	  타이머를 초기화하는 시점에 틱 카운트로 아래 변수의 값을 초기화
	//    델타 시간 연산을 위해 아래의 변수들을 정상적으로 사용하기 위해,
	//    초기화하는 용도로 사용했다고 생각하면 됨
	QueryPerformanceCounter(&currentTime);
	QueryPerformanceCounter(&prevTime);

}

void TimeMgr::Update(void)
{
	// 해당 함수를 반복되는 구조에서 계속해서 호출한다고 가정

	// 계속해서 변하는 현재 틱수를 가져옴
	QueryPerformanceCounter(&currentTime);

	// 델타 타임 = (현재 틱수 - 이전 틱수) / 초당틱수
	deltaTime = float(currentTime.QuadPart - prevTime.QuadPart) / (cpuTick.QuadPart * 0.001f);
	// 델타 타임을 구했으므로, 이전 틱수를 현재 틱수로 갱신
	prevTime = currentTime;
}
