/*!
@file TemplateObject.cpp
@brief オブジェクトの継承元
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	// 生成時の処理
	void TemplateObject::OnCreate()
	{
		// トランスフォームコンポーネントの取得
		m_ptrTrans = GetComponent<Transform>();
	}

	// SEの再生
	void TemplateObject::StartSE(const wstring& seKey, float volume)
	{
		// 所属ステージ上のSEManagerにSE再生関数を送る
		GetTypeStage<BaseStage>()->CreateSE(seKey, volume, ThisPtr);
	}

	// SEの停止
	void TemplateObject::StopSE(const wstring& seKey)
	{
		// 所属ステージ上のSEManagerにSE停止関数を送る
		GetTypeStage<BaseStage>()->StopSE(seKey, ThisPtr);
	}

	// タイマー設定
	bool TemplateObject::SetTimer(float time, bool reset)
	{
		// 所属ステージからタイマーを取得
		const auto& timer = GetTypeStage<BaseStage>()->GetTimer();

		// タイマーを設定
		return timer->SetTimer(ThisPtr, time, reset);
	}

	// タイマーの経過時間取得
	float TemplateObject::GetTime(float time)
	{
		// 所属ステージからタイマーを取得
		const auto& timer = GetTypeStage<BaseStage>()->GetTimer();

		// タイマーの経過時間を取得
		return timer->GetTime(ThisPtr, time);
	}
}