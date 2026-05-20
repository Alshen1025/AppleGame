#include "INC_Windows.h"
#include "MyFirstWndGame.h"
#include "GameTimer.h"
#include "Collider.h"
#include "GameObject.h"
#include "RenderHelp.h"
#include <iostream>
#include <random>
#include <assert.h>
#include <algorithm>
using namespace learning;

constexpr int MAX_GAME_OBJECT_COUNT = 1000;

bool MyFirstWndGame::Initialize()
{
    m_pGameTimer = new GameTimer();
    m_pGameTimer->Reset();

    const wchar_t* className = L"MyFirstWndGame";
    const wchar_t* windowName = L"MyFirstWndGame";

    if (false == __super::Create(className, windowName, 1024, 720))
    {
        return false;
    }


    RECT rcClient = {};
    GetClientRect(m_hWnd, &rcClient);
    m_width = rcClient.right - rcClient.left;
    m_height = rcClient.bottom - rcClient.top;

    m_hFrontDC = GetDC(m_hWnd);
    m_hBackDC = CreateCompatibleDC(m_hFrontDC);
    m_hBackBitmap = CreateCompatibleBitmap(m_hFrontDC, m_width, m_height);

    m_hDefaultBitmap = (HBITMAP)SelectObject(m_hBackDC, m_hBackBitmap);

    m_GameObjectPtrTable = new GameObjectBase * [MAX_GAME_OBJECT_COUNT];

    for (int i = 0; i < MAX_GAME_OBJECT_COUNT; ++i)
    {
        m_GameObjectPtrTable[i] = nullptr;
    }

    // 필요한 리소스를 로드해볼까요
#pragma region resource

    // 파일의 절대 경로와 상대 경로 구분
    //m_pAppleBitmapInfo = renderHelp::CreateBitmapInfo(L"./GameFrameWork/Resource/graybird.png");
    m_pAppleBitmapInfo = renderHelp::CreateBitmapInfo(L".\\Resource\\apple.png");
#pragma endregion

    // 리소스를 먼저 로드한 후에 세팅을 합니다.

	// [CHECK]. 첫 번째 게임 오브젝트는 플레이어 캐릭터로 고정!
	//CreatePlayer();
    InitApples();
    return true;

}

void MyFirstWndGame::Run()
{
    MSG msg = { 0 };
    while (msg.message != WM_QUIT)
    {
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_LBUTTONDOWN)
            {
                MyFirstWndGame::OnLButtonDown(LOWORD(msg.lParam), HIWORD(msg.lParam));
            }
            else if (msg.message == WM_LBUTTONUP)
            {
                MyFirstWndGame::OnLButtonUP(LOWORD(msg.lParam), HIWORD(msg.lParam));
            }
            else if (msg.message == WM_MOUSEMOVE)
            {
                MyFirstWndGame::OnMouseMove(LOWORD(msg.lParam), HIWORD(msg.lParam));
            }
            else
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }
        else
        {
            Update();
            Render();
        }
    }
}

void MyFirstWndGame::Finalize()
{
    delete m_pGameTimer;
    m_pGameTimer = nullptr;

    if (m_GameObjectPtrTable)
    {
        for (int i = 0; i < MAX_GAME_OBJECT_COUNT; ++i)
        {
            if (m_GameObjectPtrTable[i])
            {
                delete m_GameObjectPtrTable[i];
                m_GameObjectPtrTable[i] = nullptr;
            }
        }
        delete m_GameObjectPtrTable;
    }

    __super::Destroy();
}

void MyFirstWndGame::FixedUpdate()
{
}

void MyFirstWndGame::LogicUpdate()
{
    UpdateLBtnState();
    for (int i = 0; i < MAX_GAME_OBJECT_COUNT; ++i)
    {
        if (m_GameObjectPtrTable[i])
        {
            m_GameObjectPtrTable[i]->Update(m_fDeltaTime);
        }
    }
}

void MyFirstWndGame::CheckAppleResult(ArrayIndex indexStart, ArrayIndex indexEnd)
{
    std::cout << "CheckAppleResult!" << std::endl;
    std::cout << "indexStart.x : " << indexStart.x << " indexEnd.x : " << indexEnd.x << std::endl;
    std::cout << "indexStart.y : " << indexStart.y << " indexEnd.y : " << indexEnd.y << std::endl;
    //int widthCount = std::abs(indexStart.x - indexEnd.x) + 1;
    //int heightCount = std::abs(indexStart.y - indexEnd.y) + 1;
    ////일단 가로, 세로가 1인 직사각형에 한해서만 판단하도록 하기
    //if (widthCount != 1 && heightCount != 1)
    //{
    //    return;
    //}
    int maxX = std::max(indexStart.x, indexEnd.x);
    int minX = std::min(indexStart.x, indexEnd.x);
    int maxY = std::max(indexStart.y, indexEnd.y);
    int minY = std::min(indexStart.y, indexEnd.y);

    if (IsOverTen(maxX, minX, maxY, minY))
    {
        std::cout << "Is Ten!" << std::endl;
        HideApple(maxX, minX, maxY, minY);
    }
}

