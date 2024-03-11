/*!
@file MoveWall.h
@brief ステージギミックの動く壁
*/

#pragma once
#include "Gimmick.h"
#include "Button.h"

namespace basecross
{
	/*!
	@brief 動く壁
	*/
	class MoveWall : public Gimmick
	{
	public:

		// 動く方向タイプenum
		enum class eMoveType : int8_t
		{
			Down,   // 下方向
			Up,		// 上方向
			Left,	// 左方向
			Right,	// 右方向
			DownSE,	// SE付き下方向
			UpSE,	// SE付き上方向
			LeftSE, // SE付き左方向
			RightSE	// SE付き右方向
		};

	private:

		shared_ptr<PNTStaticDraw> m_ptrDraw; // 描画コンポーネント
		vector<weak_ptr<Button>> m_buttons;  // 開閉用のボタン

		const eMoveType m_moveType; // 動く方向タイプ
		const Vec3 m_startPos;		// 開始座標
		const int m_number;			// ボタン識別ナンバー
		const float m_moveSpeed;	// 開閉速度
		const float m_moveLength;	// 開閉距離

		float m_moveRatio;		// 動いた量
		float m_currentRatio;	// 前回の動いた量
		bool m_currentInput;	// 前回動いたか
		Vec3 m_movePoint;		// 開閉座標

	public:

		/*!
		@brief コンストラクタ
		@param ステージポインタ
		@param ポジション
		@param スケール
		@param 動く方向タイプ
		@param 開閉速度
		@param 開閉距離
		@param 識別ナンバー
		*/
		MoveWall(const shared_ptr<Stage>& stagePtr,
			const Vec2& position, const float scale,
			const eMoveType& type, const float speed, 
			const float length, const int number
		) :
			Gimmick(stagePtr, Vec3(position), Vec3(scale), GimmickAngle::eAngle::All),
			m_moveType(type),
			m_moveSpeed(speed),
			m_moveLength(length),
			m_number(number),
			m_startPos(position)
		{
			m_movePoint = position;
			m_moveRatio = 0.0f;
			m_currentRatio = 0.0f;
			m_currentInput = false;

			m_modelMat.affineTransformation(
				Vec3(1.35f),
				Vec3(0.0f),
				Vec3(0.0f),
				Vec3(0.0f, -0.5f, 0.0f)
			);
		}

		/*!
		@brief デストラクタ
		*/
		virtual ~MoveWall() {}

		/*!
		@brief 生成時に一度だけ呼び出される関数
		*/
		void OnCreate() override;

		/*!
		@brief 毎フレーム度に呼び出される関数
		*/
		void OnUpdate() override;

		/*!
		@brief 毎フレーム更新処理を行うかの設定関数
		*/
		void SetUpdateActive(bool b) override;

		/*!
		@brief 開閉用ボタン設定関数
		*/
		void SetTargetButton();

		/*!
		@brief 動く壁移動関数
		@param 開始座標
		@param 終了座標
		*/
		void MoveWallBlock(const Vec3& start, const Vec3& end);

		/*!
		@brief 動く壁の移動量取得関数
		@return m_moveRatio
		*/
		const float GetMoveRatio() const
		{
			return m_moveRatio;
		}
	};
}