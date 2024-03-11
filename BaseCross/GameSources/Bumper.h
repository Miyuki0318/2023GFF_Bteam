/*!
@file Bumper.h
@brief ステージギミックのバンパー
*/

#pragma once
#include "TemplateObject.h"

namespace basecross
{
	/*!
	@brief バンパー
	*/
	class Bumper : public TemplateObject
	{
		vector<weak_ptr<GameObject>> m_targetObj; // ターゲット配列
		shared_ptr<CollisionSphere> m_ptrColl;	  // スフィアコリジョンコンポーネント
		shared_ptr<PNTBoneModelDraw> m_ptrDraw;   // 描画コンポーネント
		Mat4x4 m_modelMat; // モデルのトランスフォームとの差分行列

	public:

		/*!
		@brief コンストラクタ
		@param ステージポインタ
		@param ポジション
		@param スケール
		*/
		Bumper(const shared_ptr<Stage>& stagePtr,
			const Vec2& position, float scale
		) :
			TemplateObject(stagePtr, Vec3(position), Vec3(0.0f), Vec3(scale))
		{
			m_modelMat.affineTransformation(
				Vec3(1.35f),
				Vec3(0.0f),
				Vec3(-XM_PIDIV2, 0.0f, 0.0f),
				Vec3(0.0f, 0.0f, 0.5f)
			);
		}

		/*!
		@brief デストラクタ
		*/
		virtual ~Bumper() {}

		/*!
		@brief 生成時に一度だけ呼び出される関数
		*/
		void OnCreate() override;

		/*!
		@brief 毎フレーム度に呼び出される関数
		*/
		void OnUpdate() override;

		/*!
		@brief 衝突時にアニメーションをリセットする関数
		*/
		void OnHit()
		{
			StartSE(L"BUMPER_SE", 1.0f); // SEの再生
			m_ptrDraw->ChangeCurrentAnimation(L"PUSH"); // アニメーションを変更
		}
	};

	/*!
	@brief 動くバンパー
	*/
	class MoveBumper : public Bumper
	{
	public:

		// 動くタイプenum
		enum class eMoveType : int8_t
		{
			UpDown,
			LeftRight,
		};

		// ステートenum
		enum class eMoveState : int8_t
		{
			StandBy,
			StartPos,
			MoveA,
			MoveB,
		};

	private:

		eMoveType m_moveType;      // タイプ
		eMoveState m_moveState;	   // ステート
		eMoveState m_currentState; // 前回のステート

		const Vec3 m_startPos; // 開始時座標
		Vec3 m_movePointA;     // 移動位置A
		Vec3 m_movePointB;	   // 移動位置B

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
		MoveBumper(const shared_ptr<Stage>& stagePtr,
			const Vec2& position, const float scale,
			const eMoveType& type, const float speed, const float length
		) :
			Bumper(stagePtr, position, scale),
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
		virtual ~MoveBumper() {}

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