#include "CodeLog.h"

void CodeLog::PushKey(KeyCode keycode)
{
	m_codeLock = false;
	if (!IsFull())
	{
		m_keyLog.emplace_back(keycode);
		m_size++;
		return;
	}
	else
	{
		m_keyLog.pop_front();
		m_keyLog.emplace_back(keycode);
	}
}

///////////////////////////////////////////////////////////////
// Currently has no use but Leaving this in as an example how to deal,
//	with code checks. Will improve once becomes an issue. Might look at using an array to pass as a 
//	param to check against the keyLog. 
///////////////////////////////////////////////////////////////
void CodeLog::CheckCode(KeyCode one, KeyCode two, KeyCode three, KeyCode four,
	KeyCode five, KeyCode six, KeyCode seven, KeyCode eight,
	KeyCode nine, KeyCode ten)
{
	if (!m_codeLock)
	{
		if (m_keyLog[0] == one)
		{
			if (m_keyLog[1] == two)
			{
				if (m_keyLog[2] == three)
				{
					if (m_keyLog[3] == four)
					{
						if (m_keyLog[4] == five)
						{
							if (m_keyLog[5] == six)
							{
								if (m_keyLog[6] == seven)
								{
									if (m_keyLog[7] == eight)
									{
										if (m_keyLog[8] == nine)
										{
											if (m_keyLog[9] == ten)
											{
												m_konamiActive = !m_konamiActive;
												m_codeLock = true;
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
}