/*!
@file WallRabbit.h
@brief 壁になってるメカウサギ
*/

#pragma once
#include "Rabbit.h"

namespace basecross
{
	/*!
	@brief 敵キャラの壁ウサギ
	*/
	class WallRabbit : public Rabbit
	{
	public:

		/*!
		@brief コンストラクタ
		@param ステージポインタ
		@param ポジション
		@param スケール
		*/
		WallRabbit(const shared_ptr<Stage>& stagePtr,
			const Vec2& position, float scale
		) :
			Rabbit(stagePtr, position, scale)
		{
		}

		/*!
		@brief デストラクタ
		*/
		virtual ~WallRabbit() {}

		/*!
		@brief 毎フレーム度に呼び出される関数
		*/
		void OnUpdate() override;

		/*!
		@brief 死亡時のステート関数
		*/
		void DeathState();

		/*!
		@brief コリジョンのパフォーマンス管理関数
		*/
		void CollisiontPerformance();
	};
}