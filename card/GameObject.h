/*
*
* @file		GameObject.h
* @date     Wed May  9 08:41:50 2018
* @author
*/

#pragma once
class GameObject
{
private:
	int x, y;
	bool isVisible;
public:
	int GetX() const;
	int GetY() const;
	void SetX(const int _x);
	void SetY(const int _y);
	bool GetVisible() const;
	void SetVisible(const bool _isVisible);
};

