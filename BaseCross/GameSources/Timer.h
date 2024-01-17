#pragma once
#include "stdafx.h"

namespace basecross
{
	class Timer : public GameObject
	{
		struct TimerParam
		{
			weak_ptr<ObjectInterface> objectPtr;
			float totalTime = 0.0f;
			float limitTime = 0.0f;

			TimerParam()
			{
				totalTime = 0.0f;
				limitTime = 0.0f;
			}

			TimerParam(const weak_ptr<ObjectInterface>& ptr, float limit) :
				objectPtr(ptr),
				limitTime(limit)
			{
				totalTime = 0.0f;
			}

		public:

			void Reset()
			{
				objectPtr.reset();
				totalTime = 0.0f;
				limitTime = 0.0f;
			}

			void Set(float limit)
			{
				totalTime = 0.0f;
				limitTime = limit;
			}

			void Set(const weak_ptr<ObjectInterface>& ptr, float limit)
			{
				objectPtr = ptr;
				totalTime = 0.0f;
				limitTime = limit;
			}
		};

		vector<TimerParam> m_timers;

	public:

		Timer(const shared_ptr<Stage>& stagePtr) :
			GameObject(stagePtr)
		{
			m_timers = {};
		}

		virtual ~Timer() {}

		void OnUpdate() override;

		bool SetTimer(const weak_ptr<ObjectInterface>& ptr, float time, bool reset = false);
	};
}