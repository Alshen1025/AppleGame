#include "INC_Windows.h"

#include "Collider.h"
#include "GameObject.h"
#include "RenderHelp.h"
#include <assert.h>

GameObject::~GameObject()
{
	if (m_pColliderCircle)
	{
		delete m_pColliderCircle;
		m_pColliderCircle = nullptr;
	}

	if (m_pColliderBox)
	{
		delete m_pColliderBox;
		m_pColliderBox = nullptr;
	}
}

void GameObject::SetBitmapInfo(BitmapInfo* bitmapInfo)
{
	assert(m_pBitmapInfo == nullptr && "BitmapInfo must be null!");
	assert(bitmapInfo != nullptr && "CreateBitmapInfo failed or bitmapInfo is null!");

	m_pBitmapInfo = bitmapInfo;

	m_frameWidth = m_pBitmapInfo->GetWidth();
	m_frameHeight = m_pBitmapInfo->GetHeight();
	m_frameIndex = 0;

	m_frameXY[0].x = 0;
	m_frameXY[0].y = 0;
}

void GameObject::Update(float deltaTime)
{
	//UpdateFrame(deltaTime);
	
	Move(deltaTime);
	// Collider 업데이트
}

void GameObject::Render(HDC hdc)
{
	if (isDelete) return;
	DrawBitmap(hdc);
	DrawCollider(hdc);
	
}

void GameObject::TextRender(HDC hdc)
{
	if (isDelete) return;
	DrawAppleText(hdc);
}


void GameObject::SetColliderCircle(float radius)
{
	if (m_pColliderCircle)
	{
		delete m_pColliderCircle;
		m_pColliderCircle = nullptr;
	}

	m_pColliderCircle = new ColliderCircle;

	assert(m_pColliderCircle != nullptr && "Failed to create ColliderCircle!");

	m_pColliderCircle->radius = radius;
	m_pColliderCircle->center = m_pos;
}


void GameObject::SetColliderBox(float width, float height)
{
	if (m_pColliderBox)
	{
		delete m_pColliderBox;
		m_pColliderBox = nullptr;
	}

	m_pColliderBox = new ColliderBox;

	assert(m_pColliderBox != nullptr && "Failed to create ColliderBox!");

	m_pColliderBox->center = m_pos;
	m_pColliderBox->halfSize.x = width / 2.0f;
	m_pColliderBox->halfSize.y = height / 2.0f;
}

// 과제: 해당 코드의 문제는 무엇일까요? 어떻게 개선하면 좋을까요?
// 개선 방향에 대해 서로 토론하고 비교해 보세요.
void GameObject::DrawCollider(HDC hdc)
{
	HPEN hPen = CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
	HPEN hOldPen = (HPEN)SelectObject(hdc, hPen);
	HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, GetStockObject(NULL_BRUSH));

	if (m_pColliderCircle)
	{
		Ellipse(hdc, m_pColliderCircle->center.x - m_pColliderCircle->radius,
			m_pColliderCircle->center.y - m_pColliderCircle->radius,
			m_pColliderCircle->center.x + m_pColliderCircle->radius,
			m_pColliderCircle->center.y + m_pColliderCircle->radius);
	}

	if (m_pColliderBox)
	{
		Rectangle(hdc, m_pColliderBox->center.x - m_pColliderBox->halfSize.x,
			m_pColliderBox->center.y - m_pColliderBox->halfSize.y,
			m_pColliderBox->center.x + m_pColliderBox->halfSize.x,
			m_pColliderBox->center.y + m_pColliderBox->halfSize.y);
	}

	// 이전 객체 복원 및 펜 삭제
	SelectObject(hdc, hOldPen);
	SelectObject(hdc, hOldBrush);
	DeleteObject(hPen);
}

void GameObject::DrawAppleText(HDC hdc)
{
	int centerX = static_cast<int>(m_pos.x);
	int centerY = static_cast<int>(m_pos.y);

	RECT rect = {
		centerX - m_width / 2,
		centerY - m_height / 2,
		centerX + m_width / 2,
		centerY + m_height / 2
	};

	wchar_t buffer[16];
	swprintf_s(buffer, L"%d", AppleNumber);

	SetBkMode(hdc, TRANSPARENT);
	SetTextColor(hdc, RGB(255, 255, 255));

	DrawText(
		hdc,
		buffer,
		-1,
		&rect,
		DT_CENTER | DT_VCENTER | DT_SINGLELINE
	);
}

void GameObject::Move(float deltaTime)
{
	GameObjectBase::Move(deltaTime);
}

void GameObject::DrawBitmap(HDC hdc)
{
	if (m_pBitmapInfo == nullptr) return;
	if (m_pBitmapInfo->GetBitmapHandle() == nullptr) return;

	HDC hBitmapDC = CreateCompatibleDC(hdc);

	HBITMAP hOldBitmap = (HBITMAP)SelectObject(hBitmapDC, m_pBitmapInfo->GetBitmapHandle());
	// BLENDFUNCTION 설정 (알파 채널 처리)
	BLENDFUNCTION blend = { 0 };
	blend.BlendOp = AC_SRC_OVER;
	blend.SourceConstantAlpha = 255;  // 원본 알파 채널 그대로 사용
	blend.AlphaFormat = AC_SRC_ALPHA;

	const int x = m_pos.x - m_width / 2;
	const int y = m_pos.y - m_height / 2;

	const int srcX = m_frameXY[m_frameIndex].x;
	const int srcY = m_frameXY[m_frameIndex].y;

	AlphaBlend(hdc, x, y, m_width, m_height,
		hBitmapDC, srcX, srcY, m_frameWidth, m_frameHeight, blend);

	// 비트맵 핸들 복원
	SelectObject(hBitmapDC, hOldBitmap);
	DeleteDC(hBitmapDC);
}

void GameObject::UpdateFrame(float deltaTime)
{
	m_frameTime += deltaTime;
	if (m_frameTime >= m_frameDuration)
	{
		m_frameTime = 0.0f;
		m_frameIndex = (m_frameIndex + 1) % (m_frameCount);
	}
}

void GameObjectBase::SetName(const char* name)
{
	if (name == nullptr) return;

	strncpy_s(m_name, name, OBJECT_NAME_LEN_MAX - 1);
	m_name[OBJECT_NAME_LEN_MAX - 1] = '\0';
}

void GameObjectBase::ChaseTarget(Vector2f target, Vector2f self)
{
	SetDirection(target - self);
}
