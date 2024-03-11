/*!
@file TemplateObject.h
@brief オブジェクトの継承元
*/

#pragma once
#include "stdafx.h"

namespace basecross
{
	/*!
	@brief テンプレートオブジェクト
	*/
	class TemplateObject : public GameObject
	{
	protected:

		shared_ptr<Transform> m_ptrTrans; // トランスフォームコンポーネント

		Vec3 m_position; // ポジション
		Vec3 m_rotation; // ローテーション
		Vec3 m_scale;    // スケール

	public:

		/*!
		@brief コンストラクタ
		@param ステージポインタ
		*/
		TemplateObject(const shared_ptr<Stage>& stagePtr
		) :
			GameObject(stagePtr)
		{
			m_position.zero();
			m_rotation.zero();
			m_scale.zero();
		}

		/*!
		@brief コンストラクタ
		@param ステージポインタ
		@param ポジション
		@param ローテーション
		@param スケール
		*/
		TemplateObject(const shared_ptr<Stage>& stagePtr,
			const Vec3& position, const Vec3& rotation, const Vec3& scale
		) :
			GameObject(stagePtr),
			m_position(position),
			m_rotation(rotation),
			m_scale(scale)
		{
		}

		/*!
		@brief デストラクタ
		*/
		virtual ~TemplateObject() {}

		/*!
		@brief 生成時に一度だけ呼び出される関数
		*/
		virtual void OnCreate() override;

		/*!
		@brief ステージステート設定関数
		@param eStageState
		@param エラーログを出すかの真偽(デフォルトはtrue)
		*/
		template <typename T>
		void SetStageState(const typename T::eStageState& state, bool exceptionActive = true)
		{
			GetTypeStage<T>(exceptionActive)->SetStageState(state);
		}

		/*!
		@brief ステージステート取得
		@param エラーログを出すかの真偽(デフォルトはtrue)
		@return eStageState
		*/
		template <typename T>
		const typename T::eStageState& GetStageState(bool exceptionActive = true)
		{
			return GetTypeStage<T>(exceptionActive)->GetStageState();
		}

		/*!
		@brief ポジション設定関数
		@param Vec3型座標
		*/
		virtual void SetPosition(const Vec3& position)
		{
			m_position = position;
			m_ptrTrans->SetPosition(m_position);
		}

		/*!
		@brief ポジション設定関数
		@param Vec2型座標
		*/
		virtual void SetPosition(const Vec2& position)
		{
			SetPosition(Vec3(position));
		}

		/*!
		@brief ポジション設定関数
		@param float3型座標
		*/
		virtual void SetPosition(const float& x, const float& y, const float& z)
		{
			SetPosition(Vec3(x, y, z));
		}

		/*!
		@brief ポジション設定関数
		@param float2型座標
		*/
		virtual void SetPosition(const float& x, const float& y)
		{
			SetPosition(Vec2(x, y));
		}

		/*!
		@brief ポジション取得関数
		@return Vec3型座標
		*/
		virtual Vec3 GetPosition() const
		{
			return m_ptrTrans->GetPosition();
		}

		/*!
		@brief ローテーション設定関数
		@param Vec3型Radian
		*/
		virtual void SetRotation(const Vec3& rotation)
		{
			m_rotation = rotation;
			m_ptrTrans->SetRotation(m_rotation);
		}

		/*!
		@brief ローテーション設定関数
		@param Vec3型Degree
		*/
		virtual void SetDegreeAngle(const Vec3& degree)
		{
			SetRotation(Utility::DegToRad(degree));
		}

		/*!
		@brief ローテーション取得関数
		@return Vec3型Radian
		*/
		virtual Vec3 GetRotation() const
		{
			return m_rotation;
		}

		/*!
		@brief ローテーション取得関数
		@return Vec3型Degree
		*/
		virtual Vec3 GetDegreeAngle() const
		{
			return Utility::RadToDeg(m_rotation);
		}

		/*!
		@brief スケール設定関数
		@param Vec3型Scale
		*/
		virtual void SetScale(const Vec3& scale)
		{
			m_scale = scale;
			m_ptrTrans->SetScale(m_scale);
		}

		/*!
		@brief スケール設定関数
		@param Vec2型Scale
		*/
		virtual void SetScale(const Vec2& scale)
		{
			SetScale(Vec3(scale.x, scale.y, 1.0f));
		}

		/*!
		@brief スケール設定関数
		@param float3型Scale
		*/
		virtual void SetScale(const float& x, const float& y, const float& z)
		{
			SetScale(Vec3(x, y, z));
		}

