#include "stdafx.h"
#include "Project.h"

using namespace basecross;

namespace Input
{
	const CONTROLER_STATE& GetPad()
	{
		return App::GetApp()->GetInputDevice().GetControllerVec()[0];
	}

	const KEYBOARD_STATE& GetKey()
	{
		return App::GetApp()->GetInputDevice().GetKeyState();
	}

	bool GetPadConected()
	{
		return GetPad().bConnected;
	}

	bool GetPushA()
	{
		if (GetPadConected())
		{
			return GetPad().wPressedButtons & XINPUT_GAMEPAD_A;
		}
		if (GetKey().m_bPressedKeyTbl[VK_SPACE])
		{
			return true;
		}

		return false;
	}

	bool GetPressA()
	{
		if (GetPadConected())
		{
			return GetPad().wButtons & XINPUT_GAMEPAD_A;
		}
		if (GetKey().m_bPushKeyTbl[VK_SPACE])
		{
			return true;
		}

		return false;
	}

	bool GetReleaseA()
	{
		if (GetPadConected())
		{
			return GetPad().wReleasedButtons & XINPUT_GAMEPAD_A;
		}
		if (GetKey().m_bUpKeyTbl[VK_SPACE])
		{
			return true;
		}

		return false;
	}

	Vec2 GetLStickValue()
	{
		if (GetPadConected())
		{
			const auto& pad = GetPad();
			
			Vec2 vec;
			vec.x = pad.fThumbLX;
			vec.y = pad.fThumbLY;

			return vec;
		}
		else
		{
			const auto& key = GetKey();

			Vec2 vec;
			if (key.m_bPushKeyTbl['W']) vec.y += 1.0f;
			if (key.m_bPushKeyTbl['A']) vec.x += -1.0f;
			if (key.m_bPushKeyTbl['S']) vec.y += -1.0f;
			if (key.m_bPushKeyTbl['D']) vec.x += 1.0f;

			return vec;
		}

		return Vec2(0.0f);
	}

	bool IsInputLStick()
	{
		return GetLStickValue().length() > 0.0f;
	}

	bool IsInputLStickX()
	{
		Vec2 stick = GetLStickValue();
		stick.y = 0.0f;
		return stick.length() > 0.0f;
	}

	bool IsInputLStickY()
	{
		Vec2 stick = GetLStickValue();
		stick.x = 0.0f;
		return stick.length() > 0.0f;
	}
}