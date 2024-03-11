/*!
@file Player.h
@brief プレイヤー本体
*/

#pragma once
#include "TemplateObject.h"
#include "ArrowEffect.h"
#include "AirJetEffect.h"
#include "Cannon.h"

namespace basecross
{
	/*!
	@brief プレイヤーオブジェクト本体
	*/
	class Player : public TemplateObject
	{
	protected:

		// ステータス管理用enum
		enum class eStatus : uint8_t
		{
			IsAir,				// 空中かどうか
			IsBlower,			// 送風機に当たっているか
			IsHighJump,			// 高く跳んでいるか
			IsInvincible,		// 無敵中か
			IsAliveMoveBlock,	// 動く壁の上に居るか
			IsHitMoveBlock,		// 動く壁に衝突しているか
			IsFirePossible,		// エアショック使用可能か
			IsCannonFire,		// 大砲発射後か
			IsCannonStandby		// 大砲待機中か
		};

		shared_ptr<PNTBoneModelDraw> m_bodyDraw; // 胴の描画コンポーネント
		shared_ptr<PNTBoneModelDraw> m_armDraw;  // 腕の描画コンポーネント
		shared_ptr<CollisionSphere> m_ptrColl;   // スフィアコリジョンコンポーネント

		const Vec2 m_deffVelo;	// 未入力時のデフォルトの移動量
		const Vec3 m_startPos;  // 開始時の座標

		Vec2 m_velocity;	// 移動量
		Vec2 m_meddleVelo;	// 加算移動量
		Mat4x4 m_bodyMat;	// 胴のモデルとトランスフォームの差分行列
		Mat4x4 m_armMat;	// 腕のモデルとトランスフォームの差分行列

		weak_ptr<TemplateObject> m_arm;			// 腕モデル用オブジェクト
		weak_ptr<AirJetEffect> m_airEffect;		// エアショックエフェクト
		weak_ptr<ShieldEffect> m_shieldEffect;	// シールドエフェクト
		weak_ptr<Cannon> m_activeCannon;		// 大砲ポインタ一時保持用
		weak_ptr<MultiParticle> m_particle;		// パーティクル
		weak_ptr<ArrowEffect> m_arrow;			// 軌道描画
		weak_ptr<TemplateObject> m_currentWall; // 前回衝突した動く壁

		const int m_jumpLimit;			  // 規定ジャンプ回数
		const int m_shieldLimit;		  // 規定シールド枚数
		const float m_speed;			  // 速度
		const float m_gravity;			  // 重力
		const float m_maxAcsel;			  // 最大加速度
		const float m_veloSpeed;		  // 移動量乗算速度
		const float m_damageAcsel;		  // ダメージ時加速度
		const float m_timeSpeed;		  // 時間乗算速度
		const float m_invincibleTime;	  // 規定無敵時間
		const float m_jumpRecoveryLimit;  // 規定ジャンプ回復時間
		const array<int, 4> m_sRingLimit; // スモールリング規定個数

		int m_jumpCount;			// ジャンプ回数
		int m_sRingCount;			// スモールリング回収個数
		int m_shieldCount;			// シールド枚数
		float m_acsel;				// 加速度
		float m_jumpRecoveryTime;	// ジャンプ時の回復までの経過時間
		
		Bool16_t<eStatus> m_status; // 状態フラグ

	public:

		/*!
		@brief コンストラクタ
		@param ステージポインタ
		@param ポジション
		*/
		Player(const shared_ptr<Stage>& stagePtr,
			const Vec3& position
		) :
			TemplateObject(stagePtr),
			m_startPos(position),
			m_jumpLimit(2),
			m_shieldLimit(3),
			m_speed(4.0f),
			m_gravity(-5.0f),
			m_maxAcsel(4.5f),
			m_veloSpeed(2.5f),
			m_damageAcsel(2.5f),
			m_timeSpeed(1.5f),
			m_invincibleTime(1.0f),
			m_jumpRecoveryLimit(0.5f),
			m_deffVelo(0.0f, -1.0f),
			m_sRingLimit({ 5, 20, 25, 0	})
		{
			m_position = m_startPos;
			m_rotation.zero();
			m_scale = Vec3(1.0f);
			m_velocity = Vec2(-5.0f, 0.0f);
			m_meddleVelo.zero();
			m_jumpCount = 0;
			m_sRingCount = 0;
			m_shieldCount = 1;
			m_acsel = 7.5f;
			m_jumpRecoveryTime = 0.0f;
			m_status.Set(eStatus::IsAir, eStatus::IsFirePossible) = true;

			m_bodyMat.affineTransformation(
				Vec3(1.25f),
				Vec3(0.0f),
				Vec3(0.0f, -XM_PIDIV2, 0.0f),
				Vec3(0.0f, -0.65f, 0.0f)
			);

			m_armMat.affineTransformation(
				Vec3(1.25f),
				Vec3(0.0f),
				Vec3(0.0f, -XM_PIDIV2, 0.0f),
				Vec3(0.0f, -1.35f, 0.0f)
			);
		}

