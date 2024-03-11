/*!
@file Blower.h
@brief ステージギミックの送風機
*/

#pragma once
#include "Gimmick.h"

namespace basecross
{
	/*!
	@brief 送風機
	*/
	class Blower : public Gimmick
	{
		shared_ptr<PNTBoneModelDraw> m_ptrDraw; // 描画コンポーネント
		const float m_range; // 送風範囲

	public:

		/*!
		@brief コンストラクタ
		@param ステージポインタ
		@param ポジション
		@param スケール
		@param 角度
		@param 範囲
		*/
		Blower(const shared_ptr<Stage>& stagePtr,
			const Vec2& position, const Vec3& scale,
			const GimmickAngle::eAngle& angle, const float length
		) :
			Gimmick(stagePtr, Vec3(position), scale, angle),
			m_range(length)
		{
			m_modelMat.affineTransformation(
				Vec3(0.4f, 2.0f, 0.4f),
				Vec3(0.0f),
				Vec3(0.0f, XM_PI, 0.0f),
				Vec3(0.0f, -0.5f, 0.0f)
			);
		}

		/*!
		@brief デストラクタ
		*/
		virtual ~Blower() {}

		/*!
		@brief 生成時に一度だけ呼び出される関数
		*/
		void OnCreate() override;

		/*!
		@brief 毎フレーム度に呼び出される関数
		*/
		void OnUpdate() override;

		/*!
		@brief 送風範囲に居るかの真偽取得関数
		@param 測定する座標
		@return 居るかの真偽
		*/
		bool CheckBetween(const Vec3& targetPos) const;
	};
}