void MyFirstWndGame::HideApple(int MaxX, int MinX, int MaxY, int MinY)
{
    std::cout << "HideApple" << std::endl;
    for (int i = MinX; i <= MaxX; i++) {
        for (int j = MinY; j <= MaxY; j++) {
            ApplesArray[i][j]->SetIsDelete(true);
        }
    }
}

bool MyFirstWndGame::IsOverTen(int MaxX, int MinX, int MaxY, int MinY)
{
    int ten = 0;
    for (int i = MinX; i <= MaxX; i++) {
        for (int j = MinY; j <= MaxY; j++) {
            if (i < 0 || i > 19 || j < 0 || j > 12) continue;
            if (ApplesArray[i][j]->GetIsDelete()) continue;
            ten += ApplesArray[i][j]->GetAppleNumber();
        }
    }
    std::cout << ten << std::endl;
    return (ten == 10);
}

ArrayIndex MyFirstWndGame::MousePosToIndex(int x, int y)
{
    const int firstCenterX = 100;
    const int firstCenterY = 100;
    const int appleSize = 40;

    const int startX = firstCenterX - appleSize / 2;
    const int startY = firstCenterY - appleSize / 2;

    const int endX = startX + m_appleWidth * appleSize;
    const int endY = startY + m_appleHeight * appleSize;

    ArrayIndex index;
    if (x < startX || x >= endX || y < startY || y >= endY)
    {
        return index;
    }

    index.x = (x - startX) / appleSize;
    index.y = (y - startY) / appleSize;

    return index;
}

void MyFirstWndGame::UpdateLBtnState()
{
    if (isLBtnDown && isLBtnUp)
    {
        std::cout << "Make Rect!" << std::endl;
        ArrayIndex startIndex = MousePosToIndex(m_FirstPoint.x, m_FirstPoint.y);
        ArrayIndex endndex = MousePosToIndex(m_SecondPoint.x, m_SecondPoint.y);
        CheckAppleResult(startIndex, endndex);
        isLBtnDown = false;
        isLBtnUp = false;
    }
}


std::vector<int> MyFirstWndGame::GenerateRandomNumber(int width, int height, int minSolutions)
{
    std::random_device rd;
    unsigned long long seed = rd();
    std::mt19937 gen(seed);
    std::uniform_int_distribution<int> dist(1, 9);
    int attempts = 0;
    int totalCells = 20 * 13;

    while (true) {
        attempts++;
        std::vector<int> board(totalCells);

        // 랜덤 채우기
        for (int i = 0; i < totalCells; ++i) {
            board[i] = dist(gen);
        }

        // 검증
        int solutions = CountRectangles(width, height, board);
        if (solutions >= minSolutions) {
            std::printf("[Success] Attempts: %d | Solutions: %d\n", attempts, solutions);
            return board;
        }
    }
}

void MyFirstWndGame::InitApples()
{
    appleMap.resize(m_appleWidth * m_appleHeight);
    appleMap = GenerateRandomNumber(m_appleWidth, m_appleHeight, m_minSolutions);
    const int startX = 100;
    const int startY = 100;
    const int appleSize = 40;
    for(int i = 0; i <= 19; i++){
        for (int j = 0; j <= 12; j++)
        {
            GameObject* pNewObject = new GameObject(ObjectType::APPLE);
            int posX = startX + (i * appleSize);
            int posY = startY + (j * appleSize);
            pNewObject->SetPosition(posX, posY);
            pNewObject->SetWidth(appleSize);
            pNewObject->SetHeight(appleSize);
            pNewObject->SetBitmapInfo(m_pAppleBitmapInfo);
            pNewObject->SetAppleNumber(appleMap[CoordinateToIndex(i, j, m_appleWidth)]);
            ApplesArray[i][j] = pNewObject;
            PutAppleToArray(pNewObject);
        }
    }
}

void MyFirstWndGame::PutAppleToArray(GameObject* apple)
{
    int i = 0;
    while (i < MAX_GAME_OBJECT_COUNT) 
    {
        if (nullptr == m_GameObjectPtrTable[i])
        {
            m_GameObjectPtrTable[i] = apple;
            break;
        }
        i++;
    }
}

