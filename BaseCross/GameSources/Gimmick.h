/*!
@file Gimmick.h
@brief ステージギミックの継承元
*/

#pragma once
#include "CubeObject.h"

namespace basecross
{
	/*!
	@brief ギミック用角度
	*/
	namespace GimmickAngle
	{
		// 角度enum
		enum class eAngle : int8_t
		{
			Up,		// 上
			Down,	// 下
			Left,	// 左
			Right,	// 右
			All,	// 全方位
			Uleft,	// 左上
			Uright,	// 右上
			Dleft,	// 左下
			Dright,	// 右下
			Invers,	// 反転
		};

		/*!
		@brief 角度取得関数
		@param eAngle
		@return 角度
		*/
		static const Vec3 GetAngle(const eAngle& angle)
		{
			switch (angle)
			{
			case eAngle::Down:
				return Vec3(0.0f, 0.0f, 180.0f); // 上向きの角度
				break;

			case eAngle::Left:
				return Vec3(0.0f, 0.0f, 90.0f); // 左向きの角度
				break;

			case eAngle::Right:
				return Vec3(0.0f, 0.0f, -90.0f); // 右向きの角度
				break;

			case eAngle::Uleft:
				return Vec3(0.0f, 0.0f, 45.0f); // 左上向きの角度
				break;

			case eAngle::Uright:
				return Vec3(0.0f, 0.0f, -45.0f); // 右上向きの角度
				break;

			case eAngle::Dleft:
				return Vec3(0.0f, 0.0f, 135.0f); // 左下向きの角度
				break;

			case eAngle::Dright:
				return Vec3(0.0f, 0.0f, 225.0f); // 右下向きの角度
				break;

			case eAngle::Invers:
				return Vec3(0.0f, 180.0f, 0.0f); // 反対向きの角度
				break;

			default:
				return Vec3(0.0f); // 上向き、またはそれ以外の角度
				break;
			}
		}
	};

	/*!
	@brief ステージギミックの継承元
	*/
	class Gimmick : public CubeObject
	{
	protected:

		Mat4x4 m_modelMat; // モデルのトランスフォームとの差分行列
		GimmickAngle::eAngle m_angle; // 角度

	public:

		/*!
		@brief コンストラクタ
		@param ステージポインタ
		*/
		Gimmick(const shared_ptr<Stage>& stagePtr) :
			CubeObject(stagePtr)
		{
			m_angle = GimmickAngle::eAngle::Up;
		}

		/*!
		@brief コンストラクタ
		@param ステージポインタ
		@param ポジション
		@param スケール
		@param アングル
		*/
		Gimmick(const shared_ptr<Stage>& stagePtr,
			const Vec3& position, const Vec3& scale, const GimmickAngle::eAngle& angle
		) :
			CubeObject(stagePtr, position, Vec3(0.0f, 0.0f, 0.0f), scale, true),
			m_angle(angle)
		{
		}

		/*!
		@brief デストラクタ
		*/
		virtual ~Gimmick() {}

		/*!
		@brief 生成時に一度だけ呼び出される関数
		*/
		void OnCreate() override;

		/*!
		@brief 角度取得関数
		@return m_angle
		*/
		const GimmickAngle::eAngle& GetAngle() const
		{
			return m_angle;
		}
	};
}