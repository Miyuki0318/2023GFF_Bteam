#pragma once
#include "stdafx.h"

using namespace basecross;

namespace Input
{
	const CONTROLER_STATE& GetPad();

	bool GetPadConected();

	bool GetPushA();

	bool GetPressA();

	bool GetReleaseA();

	Vec2 GetLStickValue();

	bool IsInputLStick();

	bool IsInputLStickX();

	bool IsInputLStickY();
}