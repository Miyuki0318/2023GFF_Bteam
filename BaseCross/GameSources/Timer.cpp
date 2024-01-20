#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	void Timer::OnUpdate()
	{
		for (auto& timer : m_timers)
		{
			if (timer.objectPtr == NULL)
			{
				timer.Reset();
				continue;
			}
			if (timer.totalTime > timer.limitTime)
			{
				timer.Reset();
				continue;
			}

#if BASECROSS

			const auto& obj = static_cast<GameObject*>(timer.objectPtr);
			if (!obj)
			{
				continue;
			}
			if (!obj->GetUpdateActive())
			{
				continue;
			}
#endif
			// 経過時間をデルタタイムで加算
			timer.totalTime += DELTA_TIME;
		}
	}

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

		if (check)
		{
			return m_timers.at(elem).totalTime;
		}

		return 0.0f;
	}
}