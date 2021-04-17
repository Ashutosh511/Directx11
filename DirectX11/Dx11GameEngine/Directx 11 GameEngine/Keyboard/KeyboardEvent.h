#pragma once

class KeyboardEvent
{
public:
	enum  EventType
	{
		Press,
		Release,
		Invailid
	};

	KeyboardEvent();
	KeyboardEvent(const EventType type, const unsigned char key);
	bool IsPressed() const;
	bool IsReleased() const;
	bool IsVailide() const;
	unsigned char GetKeyCode() const;


private:
	EventType type;
	unsigned char key;
};