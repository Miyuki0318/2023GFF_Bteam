#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	void Timer::OnUpdate()
	{
		for (auto& timer : m_timers)
		{
			if (!timer.objectPtr.lock())
			{
				timer.Reset();
				continue;
			}

			timer.totalTime += DELTA_TIME;
		}
	}

	bool Timer::SetTimer(const weak_ptr<ObjectInterface>& ptr, float time, bool reset)
	{
		// 存在しないポインタかどうかの真偽
		if (!ptr.lock()) return false;

		// 設定された時間が0.0f以下だったらそもそもタイマーとして意味がない
		if (time <= 0.0f) return false;

		// チェッカー
		size_t elem = 0;
		bool check = false;

		// 同じオブジェクトから送られたかのチェック
		for (size_t i = 0; i < m_timers.size(); i++)
		{
			// ポインタが一致なら
			if (m_timers.at(i).objectPtr.lock() == ptr.lock())
			{
				elem = i;
				check = true;
				break;
			}
		}

		// 同一オブジェクトが無いなら新規登録
		if (!check)
		{
			for (auto& timer : m_timers)
			{
				if (!timer.objectPtr.lock())
				{
					timer.Set(ptr, time);
					return false;
				}
			}

			TimerParam nTimer = { ptr, time };
			m_timers.push_back(nTimer);
			return false;
		}

		// リセットするなら
		if (reset)
		{
			// 時間を設定
			m_timers.at(elem).Set(time);
			return false;
		}
		else
		{
			// 経過時間が設定時間以上になったらリセットしてtrueを返す
			if (m_timers.at(elem).totalTime >= m_timers.at(elem).limitTime)
			{
				m_timers.at(elem).Reset();
				return true;
			}
		}

		return false;
	}
}