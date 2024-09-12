#pragma once

#include <deque>

using KeyCode = uint32_t;

///////////////////////////////////////////////////////////////
// TODO(Shilo):Currently unused in the engine although I should modify this portion,
//	To allow for "Cheats" to be added using codes. So might need to think about taking this away from the
//	keyboard area into maybe a settings?? unknown at this time but will leave here for WIP. 
///////////////////////////////////////////////////////////////
class CodeLog
{
private:
	std::deque<uint32_t> m_keyLog;
	int m_size = 0;
	int m_capacity = 10;
	bool m_konamiActive = false;
	bool m_codeLock = true;
public:
	void PushKey(KeyCode keycode);
	int Size()const { return m_size; }
	int Capacity()const { return m_capacity; }
	bool IsFull() { return m_size == m_capacity; }
	bool IsCodeActive() { return m_konamiActive; }
	void CheckCode(KeyCode one, KeyCode two, KeyCode three, KeyCode four, KeyCode five,
		KeyCode six, KeyCode seven, KeyCode eight, KeyCode nine, KeyCode ten);

};

