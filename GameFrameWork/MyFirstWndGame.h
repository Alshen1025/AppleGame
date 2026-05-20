#pragma once

#include "NzWndBase.h"
#include "Utillity.h"
#include <vector>

// [CHECK] #7  전방 선언을 사용하여 헤더파일의 의존성을 줄임.
class GameTimer;
class GameObjectBase;
class GameObject;

namespace renderHelp
{
	class BitmapInfo;
}

// ArrayIndex 구조체 정의 (헤더 파일에 선언 필요)
struct ArrayIndex
{
    int x;
    int y;
};


class MyFirstWndGame : public NzWndBase
{
public:
	MyFirstWndGame() = default;
	~MyFirstWndGame() override = default;

	bool Initialize();
	void Run();
	void Finalize();

private:
	void Update();
	void Render();

	void OnResize(int width, int height) override;
	void OnClose() override;

	void OnMouseMove(int x, int y);
	void OnLButtonDown(int x, int y);
	void OnLButtonUP(int x, int y);
	void OnRButtonDown(int x, int y);

	void FixedUpdate();
	void LogicUpdate();

	//직사각형 사과 판정
	void CheckAppleResult(ArrayIndex indexStart, ArrayIndex indexEnd);
	bool IsOverTen(int MaxX, int MinX, int MaxY, int MinY);
	ArrayIndex MousePosToIndex(int x, int y);

	GameObject* GetPlayer() const { return (GameObject*)m_GameObjectPtrTable[0]; }

	void UpdateLBtnState();

	//사과 게임 관련 함수들

	//난수 생성기 초기화
	//랜덤 숫자 생성
	std::vector<int> GenerateRandomNumber(int width, int height, int minSolutions);
	//사과 생성
	void InitApples();
	//생성한 사과 오브젝트 배열에 추가하는 함수
	void PutAppleToArray(GameObject* apple);
	//사과 생성 알고리즘
	int CountRectangles(int width, int height, const std::vector<int>& board);
	int CoordinateToIndex(int x, int y, int width);

	void HideApple(int MaxX, int MinX, int MaxY, int MinY);

private:
	HDC m_hFrontDC = nullptr;
	HDC m_hBackDC = nullptr;
	HBITMAP m_hBackBitmap = nullptr;
	HBITMAP m_hDefaultBitmap = nullptr;

	// [CHECK] #8. 게임 타이머를 사용하여 프레임을 관리하는 예시.
	GameTimer* m_pGameTimer = nullptr;

	float m_fDeltaTime = 0.0f;
	float m_fFrameCount = 0.0f;

	// [CHECK] #8. 게임 오브젝트를 관리하는 컨테이너.
	GameObjectBase** m_GameObjectPtrTable = nullptr;

	//사과 게임 사과 배열
	GameObject* ApplesArray[20][13] = { nullptr };
	//사과에 담을 숫자 배열
	std::vector<int> appleMap;

	int m_appleWidth = 20;
	int m_appleHeight = 13;
	//난이도 조절 용
	int m_minSolutions = 100;

	struct MOUSE_POS
	{
		int x = 0;
		int y = 0;

		bool operator!=(const MOUSE_POS& other) const
		{
			return (x != other.x || y != other.y);
		}
	};

	MOUSE_POS m_MousePos = { 0, 0 };
	MOUSE_POS m_MousePosPrev = { 0, 0 };

	MOUSE_POS m_PlayerTargetPos = { 0, 0 };
	MOUSE_POS m_EnemySpawnPos = { 0, 0 };

	MOUSE_POS m_FirstPoint = { 0, 0 };
	MOUSE_POS m_SecondPoint = { 0, 0 };
	bool isLBtnDown = false;
	bool isLBtnUp = false;
	

	using BitmapInfo = renderHelp::BitmapInfo;

	BitmapInfo* m_pPlayerBitmapInfo = nullptr;
	BitmapInfo* m_pEnemyBitmapInfo = nullptr;
	BitmapInfo * m_pAppleBitmapInfo = nullptr;

	//사과 테이블 밖 범위
	int boardLeft = 80;
	int boardTop = 80;
	int boardRight = 880;
	int boardBottom = 600;
};