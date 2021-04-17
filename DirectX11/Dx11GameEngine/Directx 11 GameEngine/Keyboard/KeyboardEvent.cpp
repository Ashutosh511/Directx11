#include "KeyboardEvent.h"

KeyboardEvent::KeyboardEvent()
	:
	type(EventType::Invailid),
	key(0u)
{}

KeyboardEvent::KeyboardEvent(const EventType type , const unsigned char key)
	:
	type(type),
	key(key)
{}

bool KeyboardEvent::IsPressed() const
{
	return this->type == EventType::Press;
}

bool KeyboardEvent::IsReleased() const
{
	return this->type == EventType::Release;
}

bool KeyboardEvent::IsVailide() const
{
	return this->type != EventType::Invailid;
}

unsigned char KeyboardEvent::GetKeyCode() const
{
	return this->key;
}
