/*!
@file DrawObject.h
@brief 描画用オブジェクトの継承元
*/

#pragma once
#include "stdafx.h"
#include "OzawaUtility.h"

namespace basecross
{
	/*!
	@brief 描画用オブジェクトの継承元
	*/
	class DrawObject : public GameObject
	{
	protected:

		Vec2 m_scale;			// スケール
		Vec2 m_velocity;		// UV用ベロシティ
		Vec3 m_position;		// ポジション
		Vec3 m_rotation;		// ローテーション
		Col4 m_diffuseColor;	// ディヒューズ色
		Col4 m_verticesColor;	// 頂点色
		Col4 m_emissiveColor;	// エミッシブ色
		wstring m_texWstr;		// テクスチャキー
		VertexData m_vertex;	// 頂点データ

		shared_ptr<Transform> m_ptrTrans; // トランスフォームコンポーネント

	public:

		/*!
		@brief コンストラクタ
		@param ステージポインタ
		*/
		DrawObject(const shared_ptr<Stage>& stagePtr) :
			GameObject(stagePtr)
		{
			m_scale.zero();
			m_velocity.zero();
			m_position.zero();
			m_rotation.zero();
			m_texWstr = L"WHITE_TX";
			m_emissiveColor = COL_NONE;
			m_diffuseColor = COL_WHITE;
			m_verticesColor = COL_WHITE;
			Utility::SimpleVerticesIndices(m_vertex);
		}

		/*!
		@brief コンストラクタ
		@param ステージポインタ
		@param テクスチャキー
		@param スケール
		@param ポジション
		*/
		DrawObject(const shared_ptr<Stage>& stagePtr,
			const wstring& texWstr, const Vec3& position,
			const Vec3& rotation, const Vec2& scale
		) :
			GameObject(stagePtr),
			m_texWstr(texWstr),
			m_position(position),
			m_rotation(rotation),
			m_scale(scale)
		{
			m_velocity.zero();
			m_emissiveColor = COL_NONE;
			m_diffuseColor = COL_WHITE;
			m_verticesColor = COL_WHITE;
			Utility::SimpleVerticesIndices(m_vertex);
		}

		/*!
		@brief デストラクタ
		*/
		virtual ~DrawObject() {}

		/*!
		@brief 生成時に一度だけ呼び出される関数
		*/
		virtual void OnCreate() override
		{
			m_ptrTrans = GetComponent<Transform>();
			m_ptrTrans->SetScale(m_scale.x, m_scale.y, 1.0f);
			m_ptrTrans->SetRotation(m_rotation);
			m_ptrTrans->SetPosition(m_position);
		}

		/*!
		@brief 色の範囲が0.0f～1.0fの間になってるかの修正関数
		@param 色
		*/
		void CorrectionColor(Col4& color)
		{
			// 色の範囲が0.0f～1.0fの間になってるかの修正
			for (int i = 0; i < 4; i++)
			{
				color.setElem(i, min(color.getElem(i), 1.0f));
				color.setElem(i, max(color.getElem(i), 0.0f));
			}
		}

		/*!
		@brief スケールを変更する関数
		@param scale
		*/
		void SetScale(const Vec2& scale)
		{
			m_ptrTrans->SetScale(scale.x, scale.y, 1.0f);
			m_scale = scale;
		}

		/*!
		@brief スケールを変更する関数
		@param x
		@param y
		*/
		void SetScale(const float x, const float y)
		{
			m_ptrTrans->SetScale(x, y, 1.0f);
			m_scale.x = x;
			m_scale.y = y;
		}

		/*!
		@brief ポジションを変更する関数
		@param ポジション
		*/
		void SetPosition(const Vec3& position)
		{
			m_ptrTrans->SetPosition(position);
			m_position = position;
		}

		/*!
		@brief ポジションを変更する関数
		@param ポジション
		*/
		void SetPosition(float x, float y, float z)
		{
			SetPosition(Vec3(x, y, z));
		}

		/*!
		@brief ローテーションを変更する関数
		@param ローテーション
		*/
		void SetRotation(const Vec3& rotation)
		{
			m_ptrTrans->SetRotation(rotation);
			m_rotation = rotation;
		}

		/*!
		@brief ローテーションを変更する関数
		@param ローテーション
		*/
		void SetRotation(float x, float y, float z)
		{
			SetRotation(Vec3(x, y, z));
		}

		/*!
		@brief ベロシティを変更する関数
		@param ベロシティ
		*/
		void SetVelocity(const Vec2& velo)
		{
			m_velocity = velo;
		}

		/*!
		@brief ベロシティを変更する関数
		@param ベロシティ
		*/
		void SetVelocity(float x, float y)
		{
			SetVelocity(Vec2(x, y));
		}

		/*!
		@brief スケールを返す関数
		@return スケール
		*/
		Vec2 GetScale() const
		{
			return m_scale;
		}

		/*!
		@brief ポジションを返す関数
		@return ポジション
		*/
		Vec3 GetPosition() const
		{
			return m_position;
		}

		/*!
		@brief ローテーション返す関数
		@return ローテーション
		*/
		Vec3 GetRotation() const
		{
			return m_rotation;
		}

		/*!
		@brief ディフューズ色を返す関数
		@return 色
		*/
		Col4 GetDiffuseColor() const
		{
			return m_diffuseColor;
		}

		/*!
		@brief 頂点色を返す関数
		@return 色
		*/
		Col4 GetVerticesColor() const
		{
			return m_verticesColor;
		}

		/*!
		@brief エミッシブ色を返す関数
		@return 色
		*/
		Col4 GetEmissiveColor() const
		{
			return m_emissiveColor;
		}

		/*!
		@brief テクスチャキーを返す関数
		@return テクスチャキー
		*/
		wstring GetTextureKey() const
		{
			return m_texWstr;
		}
	};
}