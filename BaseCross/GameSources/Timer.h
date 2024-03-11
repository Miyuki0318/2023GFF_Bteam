/*!
@file Timer.h
@brief タイマークラス
*/

#pragma once
#include "stdafx.h"

namespace basecross
{
	/*!
	@brief タイマー
	*/
	class Timer : public GameObject
	{
		// タイマー管理構造体
		struct TimerParam
		{
			void* objectPtr = NULL; // オブジェクトのポインタ
			float totalTime = 0.0f; // 経過時間
			float limitTime = 0.0f; // 設定時間

			/*!
			@brief コンストラクタ
			*/
			TimerParam()
			{
				objectPtr = NULL;
				totalTime = 0.0f;
				limitTime = 0.0f;
			}

			/*!
			@brief コンストラクタ
			@param 呼び出し元ポインタ
			@param 設定時間
			*/
			TimerParam(void* ptr, float limit) :
				objectPtr(ptr),
				limitTime(limit)
			{
				totalTime = 0.0f;
			}

		public:

			/*!
			@brief リセット関数
			*/
			void Reset()
			{
				objectPtr = NULL;
				totalTime = 0.0f;
				limitTime = 0.0f;
			}

			/*!
			@brief セット関数
			@param 設定時間
			*/
			void Set(float limit)
			{
				totalTime = 0.0f;
				limitTime = limit;
			}

			/*!
			@brief セット関数
			@param オブジェクトポインタ
			@param 設定時間
			*/
			void Set(void* ptr, float limit)
			{
				objectPtr = ptr;
				totalTime = 0.0f;
				limitTime = limit;
			}
		};

		vector<TimerParam> m_timers; // 管理配列

	public:

		/*!
		@brief コンストラクタ
		@param ステージポインタ
		*/
		Timer(const shared_ptr<Stage>& stagePtr) :
			GameObject(stagePtr)
		{
			m_timers = {};
		}

		/*!
		@brief デストラクタ
		*/
		virtual ~Timer() {}

		/*!
		@brief 毎フレーム度に呼び出される関数
		*/
		void OnUpdate() override;

		/*!
		@brief タイマーセット関数
		@param オブジェクトポインタ
		@param 設定時間
		@param 上書きセットするか
		@return タイマーが終了したかの真偽
		*/
		bool SetTimer(void* ptr, float time, bool reset = false);

		/*!
		@brief タイマー経過時間取得関数
		@param オブジェクトポインタ
		@param 設定時間
		@return 経過時間
		*/
		float GetTime(void* ptr, float time);
	};
}