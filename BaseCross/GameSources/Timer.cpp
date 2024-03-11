/*!
@file Timer.cpp
@brief タイマークラス
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	// 毎フレーム更新処理
	void Timer::OnUpdate()
	{
		// タイマーセット数分ループ
		for (auto& timer : m_timers)
		{
			// オブジェクトポインタが空なら
			if (timer.objectPtr == NULL)
			{
				// リセット
				timer.Reset();
				continue;
			}

			// 経過時間が設定時間より大きかったら
			if (timer.totalTime > timer.limitTime)
			{
				// リセット
				timer.Reset();
				continue;
			}

			// ゲームオブジェクト型にキャスト
			const auto& obj = static_cast<GameObject*>(timer.objectPtr);
			if (!obj)
			{
				// リセット
				timer.Reset();
				continue;
			}

			// 非アクティブなら無視
			if (!obj->GetUpdateActive())
			{
				continue;
			}

			// 経過時間をデルタタイムで加算
			timer.totalTime += DELTA_TIME;
		}
	}

	// タイマーセット
	bool Timer::SetTimer(void* ptr, float time, bool reset)
	{
		// 存在しないポインタかどうかの真偽
		if (ptr == NULL) return false;

		// 設定された時間が0.0f以下だったらそもそもタイマーとして意味がない
		if (time <= 0.0f) return false;

		// チェッカー
		size_t elem = 0;
		bool check = false;

		// 同じオブジェクトから送られたかのチェック
		for (size_t i = 0; i < m_timers.size(); i++)
		{
			// ポインタが一致で設定時間まで同一なら
			if (m_timers.at(i).objectPtr == ptr)
			{
				if (m_timers.at(i).limitTime == time)
				{
					elem = i;
					check = true;
					break;
				}
			}
		}

		// 同一が無いなら新規登録
		if (!check)
		{
			// 配列に空きがあるなら空きに入れる
			for (auto& timer : m_timers)
			{
				if (timer.objectPtr == NULL)
				{
					timer.Set(ptr, time);
					return false;
				}
			}

			// 無ければ追加
			TimerParam nTimer = { ptr, time };
			m_timers.push_back(nTimer);
			return false;
		}

		// 経過時間を無視してSetしたタイミングでリセットするなら
		if (reset)
		{
			// 時間を再設定
			m_timers.at(elem).Set(time);
			return false;
		}
		else
		{
			// 経過時間が設定時間以上になったらリセットしてtrueを返す
			if (m_timers.at(elem).totalTime > m_timers.at(elem).limitTime)
			{
				m_timers.at(elem).Reset();
				return true;
			}
		}

		return false;
	}

	// 経過時間の取得用
	float Timer::GetTime(void* ptr, float time)
	{
		// チェッカー
		size_t elem = 0;
		bool check = false;

		// 同じオブジェクトから送られたかのチェック
		for (size_t i = 0; i < m_timers.size(); i++)
		{
			// ポインタが一致で設定時間まで同一なら
			if (m_timers.at(i).objectPtr == ptr)
			{
				if (m_timers.at(i).limitTime == time)
				{
					elem = i;
					check = true;
					break;
				}
			}
		}

		// チェッカがtrueなら
		if (check)
		{
			// 経過時間を返す
			return m_timers.at(elem).totalTime;
		}

		return 0.0f;
	}
}