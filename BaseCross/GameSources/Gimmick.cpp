/*!
@file Gimmick.cpp
@brief ステージギミックの継承元
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	// 生成時の処理
	void Gimmick::OnCreate()
	{
		// 継承元の生成時の関数を実行する
		CubeObject::OnCreate();

		// ローテーションの設定
		SetRotation(Utility::DegToRad(GimmickAngle::GetAngle(m_angle)));

		// タグを設定
		AddTag(L"Gimmick");
	}
}