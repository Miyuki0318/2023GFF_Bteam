#include "stdafx.h"
#include "Project.h"

using namespace basecross;

namespace Input
{
	const CONTROLER_STATE& GetPad()
	{
		return App::GetApp()->GetInputDevice().GetControllerVec()[0];
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

		return false;
	}

	bool GetPressA()
	{
		if (GetPadConected())
		{
			return GetPad().wButtons & XINPUT_GAMEPAD_A;
		}

		return false;
	}

	bool GetReleaseA()
	{
		if (GetPadConected())
		{
			return GetPad().wReleasedButtons & XINPUT_GAMEPAD_A;
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