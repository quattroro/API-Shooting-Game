#pragma once
#include <Windows.h>
#include <list>
#include <vector>

//��Ʈ�ѷ��� ��üŬ������ ���������� ���� ��Ʈ�ѷ��� base�� ��ӹް� �ؼ� �ߺ����� ����� �ҷ��� ���°��� ���� ���� Ŭ����
class BaseMainController
{
public:
	virtual ~BaseMainController()
	{

	}
	virtual BOOL isCollision(POINT pos1, int radious1, POINT pos2, int radious2) = 0;
	virtual BOOL DetectMapCollision(POINT pos, int radious) = 0;
	virtual POINT GetCharacterPos() = 0;
	virtual BOOL isMapCollision(POINT rectpos, int size, POINT cpos/*�߽���*/, int radious) = 0;
	virtual POINT GetScrollindex() = 0;
	virtual float GetPointDistnace(POINT pt1, POINT pt2) = 0;
	virtual POINT GetMousePos() = 0;
	virtual float GetAngle(POINT startpoint, POINT destpoint) = 0;
	virtual POINT GetScrollPos() = 0;
	
protected:
	
	
};

