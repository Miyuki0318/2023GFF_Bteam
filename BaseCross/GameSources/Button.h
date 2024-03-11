/*!
@file Bumper.h
@brief ステージギミックの動く壁の開閉用ボタン
*/

#pragma once
#include "Gimmick.h"

namespace basecross
{
	/*!
	@brief ボタン
	*/
	class Button : public Gimmick
	{
		// 描画コンポーネント
		shared_ptr<PNTBoneModelDraw> m_ptrDraw;

		const int m_number;  // 動く壁のID

		bool m_active;		 // ボタンを押してるかの真偽
		bool m_current;		 // 前回までの状態
		float m_reverseTime; // アニメーションの再生時間

	public:

		/*!
		@brief コンストラクタ
		@param ステージポインタ
		@param ポジション
		@param スケール
		@param 動く壁とのID
		*/
		Button(const shared_ptr<Stage>& stagePtr,
			const Vec2& position, float scale, int number
		) :
			Gimmick(stagePtr, Vec3(position), Vec3(scale * 2.0f, scale, scale), GimmickAngle::eAngle::Up),
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
		bool GetHitButton() const 
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

				// ターゲット座標との距離がスケールより短ければ
				if ((pos - Vec2(GetPosition())).length() <= Vec2(GetScale()).length())
				{
					// 左右端の座標
					const Vec2 left = Vec2(GetPosition() - GetScale() / 2.0f); 
					const Vec2 right = Vec2(GetPosition() + GetScale() / 2.0f);

					// 範囲内ならreturnを返して終了
					if (Utility::GetBetween(pos, left, right))
					{
						return true;
					}

					// スケールの半分を取得
					const Vec2 helfScale = Vec2(targetPtr->GetScale()) / 2.0f;
					
					// 座標からスケール分の座標四点を求める
					vector<Vec2> points = {
						(pos + Vec2(-helfScale.x, helfScale.y)),
						(pos - helfScale),
						(pos + helfScale),
						(pos + Vec2(helfScale.x, -helfScale.y))
					};

					// 四点の内、どれか一つでも範囲内なら
					for (const auto& point : points)
					{
						// 範囲内ならreturnを返して終了
						if (Utility::GetBetween(point, left, right))
						{
							return true;
						}
					}
				}
			}
			return false;
		}

		/*!
		@brief ボタンのID取得関数
		@return m_number
		*/
		int GetButtonNumber() const
		{
			return m_number;
		}

		/*!
		@brief ボタンの入力があるかの取得関数
		@return m_active
		*/
		bool GetInput() const
		{
			return m_active;
		}
	};
}