		/*!
		@brief デストラクタ
		*/
		virtual ~Player() {}

		/*!
		@brief リセット関数
		*/
		virtual void Reset();

		/*!
		@brief 生成時に一度だけ呼び出される関数
		*/
		virtual void OnCreate() override;

		/*!
		@brief 毎フレーム度に呼び出される関数
		*/
		virtual void OnUpdate() override;

		/*!
		@brief Aボタンが押されなくなった時に呼び出される関数
		*/
		virtual void OnRushA();

		/*!
		@brief 衝突した瞬間に呼び出される関数
		@param コリジョンペア(コリジョン情報)
		*/
		void OnCollisionEnter(const CollisionPair& Pair) override;

		/*!
		@brief 衝突している間呼び出される関数
		@param コリジョンペア(コリジョン情報)
		*/
		void OnCollisionExcute(const CollisionPair& Pair) override;

		/*!
		@brief 衝突している間呼び出される関数
		@param コリジョンペア(コリジョン情報)
		*/
		void OnCollisionExit(const CollisionPair& Pair) override;

	protected:

		/*!
		@brief プレイヤーの移動関数
		*/
		void MovePlayer();

		/*!
		@brief プレイヤーの移動量減少関数
		*/
		void MoveReduction();

		/*!
		@brief エアショックの回復関数
		*/
		void RecoveryAirShock();

		/*!
		@brief プレイヤーの回転関数
		*/
		virtual void RotatePlayer();

		/*!
		@brief エアショックの軌道描画関数
		*/
		virtual void UpdateArrow();

		/*!
		@brief アニメーション更新関数
		*/
		void AnimationUpdate();

		/*!
		@brief エフェクト描画関数
		*/
		void EffectUpdate();

		/*!
		@brief 大砲待機関数
		@param 加速度
		*/
		virtual void CannonStandby(float acsel);

		/*!
		@brief 大砲発射後関数
		*/
		virtual void AftterCannon();

		/*!
		@brief 死亡時の設定関数
		*/
		virtual void DeathSetup();

		/*!
		@brief 死亡時の落下関数
		*/
		virtual void DeathDrop();

		/*!
		@brief 無敵時間経過
		*/
		void InvincibleTimer();

		/*!
		@brief 送風機の範囲に入っているかのチェック関数
		*/
		void BlowerBetweenCheck();

		/*!
		@brief 上向きの送風機に当たっている時
		*/
		void UpMeddleVelocity();

		/*!
		@brief 下向きの送風機に当たっている時
		*/
		void DownMeddleVelocity();

		/*!
		@brief 左向きの送風機に当たっている時
		*/
		void LeftMeddleVelocity();

		/*!
		@brief 右向きの送風機に当たっている時
		*/
		void RightMeddleVelocity();

		/*!
		@brief 加算移動量の減少関数
		*/
		void ReductionMeddleVelocity();

		/*!
		@brief ブロックに衝突した時
		@param ブロックのポインタ
		@param 衝突座標
		*/
		void BlockEnter(const shared_ptr<GameObject>& block, const Vec3& hitPos);

		/*!
		@brief ブロックに上から衝突した時
		@param ブロックの座標
		@param ブロックの半径
		*/
		void BlockUpperHit(const Vec3& objPos, const Vec3& helf);

		/*!
		@brief ブロックに下から衝突した時
		@param ブロックの座標
		@param ブロックの半径
		*/
		void BlockUnderHit(const Vec3& objPos, const Vec3& helf);

		/*!
		@brief ブロックに左から衝突した時
		@param ブロックの座標
		@param ブロックの半径
		*/
		void BlockLeftHit(const Vec3& objPos, const Vec3& helf);

		/*!
		@brief ブロックに右から衝突した時
		@param ブロックの座標
		@param ブロックの半径
		*/
		void BlockRightHit(const Vec3& objPos, const Vec3& helf);

		/*!
		@brief ブロックバウンドした時のSE
		*/
		void BlockBoundSE();

		/*!
		@brief ブロックに衝突し続けた時
		@param ブロックのポインタ
		@param 衝突座標
		*/
		void BlockExcute(const shared_ptr<GameObject>& block, const Vec3& hitPos);

		/*!
		@brief 棘に衝突した時
		@param 棘のポインタ
		@param 衝突座標
		*/
		void SpikeEnter(const shared_ptr<GameObject>& spike, const Vec3& hitPos);

		/*!
		@brief 棘に衝突し続けた時
		@param 棘のポインタ
		@param 衝突座標
		*/
		void SpikeExcute(const shared_ptr<GameObject>& spike, const Vec3& hitPos);

		/*!
		@brief 大砲に衝突した時
		@param 大砲のポインタ
		*/
		void CannonEnter(const shared_ptr<GameObject>& cannon);

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
		@brief 敵のウサギに衝突した時
		@param ウサギのポインタ
		*/
		void RabbitEnter(const shared_ptr<GameObject>& rabbit, const Vec3& hitPos);

