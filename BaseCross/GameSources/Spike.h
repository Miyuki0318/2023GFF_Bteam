/*!
@file Spike.h
@brief ステージギミックの棘
*/

#pragma once
#include "Gimmick.h"

namespace basecross 
{
	/*!
	@brief 棘
	*/
	class Spike : public Gimmick
	{
		// 描画コンポーネント
		shared_ptr<PNTBoneModelDraw> m_ptrDraw; 

		// モデルとトランスフォームの差分行列
		Mat4x4 m_blockMat;

	public:

		/*!
		@brief コンストラクタ
		@param ステージポインタ
		@param ポジション
		@param スケール
		@param 角度
		*/
		Spike(const shared_ptr<Stage>& stagePtr,
			const Vec2& position, const float scale, GimmickAngle::eAngle angle
		) :
			Gimmick(stagePtr, Vec3(position), Vec3(scale), angle)
		{
			m_modelMat.affineTransformation(
				Vec3(1.35f),
				Vec3(0.0f),
				Vec3(0.0f, XM_PIDIV2, 0.0f),
				Vec3(0.0f, -0.65f, 0.0f)
			);
			m_blockMat.affineTransformation(
				Vec3(1.35f),
				Vec3(0.0f),
				Vec3(0.0f, 0.0f, 0.0f),
				Vec3(0.0f, -0.65f, 0.0f)
			);
		}

		/*!
		@brief デストラクタ
		*/
		virtual ~Spike() {}

		/*!
		@brief 生成時に一度だけ呼び出される関数
		*/
		virtual void OnCreate() override;

		/*!
		@brief 毎フレーム度に呼び出される関数
		*/
		virtual void OnUpdate() override;
	};

	/*!
	@brief 動く棘
	*/
	class MoveSpike : public Spike
	{
	public:

		// 動く方向タイプenum
		enum class eMoveType : int8_t
		{
			UpDown,		// 上下
			LeftRight,	// 左右
		};

		// 動くステートenum
		enum class eMoveState : int8_t
		{
			StandBy,	// 待機時
			StartPos,	// 開始時の座標
			MoveA,		// ポイントAに移動
			MoveB,		// ポイントBに移動
		};

	private:

		eMoveType m_moveType;	   // 動くタイプ
		eMoveState m_moveState;	   // ステート
		eMoveState m_currentState; // 前回のステート

		const Vec3 m_startPos; // 開始時の座標
		Vec3 m_movePointA;	   // ポイントA
		Vec3 m_movePointB;	   // ポイントB

		const float m_moveSpeed;  // 移動速度
		const float m_moveLength; // 移動距離

	public:

		/*!
		@brief コンストラクタ
		@param ステージポインタ
		@param ポジション
		@param スケール
		@param 挙動タイプ
		@param 移動速度
		@param 移動距離
		*/
		MoveSpike(const shared_ptr<Stage>& stagePtr,
			const Vec2& position, const float scale,
			eMoveType type, const float speed, const float length
		) :
			Spike(stagePtr, position, scale, GimmickAngle::eAngle::All),
			m_moveType(type),
			m_moveSpeed(speed),
			m_moveLength(length),
			m_startPos(position)
		{
			m_moveState = eMoveState::StartPos;
			m_currentState = eMoveState::StandBy;
			m_movePointA = position;
			m_movePointB = position;
		}

		/*!
		@brief デストラクタ
		*/
		virtual ~MoveSpike() {}

		/*!
		@brief 生成時に一度だけ呼び出される関数
		*/
		void OnCreate() override;

		/*!
		@brief 毎フレーム度に呼び出される関数
		*/
		void OnUpdate() override;

		/*!
		@brief ステートの更新処理関数
		*/
		void UpdateState();

		/*!
		@brief 待機時のステート関数
		*/
		void StandbyState();

		/*!
		@brief ポイントに移動時のステート関数
		@param 開始座標
		@param 終了座標
		@param 時間
		@return 移動終了したかの真偽
		*/
		bool PointState(const Vec3& start, const Vec3& end, float time);
	};
}