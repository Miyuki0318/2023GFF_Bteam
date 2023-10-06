#pragma once
#include "stdafx.h"

using namespace basecross;

namespace Input
{
	const CONTROLER_STATE& GetPad();
	const CONTROLER_STATE& GetPad(const int index);
	
	bool GetPadConected();
	bool GetPadConected(const int index);

	bool GetPushA();
	bool GetPushA(const int index);

	bool GetPressA();
	bool GetPressA(const int index);

	bool GetReleaseA();
	bool GetReleaseA(const int index);

	Vec2 GetLStickValue();
	Vec2 GetLStickValue(const int index);
}