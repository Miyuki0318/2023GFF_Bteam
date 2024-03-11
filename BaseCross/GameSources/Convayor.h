/*!
@file Convayor.h
@brief ステージギミックのベルトコンベア
*/

#pragma once
#include "Gimmick.h"
#include "Billboard.h"

namespace basecross
{
	/*!
	@brief ベルトコンベア
	*/
	class Convayor : public Gimmick
	{
	public:

		// ベルトのタイプenum
		enum class eBeltType : int8_t
		{
			Middle, // 内側
			Side,	// 外側
		};

		// 回転タイプenum
		enum class eRotType : int8_t
		{
			LeftRot,  // 左回転
			RightRot, // 右回転
		};

	private:

		eRotType m_rotate; // 回転方向
		const eBeltType m_type; // ベルトタイプ
		const float m_speed; // ベルトコンベアの速度
		const array<wstring, 2> m_animeKey; // アニメーションキー
		shared_ptr<PNTBoneModelDraw> m_ptrDraw; // 描画コンポーネント

	public:

		/*!
		@brief コンストラクタ
		@param ステージポインタ
		@param ポジション
		@param スケール
		@param 回転方向
		@param ベルトタイプ
		@param 速度
		*/
		Convayor(const shared_ptr<Stage>& stagePtr,
			const Vec2& position, const float scale,
			const eRotType& rotate, const eBeltType& type, float speed
		) :
			Gimmick(stagePtr, Vec3(position.x, position.y, 0.0f), Vec3(scale), GimmickAngle::eAngle::Up),
			m_type(type),
			m_rotate(rotate),
			m_speed(speed),
			m_animeKey({
				L"LEFT",
				L"RIGHT"
			})
		{
			m_modelMat.affineTransformation(
				Vec3(1.35f, 1.35f, 1.35f),
				Vec3(0.0f),
				Vec3(0.0f),
				Vec3(0.0f, -0.5f, 0.0f)			
			);
		}

		/*!
		@brief デストラクタ
		*/
		virtual ~Convayor() {}

		/*!
		@brief 生成時に一度だけ呼び出される関数
		*/
		void OnCreate() override;

		/*!
		@brief 毎フレーム度に呼び出される関数
		*/
		void OnUpdate() override;

		/*!
		@brief 回転方向設定関数
		@param eRotType
		*/
		void SetRotate(const eRotType& rot)
		{
			// 現在の回転方向と相違なら
			if (rot != m_rotate)
			{
				m_rotate = rot; // 回転方向を上書き

				// アニメーションを変更
				m_ptrDraw->ChangeCurrentAnimation(m_animeKey.at(static_cast<size_t>(rot)));
			}
		}

		/*!
		@brief 回転方向取得関数
		@return eRotType
		*/
		eRotType GetRotate() const
		{
			return m_rotate;
		}

		/*!
		@brief ベルトの速度取得関数
		@return m_speed
		*/
		const float& GetConvayorSpeed() const
		{
			return m_speed;
		}
	};

	/*!
	@brief ベルトコンベアの方向ガイド
	*/
	class ConvayorGuide : public Billboard
	{
	private:

		Convayor::eRotType m_rotate; // 回転方向

	public:

		/*!
		@brief コンストラクタ
		@param ステージポインタ
		@param ポジション
		@param スケール
		@param 回転方向
		*/
		ConvayorGuide(const shared_ptr<Stage>& stagePtr,
			const Vec2& position, const float scale, const Convayor::eRotType& rotate
		) :
			Billboard(stagePtr, L"GUIDE_TX", Vec2(scale), Vec3(position.x, position.y, 0.0f)),
			m_rotate(rotate)
		{
		}

		/*!
		@brief デストラクタ
		*/
		virtual ~ConvayorGuide() {}

		/*!
		@brief 生成時に一度だけ呼び出される関数
		*/
		void OnCreate() override;

		/*!
		@brief 毎フレーム度に呼び出される関数
		*/
		void OnUpdate() override;
	};
}