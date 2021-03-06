#include "MyCharacter.h"
#include "CMyMath.h"
#include <math.h>

CMyCharacter::CMyCharacter()
{
	m_pSprite = NULL;
	m_pSpriteHp = NULL;
}

CMyCharacter::~CMyCharacter()
{
}

bool CMyCharacter::Create(char * filename)
{
	if (m_pSprite != NULL)
		delete m_pSprite;

	m_pSprite = new dsTexture(filename);
	m_pSpriteHp = new dsTexture("player_hp.png");

	bool result = m_pSprite->LoadFromFilename(filename) &&
		m_pSpriteHp->LoadFromFilename("player_hp.png");

	m_IsBarrier = false;

	return result;
}

void CMyCharacter::OnDraw(float x, float y)
{
	if(!m_IsBarrier || m_barrierEffect)
		m_pSprite->DrawRotation(m_vecPosition.x, m_vecPosition.y, m_angle);
	
	int posY = 700;
	for (int i = m_hp; i > 0; i--) {
		m_pSpriteHp->Draw(946, posY, 0);
		posY -= 48;
	}
}

void CMyCharacter::OnUpdate(DWORD tick)
{

	int newPosX = m_vecPosition.x + (m_vecDirection.x * m_speed * tick / 1000);
	int newPosY = m_vecPosition.y + (m_vecDirection.y * m_speed * tick / 1000);

	if (newPosX >= 0 && newPosX <= 1024 - m_width) {
		m_vecPosition.x = newPosX;
	}

	if (newPosY >= 0 && newPosY <= 768 - m_height) {
		m_vecPosition.y = newPosY;
	}

	if (m_IsBarrier) {
		if (m_TimeBtwBarrier <= 0)
			m_IsBarrier = false;
		else {
			m_barrierEffect = (m_TimeBtwBarrier / 100) % 2 == 0;
			m_TimeBtwBarrier -= tick;
		}

	}
}

void CMyCharacter::Init(float x, float y)
{
	m_vecPosition.x = x;
	m_vecPosition.y = y;
	m_speed = 200;
	m_hp = 3;
	m_width = m_height = 64;
	m_IsBarrier = false;
}

void CMyCharacter::SetRotationAngle(POINT targetPos)
{
	//위로 향하는 기본 벡터
	Vector2 playerDirVec = { 0.0f, -1.0f };
	
	//점 - 점 = 벡터
	Vector2 vecTargetDirection = { targetPos.x - m_vecPosition.x,
		targetPos.y - m_vecPosition.y };

	//각도를 구함.
	m_angle = -RADIAN_DEGREE(atan2(vecTargetDirection.x, vecTargetDirection.y));

	m_vecAtkDirection = vecTargetDirection;
}

void CMyCharacter::SetMoveDirection(MOVEDIR dir)
{
	m_vecDirection = { 0,0 };

	if (dir == LEFT) {
		m_vecDirection.x -= 1;
	}	
	if (dir == RIGHT) {
		m_vecDirection.x += 1;
	}
		
	if (dir == UP) {
		m_vecDirection.y -= 1;
	}
		
	if (dir == DOWN) {
		m_vecDirection.y += 1;
	}	
}

Vector2 CMyCharacter::GetCenterPos()
{
	return Vector2();
}

RECT CMyCharacter::GetPlayerRect()
{
	RECT rect = { m_vecPosition.x, m_vecPosition.y,
	m_vecPosition.x + (m_width), m_vecPosition.y + (m_height) };
	return rect;
}

void CMyCharacter::OnCollisionEnter()
{
	if (!m_IsBarrier) {
		m_hp -= 1;
		m_IsBarrier = true;
		m_TimeBtwBarrier = 1000;
	}
}

bool CMyCharacter::IsHpZero()
{
	return m_hp <= 0;
}

void CMyCharacter::StopMove(MOVEDIR dir)
{
	if (dir == UP || dir == DOWN)
		m_vecDirection.y = 0;
	else if (dir == LEFT || dir == RIGHT)
		m_vecDirection.x = 0;
}
