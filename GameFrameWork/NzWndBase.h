#pragma once
#include "INC_Windows.h"

// 함수 선언
LRESULT CALLBACK NzWndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

// [CHECK] #1. 윈도우 클래스 생성 및 등록을 클래스로 구현한 예시.
// * 상속을 받아 확장할 수 있도록 구현.

class NzWndBase
{
public:
	NzWndBase() = default;
	virtual ~NzWndBase() = default;

	bool Create(const wchar_t* className, const wchar_t* windowName, int width, int height);
	void Destroy();

	void* GetHandle() const { return m_hWnd; }

	int GetWidth() const { return m_width; }
	int GetHeight() const { return m_height; }

protected:

	// [CHECK] #3 friend 로 선언하여 해당 함수에서 접근할 수 있도록 함. (왜일까요?)
	// NzWndBase 는 NzWndProc 를 친구로 인정. NzWndProc 는 NzWndBase 의 멤버 변수나 함수를 protected 나 private 이라도 사용가능
	friend LRESULT CALLBACK NzWndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

	// [CHECK] #4 메시지 프로시저를 가상함수로 구현하여 상속받은 클래스에서 재정의할 수 있도록 함.
	virtual void OnResize(int width, int height);
	virtual void OnClose() {}

	HWND m_hWnd = HWND();
	int m_width = 0;
	int m_height = 0;

	// 프로그래머의 의도를 코드로 강제해 주세요. 
	// 해당 객체는 복사 생성과 대입(=는 복사)를 막습니다.
	
	// [CHECK] #2. 무엇을 위한 코드일까요?
	NzWndBase(const NzWndBase&) = delete;
	NzWndBase& operator=(const NzWndBase&) = delete;
	NzWndBase(NzWndBase&&) = delete;
	NzWndBase& operator=(NzWndBase&&) = delete;
};