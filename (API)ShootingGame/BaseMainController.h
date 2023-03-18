#pragma once
#include <Windows.h>
#include <list>
#include <vector>

//컨트롤러와 객체클래스간 교차포함을 위해 컨트롤러도 base를 상속받게 해서 중복으로 헤더가 불러와 지는것을 막기 위한 클래스
class BaseMainController
{
public:
	virtual ~BaseMainController()
	{

	}
	virtual BOOL isCollision(POINT pos1, int radious1, POINT pos2, int radious2) = 0;
	virtual BOOL DetectMapCollision(POINT pos, int radious) = 0;
	virtual POINT GetCharacterPos() = 0;
	virtual BOOL isMapCollision(POINT rectpos, int size, POINT cpos/*중심점*/, int radious) = 0;
	virtual POINT GetScrollindex() = 0;
	virtual float GetPointDistnace(POINT pt1, POINT pt2) = 0;
	virtual POINT GetMousePos() = 0;
	virtual float GetAngle(POINT startpoint, POINT destpoint) = 0;
	virtual POINT GetScrollPos() = 0;
	
protected:
	
	
};

