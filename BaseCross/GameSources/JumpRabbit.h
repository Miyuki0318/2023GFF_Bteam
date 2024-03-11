/*!
@file JumpRabbit.h
@brief ジャンプするメカウサギ
*/

#pragma once
#include "Rabbit.h"

namespace basecross
{
	/*!
	@brief 敵キャラのジャンプウサギ
	*/
	class JumpRabbit : public Rabbit
	{
		const array<array<Vec3, 6>, 2> m_jumpTargetPos; // ジャンプした着地先

		weak_ptr<Cannon> m_activeCannon;  // 大砲のポインタ
		weak_ptr<Billboard> m_discovered; // 発見時のエフェクト

		Vec3 m_currentTargetPos; // 見失った時の座標
		vector<Vec3> m_aliveBlockPos; // 接地してるブロック

		int m_lostJumpCount; // 見失なった時のジャンプ回数カウンタ

	public:

		/*!
		@brief コンストラクタ
		@param ステージポインタ
		@param ポジション
		@param スケール
		*/
		JumpRabbit(const shared_ptr<Stage>& stagePtr,
			const Vec2& position, float scale
		) :
			Rabbit(stagePtr, position, scale),
			m_jumpTargetPos{{
				{
					Vec3(-2.0f, 0.0f, 0.0f),
					Vec3(-2.5f, 1.5f, 0.0f),
					Vec3(-2.5f, 2.5f, 0.0f),
					Vec3(-1.5f, 2.5f, 0.0f),
					Vec3(-2.5f, -1.5f, 0.0f),
					Vec3(-1.0f, 1.0f, 0.0f)
				},
				{
					Vec3(2.0f, 0.0f, 0.0f),
					Vec3(2.5f, 1.5f, 0.0f),
					Vec3(2.5f, 2.5f, 0.0f),
					Vec3(1.5f, 2.5f, 0.0f),
					Vec3(2.5f, -1.5f, 0.0f),
					Vec3(1.0f, 1.0f, 0.0f)
				}
			}}
		{
			m_lostJumpCount = 0;
			m_currentTargetPos.zero();
		}

		/*!
		@brief デストラクタ
		*/
		virtual ~JumpRabbit() {}

		/*!
		@brief 生成時に一度だけ呼び出される関数
		*/
		void OnCreate() override;

		/*!
		@brief 毎フレーム度に呼び出される関数
		*/
		void OnUpdate() override;

		/*!
		@brief 破棄された時に呼び出される関数
		*/
		void OnDestroy() override;

		/*!
		@brief 衝突した瞬間に呼び出される関数
		*/
		void OnCollisionEnter(const CollisionPair& Pair) override;

		/*!
		@brief 衝突している間呼び出される関数
		*/
		void OnCollisionExcute(const CollisionPair& Pair) override;

		/*!
		@brief 衝突している間呼び出される関数
		@param コリジョンペア(コリジョン情報)
		*/
		void OnCollisionExit(const CollisionPair& Pair) override;

		/*!
		@brief ウサギの移動関数
		*/
		void MoveRabbit() override;

		/*!
		@brief ブロックに衝突した時
		@param ブロックのポインタ
		@param 衝突座標
		*/
		void BlockEnter(const shared_ptr<GameObject>& block, const Vec3& hitPos);

		/*!
		@brief ブロックに衝突し続けた時
		@param ブロックのポインタ
		@param 衝突座標
		*/
		void BlockExcute(const shared_ptr<GameObject>& block, const Vec3& hitPos);

		/*!
		@brief ベルトコンベアに衝突した時
		@param ベルトコンベアのポインタ
		@param 衝突座標
		*/
		void ConvayorEnter(const shared_ptr<GameObject>& convayor, const Vec3& hitPos);

		/*!
		@brief ベルトコンベアに衝突し続けた時
		@param ベルトコンベアのポインタ
		@param 衝突座標
		*/
		void ConvayorExcute(const shared_ptr<GameObject>& convayor, const Vec3& hitPos);

		/*!
		@brief バンパーに衝突した時
		@param バンパーのポインタ
		@param 衝突座標
		*/
		void BumperEnter(const shared_ptr<GameObject>& bumper, const Vec3& hitPos);

		/*!
		@brief 大砲に衝突した時
		@param 大砲のポインタ
		*/
		void CannonEnter(const shared_ptr<GameObject>& cannon);

		/*!
		@brief 死亡時のステート関数
		*/
		void DeathState();

		/*!
		@brief パトロールステート関数
		*/
		void PatrolState();

		/*!
		@brief 追跡時のステート関数
		*/
		void SeekState();

		/*!
		@brief 見失なった時のステート関数
		*/
		void LostState();

		/*!
		@brief 大砲待機時のステート関数
		*/
		void CannonState();

		/*!
		@brief プレイヤーサーチ関数
		*/
		bool SearchPlayer();

		/*!
		@brief ジャンプ関数
		*/
		void JumpingRabbit();

		/*!
		@brief ジャンプ時のSE関数
		*/
		void StartJumpSE();

		/*!
		@brief プレイヤーに向けてジャンプする関数
		@param ターゲット座標
		*/
		void PlayerTargetJump(const Vec3& targetPos);

		/*!
		@brief ブロックに向けてジャンプする関数
		@param ターゲット座標
		*/
		void BlockTargetJump(const Vec3& targetPos);

		/*!
		@brief ターゲットまでの線分と衝突するブロックを返す関数
		@param ターゲット座標
		@return 衝突したブロック配列
		*/
		const vector<Vec3> GetHitBlockPos(const Vec3& targetPos);

		/*!
		@brief 座標配列の中から最接近を返す関数
		@param 座標配列
		@return 最接近の座標
		*/
		const Vec3 GetNearPosition(const vector<Vec3>& posVec) const
		{
			Vec3 temp; // 保存用変数
			Vec3 pos = GetPosition(); // 自身の座標

			// 座標配列が空じゃなければ
			if (!posVec.empty())
			{
				temp = posVec.at(0); // 配列の一番最初の座標を代入

				// 座標の数ループ
				for (const auto& alive : posVec)
				{
					// 長さを比較
					float lengthA = (temp - pos).length();
					float lengthB = (alive - pos).length();

					// 距離が短ければ
					if (lengthA > lengthB)
					{
						temp = alive; // 保存
					}
				}
			}

			return temp;
		}

		/*!
		@brief 移動設定関数
		@param 移動量
		@param 加速度
		*/
		void SetMoveValue(const Vec2& velocity, float acsel) override
		{
			// 継承元の移動設定関数を実行
			Rabbit::SetMoveValue(velocity, acsel);

			// ジャンプ時のSEを再生
			StartJumpSE();
		}
	};
}