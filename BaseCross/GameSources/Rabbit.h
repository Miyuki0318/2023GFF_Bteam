/*!
@file Rabbit.h
@brief メカウサギ
*/

#pragma once
#include "TemplateObject.h"

namespace basecross
{
	/*!
	@brief 敵キャラのメカウサギ継承元
	*/
	class Rabbit : public TemplateObject
	{
	public:

		// ウサギの行動ステートenum
		enum class eState : int8_t
		{
			Patrol,		// パトロール
			Seek,		// 追跡
			LostSight,	// 見失う
			CannonJump,	// 大砲ジャンプ
			Death,		// 死亡
		};

		enum class eStatus : uint8_t
		{
			IsAir,		// 空中か
			IsDeath,	// 死んだか
			IsCannon,	// 大砲待機時か
		};

	protected:

		const Vec2 m_jumpVelo; // ジャンプ時の移動量
			
		eState m_state;    // ステート
		Mat4x4 m_modelMat; // モデルとトランスフォームの差分行列
		Vec2 m_velocity;   // 移動量

		weak_ptr<TemplateObject> m_targetObj;	// ターゲットポインタ
		shared_ptr<PNTBoneModelDraw> m_ptrDraw; // 描画コンポーネント
		shared_ptr<CollisionObb> m_ptrColl;		// コリジョンOBBコンポーネント

		const float m_speed;	// 速度
		const float m_maxAcsel;	// 最大加速度
		const float m_range;	// 射程

		float m_acsel;   // 加速度
		float m_dir;	 // 向き
		float m_gravity; // 重力

		Bool8_t<eStatus> m_status; // 状態ステータス

	public:

		/*!
		@brief コンストラクタ
		@param ステージポインタ
		@param ポジション
		@param スケール
		*/
		Rabbit(const shared_ptr<Stage>& stagePtr,
			const Vec2& position, float scale
		) :
			TemplateObject(stagePtr, Vec3(position.x, position.y, 0.0f), Vec3(0.0f, XM_PI, 0.0f), Vec3(scale)),
			m_speed(3.0f),
			m_maxAcsel(5.0f),
			m_range(15.0f),
			m_jumpVelo(1.0f, -2.0f)
		{
			m_velocity = Vec2(-1.0f, 0.0f);
			m_acsel = 1.0f;
			m_dir = -1.0f;
			m_gravity = -5.0f;
			m_state = eState::Patrol;
			m_status.Set(eStatus::IsAir) = true;
			m_modelMat.affineTransformation(
				Vec3(1.35f),
				Vec3(0.0f),
				Vec3(0.0f, 0.0f, 0.0f),
				Vec3(0.0f, -0.5f, 0.0f)
			);
		}

		/*!
		@brief デストラクタ
		*/
		virtual ~Rabbit() {}

		/*!
		@brief 生成時に一度だけ呼び出される関数
		*/
		void OnCreate() override;

		/*!
		@brief ウサギの移動関数
		*/
		virtual void MoveRabbit();

		/*!
		@brief ウサギの移動量減少関数
		*/
		void MoveReduction();

		/*!
		@brief ステート取得関数
		@return eState
		*/
		const eState& GetState() const
		{
			return m_state;
		}

		/*!
		@brief ステート設定関数
		@param eState
		*/
		void SetState(const eState& state)
		{
			m_state = state;
		}

		/*!
		@brief 死亡したかの真偽取得関数
		@return m_status(eStatus::IsDeath)
		*/
		bool GetIsDeath() const
		{
			return m_status(eStatus::IsDeath);
		}

		/*!
		@brief 移動設定関数
		@param 移動量
		@param 加速度
		*/
		virtual void SetMoveValue(const Vec2& velocity, float acsel)
		{
			m_velocity = velocity;
			m_acsel = acsel < 1.0f ? 1.0f : acsel;
		}
	};
}