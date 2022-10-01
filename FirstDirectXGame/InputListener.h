#pragma once

class InputListener 
{
public:
	InputListener() 
	{

	}

	~InputListener()
	{

	}

	// pure virtual callback functions

	virtual void OnKeyDown(int key) = 0;
	virtual void OnKeyUp(int key) = 0;
};