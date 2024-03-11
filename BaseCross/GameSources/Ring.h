/*!
@file Ring.h
@brief ステージギミックのリング
*/

#pragma once
#include "Gimmick.h"

namespace basecross
{
	/*!
	@brief リング
	*/
	class Ring : public Gimmick
	{
	public:

		// リングのサイズenum
		enum class eRingSize : int8_t
		{
			Big,	// 大
			Small,	// 小
		};

	private:

		// 描画コンポーネント
		shared_ptr<PNTBoneModelDraw> m_ptrDraw;

		const eRingSize m_ringSize; // リングサイズ
		bool m_isGet;			// 取得したかの真偽
		float m_rotSpeed;		// 回転速度
		float m_totalTime;		// 経過時間
		float m_deleteTime;		// 削除時間

	public:

		/*!
		@brief コンストラクタ
		@param ステージポインタ
		@param ポジション
		@param スケール
		@param リングサイズ
		*/
		Ring(const shared_ptr<Stage>& stagePtr,
			const Vec2& position, float scale, eRingSize ring
		) :
			Gimmick(stagePtr, Vec3(position.x, position.y, 0.0f), Vec3(scale, scale, scale), GimmickAngle::eAngle::Up),
			m_ringSize(ring)
		{
			m_isGet = false;
			m_rotSpeed = 3.0f;
			m_totalTime = 0.0f;
			m_deleteTime = 1.0f;
			m_modelMat.affineTransformation(
				Vec3(0.475f),
				Vec3(0.0f),
				Vec3(0.0f),
				Vec3(0.0f, -0.75f, 0.0f)
			);
		}

		/*!
		@brief デストラクタ
		*/
		virtual ~Ring() {}

		/*!
		@brief 生成時に一度だけ呼び出される関数
		*/
		void OnCreate() override;

		/*!
		@brief 毎フレーム度に呼び出される関数
		*/
		void OnUpdate() override;

		/*!
		@brief 取得した時の関数
		*/
		void IsGetRing()
		{
			m_isGet = true;
			m_rotSpeed = 12.0f;
			m_ptrColl->SetUpdateActive(false);
		}

		/*!
		@brief リングサイズ取得関数
		@return eRingSize
		*/
		const eRingSize& GetRingSize() const
		{
			return m_ringSize;
		}
	};
}