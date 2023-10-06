#include "stdafx.h"
#include "Project.h"

using namespace basecross;

namespace Input
{
	const CONTROLER_STATE& GetPad()
	{
		return App::GetApp()->GetInputDevice().GetControllerVec()[0];
	}

	const CONTROLER_STATE& GetPad(const int index)
	{
		return App::GetApp()->GetInputDevice().GetControllerVec()[index];
	}

	bool GetPadConected()
	{
		return GetPad().bConnected;
	}

	bool GetPadConected(const int index)
	{
		return GetPad(index).bConnected;
	}

	bool GetPushA()
	{
		if (GetPadConected())
		{
			return GetPad().wPressedButtons & XINPUT_GAMEPAD_A;
		}

		return false;
	}

	bool GetPushA(const int index)
	{
		if (GetPadConected(index))
		{
			return GetPad(index).wPressedButtons & XINPUT_GAMEPAD_A;
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

	bool GetPressA(const int index)
	{
		if (GetPadConected(index))
		{
			return GetPad(index).wButtons & XINPUT_GAMEPAD_A;
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

	bool GetReleaseA(const int index)
	{
		if (GetPadConected(index))
		{
			return GetPad(index).wReleasedButtons & XINPUT_GAMEPAD_A;
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

	Vec2 GetLStickValue(const int index)
	{
		if (GetPadConected(index))
		{
			const auto& pad = GetPad(index);
			
			Vec2 vec;
			vec.x = pad.fThumbLX;
			vec.y = pad.fThumbLY;

			return vec;
		}

		return Vec2(0.0f);
	}
}