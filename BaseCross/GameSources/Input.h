/*!
@file Input.h
@brief コントローラー入力取得
*/

#pragma once
#include "stdafx.h"

namespace basecross
{
	/*!
	@brief コントローラー入力取得関数群
	*/
	namespace Input
	{
		/*!
		@brief コントローラーステート取得関数
		@return CONTROLER_STATE
		*/
		inline const CONTROLER_STATE& GetPad()
		{
			// アプリケーションオブジェクトから入力デバイスを取得して返す
			return App::GetApp()->GetInputDevice().GetControllerVec()[0];
		}

		/*!
		@brief コントローラーが接続されているかの取得関数
		@return GetPad().bConnected
		*/
		inline bool GetPadConected()
		{
			return GetPad().bConnected;
		}

		/*!
		@brief Aボタンを押した瞬間の取得関数
		@return GetPad().wPressedButtons & XINPUT_GAMEPAD_A
		*/
		inline bool GetPushA()
		{
			// コントローラーが接続されてるなら
			if (GetPadConected())
			{
				// Aボタンが入力された瞬間かを返す
				return GetPad().wPressedButtons & XINPUT_GAMEPAD_A;
			}
			return false;
		}

		/*!
		@brief Aボタンを押されているかの取得関数
		@return GetPad().wButtons & XINPUT_GAMEPAD_A
		*/
		inline bool GetPressA()
		{
			// コントローラーが接続されてるなら
			if (GetPadConected())
			{
				// Aボタンが入力されているかを返す
				return GetPad().wButtons & XINPUT_GAMEPAD_A;
			}
			return false;
		}

		/*!
		@brief Aボタンを離した瞬間かの取得関数
		@return GetPad().wReleasedButtons & XINPUT_GAMEPAD_A
		*/
		inline bool GetReleaseA()
		{
			// コントローラーが接続されているなら
			if (GetPadConected())
			{
				// Aボタンの入力が解除された瞬間かを返す
				return GetPad().wReleasedButtons & XINPUT_GAMEPAD_A;
			}
			return false;
		}

		/*!
		@brief Lスティックの入力量取得関数
		@return Vec2(pad.fThumbLX, pad.fThumbLY)
		*/
		inline Vec2 GetLStickValue()
		{
			// コントローラーが接続されているなら
			if (GetPadConected())
			{
				// コントローラーを取得
				const auto& pad = GetPad();

				// コントローラーのLスティック入力量をVec2にして返す
				return Vec2(pad.fThumbLX, pad.fThumbLY);
			}
			return Vec2(0.0f);
		}

		/*!
		@brief Lスティックの入力されているかの真偽取得関数
		@return GetLStickValue().length() > 0.0f
		*/
		inline bool IsInputLStick()
		{
			// Lスティックの入力量の長さが0より大きいかの真偽返す
			return GetLStickValue().length() > 0.0f;
		}

		/*!
		@brief Lスティックの入力されているかの真偽をX軸のみで取得する関数
		@return stick.length() > 0.0f
		*/
		inline bool IsInputLStickX()
		{
			// Lスティックの入力量を取得
			Vec2 stick = GetLStickValue();
			stick.y = 0.0f; // Y軸を0に
			return stick.length() > 0.0f; // X軸のみで長さを比較
		}

		/*!
		@brief Lスティックの入力されているかの真偽をL軸のみで取得する関数
		@return stick.length() > 0.0f
		*/
		inline bool IsInputLStickY()
		{
			// Lスティックの入力量を取得
			Vec2 stick = GetLStickValue();
			stick.x = 0.0f; // X軸を0に
			return stick.length() > 0.0f; // Y軸のみでで長さを比較
		}
	}
}