int MyFirstWndGame::CountRectangles(int width, int height, const std::vector<int>& board)
{
    int count = 0;
    int psWidth = width + 1;
    int psHeight = height + 1;

    // 2차원 누적합을 위한 1차원 배열 (크기: (W+1) * (H+1))
    std::vector<int> prefixSum(psWidth * psHeight, 0);

    // 누적합 계산
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            int currentVal = board[CoordinateToIndex(x, y, width)];

            // S[y+1][x+1] = A[y][x] + S[y][x+1] + S[y+1][x] - S[y][x]
            prefixSum[CoordinateToIndex(x + 1, y + 1, psWidth)] = currentVal
                + prefixSum[CoordinateToIndex(x + 1, y, psWidth)]
                + prefixSum[CoordinateToIndex(x, y + 1, psWidth)]
                - prefixSum[CoordinateToIndex(x, y, psWidth)];
        }
    }

    // 모든 가능한 직사각형 영역 검사
    for (int y1 = 1; y1 <= height; ++y1) {
        for (int x1 = 1; x1 <= width; ++x1) {
            for (int y2 = y1; y2 <= height; ++y2) {
                for (int x2 = x1; x2 <= width; ++x2) {

                    // 구간합 공식 적용
                    int areaSum = prefixSum[CoordinateToIndex(x2, y2, psWidth)]
                        - prefixSum[CoordinateToIndex(x2, y1 - 1, psWidth)]
                        - prefixSum[CoordinateToIndex(x1 - 1, y2, psWidth)]
                        + prefixSum[CoordinateToIndex(x1 - 1, y1 - 1, psWidth)];

                    if (areaSum == 10) count++;
                }
            }
        }
    }
    return count;
}

int MyFirstWndGame::CoordinateToIndex(int x, int y, int width)
{
    return y * width + x;
}



void MyFirstWndGame::Update()
{
    m_pGameTimer->Tick();

    LogicUpdate();

    m_fDeltaTime = m_pGameTimer->DeltaTimeMS();
    m_fFrameCount += m_fDeltaTime;

    while (m_fFrameCount >= 200.0f)
    {
        FixedUpdate();
        m_fFrameCount -= 200.0f;
    }
}

void MyFirstWndGame::Render()
{
    //Clear the back buffer
    ::PatBlt(m_hBackDC, 0, 0, m_width, m_height, WHITENESS);

    //메모리 DC에 그리기
    for (int i = 0; i < MAX_GAME_OBJECT_COUNT; ++i)
    {
        if (m_GameObjectPtrTable[i])
        {
            m_GameObjectPtrTable[i]->Render(m_hBackDC);
        }
    }
    for (int i = 0; i < MAX_GAME_OBJECT_COUNT; ++i)
    {
        if (m_GameObjectPtrTable[i])
        {
            m_GameObjectPtrTable[i]->TextRender(m_hBackDC);
        }
    }

    //메모리 DC에 그려진 결과를 실제 DC(m_hFrontDC)로 복사
    BitBlt(m_hFrontDC, 0, 0, m_width, m_height, m_hBackDC, 0, 0, SRCCOPY);
}

void MyFirstWndGame::OnResize(int width, int height)
{
    std::cout << __FUNCTION__ << std::endl;

    learning::SetScreenSize(width, height);

    __super::OnResize(width, height);

    m_hBackBitmap = CreateCompatibleBitmap(m_hFrontDC, m_width, m_height);

    HANDLE hPrevBitmap = (HBITMAP)SelectObject(m_hBackDC, m_hBackBitmap);

    DeleteObject(hPrevBitmap);
}

void MyFirstWndGame::OnClose()
{
    std::cout << __FUNCTION__ << std::endl;

    SelectObject(m_hBackDC, m_hDefaultBitmap);

    DeleteObject(m_hBackBitmap);
    DeleteDC(m_hBackDC);

    ReleaseDC(m_hWnd, m_hFrontDC);
}

void MyFirstWndGame::OnMouseMove(int x, int y)
{
    /*   std::cout << __FUNCTION__ << std::endl;
       std::cout << "x: " << x << ", y: " << y << std::endl;*/
    m_MousePosPrev = m_MousePos;
    m_MousePos = { x, y };
}

void MyFirstWndGame::OnLButtonDown(int x, int y)
{
    std::cout << "OnLButtonDown" << std::endl;
    std::cout << "x: " << x << ", y: " << y << std::endl;
    m_FirstPoint.x = x;
    m_FirstPoint.y = y;
    isLBtnDown = true;

}

void MyFirstWndGame::OnLButtonUP(int x, int y)
{
    std::cout << "OnLButtonUP" << std::endl;
    std::cout << "x: " << x << ", y: " << y << std::endl;
    m_SecondPoint.x = x;
    m_SecondPoint.y = y;
    isLBtnUp = true;

}

void MyFirstWndGame::OnRButtonDown(int x, int y)
{
	std::cout << __FUNCTION__ << std::endl;
	std::cout << "x: " << x << ", y: " << y << std::endl;

    m_EnemySpawnPos.x = x;
    m_EnemySpawnPos.y = y;
}