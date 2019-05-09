/*
*
* @file		GameObject.cpp
* @date     Wed May  9 08:47:26 2018
* @author
*/

#include "GameObject.h"

int GameObject::GetX() const {
	return x;
}

int GameObject::GetY() const {
	return y;
}

void GameObject::SetX(const int _x) {
	x = _x;
}

void GameObject::SetY(const int _y) {
	y = _y;
}

void GameObject::SetVisible(const bool _isVisible) {
	isVisible = _isVisible;
}

bool GameObject::GetVisible() const{
	return isVisible;
}