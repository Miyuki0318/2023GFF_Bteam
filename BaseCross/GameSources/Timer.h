#pragma once
#include "stdafx.h"

namespace basecross
{
	class Timer : public GameObject
	{
		struct TimerParam
		{
			void* objectPtr = NULL;
			float totalTime = 0.0f;
			float limitTime = 0.0f;

			TimerParam()
			{
				objectPtr = NULL;
				totalTime = 0.0f;
				limitTime = 0.0f;
			}

			TimerParam(void* ptr, float limit) :
				objectPtr(ptr),
				limitTime(limit)
			{
				totalTime = 0.0f;
			}

		public:

			void Reset()
			{
				objectPtr = NULL;
				totalTime = 0.0f;
				limitTime = 0.0f;
			}

			void Set(float limit)
			{
				totalTime = 0.0f;
				limitTime = limit;
			}

			void Set(void* ptr, float limit)
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

		bool SetTimer(void*, float time, bool reset = false);

		float GetTime(void* ptr, float time);
	};
}