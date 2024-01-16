#pragma once
#include "stdafx.h"
#include "Gimmick.h"

namespace basecross
{
	class Button : public Gimmick
	{
		shared_ptr<PNTBoneModelDraw> m_ptrDraw;
		Mat4x4 m_modelMat;

		bool m_active;
		bool m_current;
		bool m_aliveTarget;
		float m_reverseTime;

	public:

		/*!
		@brief コンストラクタ
		@param ステージポインタ
		*/
		Button(const shared_ptr<Stage>& stagePtr,
			const Vec2& position, float scale
		) :
			Gimmick(stagePtr, Vec3(position), Vec3(scale), Gimmick::Up)
		{
			m_active = false;
			m_current = m_active;
			m_aliveTarget = false;
			m_reverseTime = 0.0f;

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
		virtual ~Button() {}

		/*!
		@brief 生成時に一度だけ呼び出される関数
		*/
		void OnCreate() override;

		/*!
		@brief 毎フレーム度に呼び出される関数
		*/
		void OnUpdate() override;

		/*!
		@brief ボタンにターゲットが接触しているか取得する関数(OnCollision系関数が謎に機能してない為)
		@return 衝突しているかの真偽
		*/
		bool GetHitButton()
		{
			for (const auto& objPtr : m_targetObj)
			{
				if (objPtr.lock())
				{
					// 型キャスト
					const auto& targetPtr = dynamic_pointer_cast<TemplateObject>(objPtr.lock());
					if (targetPtr)
					{
						// 座標の定義
						const Vec2 pos = Vec2(targetPtr->GetPosition());
						if ((pos - Vec2(GetPosition())).length() <= Vec2(GetScale()).length())
						{
							const Vec2 left = Vec2(GetPosition() - GetScale() / 2.0f);
							const Vec2 right = Vec2(GetPosition() + GetScale() / 2.0f);

							const float degAngle = 4.0f;
							const int loopNum = 360 / static_cast<int>(degAngle);

							// 360度を90回のループで処理する
							for (size_t i = 0; i < loopNum; i++)
							{
								float rad = Utility::DegToRad(i * degAngle);
								Vec2 circle = Vec2(cos(rad), sin(rad)).normalize() * 0.5f;
								if (Utility::GetBetween(pos + circle, left, right))
								{
									return true;
								}
							}
						}
					}
				}
			}
			return false;
		}

		void SetTargetAlive(bool b)
		{
			m_aliveTarget = b;
		}

		bool GetTargetAlive() const
		{
			return m_aliveTarget;
		}
	};
}