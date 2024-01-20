#pragma once
#include "stdafx.h"
#include "Gimmick.h"

namespace basecross
{
	class Button : public Gimmick
	{
		shared_ptr<PNTBoneModelDraw> m_ptrDraw;

		const int m_number;

		bool m_active;
		bool m_current;
		float m_reverseTime;

	public:

		/*!
		@brief コンストラクタ
		@param ステージポインタ
		*/
		Button(const shared_ptr<Stage>& stagePtr,
			const Vec2& position, float scale, int number
		) :
			Gimmick(stagePtr, Vec3(position), Vec3(scale), Gimmick::Up),
			m_number(number)
		{
			m_active = false;
			m_current = m_active;
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
				// エラーチェック
				if (!objPtr.lock()) continue;

				// 型キャスト
				const auto& targetPtr = dynamic_pointer_cast<TemplateObject>(objPtr.lock());
				if (!targetPtr) continue;

				// 座標の定義
				const Vec2 pos = Vec2(targetPtr->GetPosition());
				if ((pos - Vec2(GetPosition())).length() <= Vec2(GetScale()).length())
				{
					const Vec2 left = Vec2(GetPosition() - GetScale() / 2.0f);
					const Vec2 right = Vec2(GetPosition() + GetScale() / 2.0f);
					if (Utility::GetBetween(pos, left, right))
					{
						return true;
					}

					const Vec2 helfScale = Vec2(targetPtr->GetScale()) / 2.0f;
					vector<Vec2> points = {
						(pos + Vec2(-helfScale.x, helfScale.y)),
						(pos - helfScale),
						(pos + helfScale),
						(pos + Vec2(helfScale.x, -helfScale.y))
					};

					for (const auto& point : points)
					{
						if (Utility::GetBetween(point, left, right))
						{
							return true;
						}
					}
				}
			}
			return false;
		}

		int GetButtonNumber() const
		{
			return m_number;
		}

		bool GetInput() const
		{
			return m_active;
		}
	};
}