		/*!
		@brief 敵のウサギに衝突し続けた時
		@param ウサギのポインタ
		*/
		void RabbitExcute(const shared_ptr<GameObject>& rabbit, const Vec3& hitPos);

		/*!
		@brief リングに衝突した時
		@param リングのポインタ
		*/
		void RingEnter(const shared_ptr<GameObject>& ring);

		/*!
		@brief 動く壁に衝突した時
		@param 動く壁のポインタ
		@param 衝突座標
		*/
		void MoveWallEnter(const shared_ptr<GameObject>& wall, const Vec3& hitPos);

		/*!
		@brief 動く壁から離れたか
		*/
		void MoveWallExit();

		/*!
		@brief ダメージノックバック処理
		@param 移動量
		*/
		void DamageKnockBack(const Vec2& velocity);

		/*!
		@brief 動く壁の連結取得関数
		@param 動く壁グループ配列
		@param 開始座標
		@param 方向加算ベクトル
		@return 連番最後の座標
		*/
		Vec3 GetMoveWallPos(const vector<weak_ptr<GameObject>>& groupVec, const Vec3& pos, const Vec3& addVec)
		{
			Vec3 position = pos;

			while (ObjectCheck(groupVec, position))
			{
				position += addVec;
			}

			return position;
		}

		/*!
		@brief 動く壁グループの取得関数
		@param gimmickグループ配列
		@return 動く壁グループ配列
		*/
		const vector<weak_ptr<GameObject>> GetMoveWallGroup(const vector<weak_ptr<GameObject>>& groupVec)
		{
			vector<weak_ptr<GameObject>> moveWallVec;
			for (const auto& gimmick : groupVec)
			{
				if (!gimmick.lock()) continue;

				const auto& moveWall = dynamic_pointer_cast<MoveWall>(gimmick.lock());
				if (!moveWall) continue;

				moveWallVec.push_back(moveWall);
			}

			return moveWallVec;
		}

		/*!
		@brief シールド追加処理
		*/
		void AddShield()
		{
			// SEの再生
			StartSE(L"SHIELD_C_SE", 0.75f);

			// シールドを増やす
			m_shieldCount++;
			m_shieldCount = min(m_shieldLimit, m_shieldCount);
		}

		/*!
		@brief 小リング取得処理
		*/
		void GetSmallRing()
		{
			m_sRingCount++; // 小リングカウンタを増やす

			// ジャンプ回数を減らす
			m_jumpCount > 0 ? m_jumpCount-- : m_jumpCount = 0;
		}


		/*!
		@brief 上下で押し潰された時の関数
		*/
		void UnderCompressedDeath()
		{
			if (m_status(eStatus::IsAliveMoveBlock))
			{
				// 死亡時の設定をする
				m_shieldCount = 0;
				StartSE(L"SHIELD_D_SE", 1.5f);
				DeathSetup();
			}
		}

		/*!
		@brief 左右で押し潰された時の関数
		*/
		void LeftRightCompressedDeath()
		{
			if (m_status(eStatus::IsHitMoveBlock))
			{
				// 死亡時の設定をする
				m_shieldCount = 0;
				StartSE(L"SHIELD_D_SE", 1.5f);
				DeathSetup();
			}
		}

	public:

		/*!
		@brief シールドの枚数取得関数
		@return m_shieldCount
		*/
		const int& GetShieldCount() const
		{
			return m_shieldCount;
		}

		/*!
		@brief シールドの上限数取得関数
		@return m_shieldLimit
		*/
		const int& GetShieldLimit() const
		{
			return m_shieldLimit;
		}

		/*!
		@brief スモールリング獲得数取得関数
		@return m_sRingCount
		*/
		const int& GetSRingCount() const
		{
			return m_sRingCount;
		}

		/*!
		@brief スモールリング獲得上限数取得関数
		@return m_sRingLimit
		*/
		const array<int, 4>& GetSRingLimit() const
		{
			return m_sRingLimit;
		}

		/*!
		@brief ジャンプ回数取得関数
		@return m_jumpCount
		*/
		const int& GetJumpCount() const
		{
			return m_jumpCount;
		}

		/*!
		@brief ジャンプ上限値取得関数
		@return m_jumpLimit
		*/
		const int& GetJumpLimit() const
		{
			return m_jumpLimit;
		}

		/*!
		@brief ジャンプ経過時間取得関数
		@return m_jumpRecoveryTime
		*/
		const float& GetJumpRecoveryTime() const
		{
			return m_jumpRecoveryTime;
		}

		/*!
		@brief ジャンプ回復時間取得関数
		@return m_jumpRecoveryLimit
		*/
		const float& GetJumpRecoveryLimit() const
		{
			return m_jumpRecoveryLimit;
		}

		/*!
		@brief 加速度の取得関数
		@return m_acsel
		*/
		const float& GetAcsel() const
		{
			return m_acsel;
		}

		/*!
		@brief 移動量の取得関数
		@return m_velocity
		*/
		const Vec2& GetVelocity() const
		{
			return m_velocity;
		}
	};
}