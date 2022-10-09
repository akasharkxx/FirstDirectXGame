#pragma once
#include "Point.h"

class InputListener 
{
public:
	InputListener() 
	{

	}

	~InputListener()
	{

	}

	// Keyboard pure virtual callback functions
	virtual void OnKeyDown(int key) = 0;
	virtual void OnKeyUp(int key) = 0;

	// Mouse pure virtual callback functions
	virtual void OnMouseMove(const Point& delta_mouse_pos) = 0;
	virtual void OnLeftMouseDown(const Point& delta_mouse_pos) = 0;
	virtual void OnLeftMouseUp(const Point& delta_mouse_pos) = 0;
	virtual void OnRightMouseDown(const Point& delta_mouse_pos) = 0;
	virtual void OnRightMouseUp(const Point& delta_mouse_pos) = 0;
	
};