		/*!
		@brief スケール設定関数
		@param float2型Scale
		*/
		virtual void SetScale(const float& x, const float& y)
		{
			SetScale(Vec2(x, y));
		}

		/*!
		@brief スケール設定関数
		@param float型Scale
		*/
		virtual void SetScale(const float& scale)
		{
			SetScale(Vec3(scale));
		}

		/*!
		@brief スケール取得関数
		@return Vec3型Scale
		*/
		virtual Vec3 GetScale() const
		{
			return m_scale;
		}

		/*!
		@brief トランスフォーム簡易設定
		*/
		virtual void SetTransParam()
		{
			SetPosition(m_position);
			SetRotation(m_rotation);
			SetScale(m_scale);
		}

		/*!
		@brief SE再生関数(途中で終了できる版)
		*/
		virtual void StartSE(const wstring& seKey, float volume);

		/*!
		@brief SE終了関数
		*/
		virtual void StopSE(const wstring& seKey);

		/*!
		@brief タイマー関数
		@param 設定時間(s)
		@param リセット真偽(デフォルトはfalse)
		@return タイマーが終了しているかの真偽
		*/
		virtual bool SetTimer(float time, bool reset = false);

		/*!
		@brief タイマーの時間取得関数
		@return 経過時間
		*/
		virtual float GetTime(float time);

		/*!
		@brief 指定の座標にオブジェクトがあるかの確認関数
		@return 存在していればtrue
		*/
		virtual bool ObjectCheck(const vector<weak_ptr<GameObject>>& groupVec, const Vec3& checkPos)
		{
			bool check = false;

			for (const auto& ptr : groupVec)
			{
				if (!ptr.lock()) continue;

				// 型キャスト
				const auto& block = dynamic_pointer_cast<TemplateObject>(ptr.lock());
				if (!block) continue;

				// 座標が一致しているか
				if (block->GetPosition() == checkPos)
				{
					check = true;
				}
			}

			return check;
		}

		/*!
		@brief 指定の座標にアクティブなブロックがあるかの確認関数
		@return 存在していればtrue
		*/
		virtual bool BlockCheck(const Vec3& checkPos)
		{
			// アクティブグループの取得
			const auto& blockVec = GetStage()->GetSharedObjectGroup(L"Active")->GetGroupVector();

			return ObjectCheck(blockVec, checkPos);
		}

		/*!
		@brief オブジェクトに上から衝突しているか
		@param 衝突座標
		@param 衝突したオブジェクトの座標
		@param 衝突したオブジェクトの半分のスケール
		@return 衝突しているかの真偽
		*/
		virtual bool CollHitUpper(const Vec3& hitPos, const Vec3& hitObjPos, const Vec3& helfScale)
		{
			return hitPos.y > hitObjPos.y && ((hitPos.y - hitObjPos.y) >= helfScale.y);
		}

		/*!
		@brief オブジェクトに下から衝突しているか
		@param 衝突座標
		@param 衝突したオブジェクトの座標
		@param 衝突したオブジェクトの半分のスケール
		@return 衝突しているかの真偽
		*/
		virtual bool CollHitUnder(const Vec3& hitPos, const Vec3& hitObjPos, const Vec3& helfScale)
		{
			return hitPos.y < hitObjPos.y && ((hitPos.y - hitObjPos.y) <= -helfScale.y);
		}

		/*!
		@brief オブジェクトに左から衝突しているか
		@param 衝突座標
		@param 衝突したオブジェクトの座標
		@param 衝突したオブジェクトの半分のスケール
		@return 衝突しているかの真偽
		*/
		virtual bool CollHitLeft(const Vec3& hitPos, const Vec3& hitObjPos, const Vec3& helfScale)
		{
			return ((hitPos.y - hitObjPos.y) < helfScale.y && (hitPos.y - hitObjPos.y) > -helfScale.y)
				&& ((hitPos.x - hitObjPos.x) < helfScale.x);
		}

		/*!
		@brief オブジェクトに右から衝突しているか
		@param 衝突座標
		@param 衝突したオブジェクトの座標
		@param 衝突したオブジェクトの半分のスケール
		@return 衝突しているかの真偽
		*/
		virtual bool CollHitRight(const Vec3& hitPos, const Vec3& hitObjPos, const Vec3& helfScale)
		{
			return ((hitPos.y - hitObjPos.y) < helfScale.y && (hitPos.y - hitObjPos.y) > -helfScale.y)
				&& ((hitPos.x - hitObjPos.x) > -helfScale.x);
		}
	};
}