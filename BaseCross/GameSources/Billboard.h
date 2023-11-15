/*!
@file Billboard.h
@brief ビルボードオブジェクトの継承元
@author 小澤博貴
*/

#pragma once
#include "stdafx.h"

namespace basecross
{
	// ----------------------------------------------------------------------------------
	//	Billboard : public GameObject ビルボードオブジェクトの継承元
	// ----------------------------------------------------------------------------------
	class Billboard : public GameObject
	{
	protected:

		Vec2 m_scale;					// スケール
		Vec3 m_position;				// ポジション
		Vec3 m_rotation;				// ローテーション
		wstring m_texWstr;				// テクスチャキー
		Col4 m_diffuseColor;			// ディヒューズ色
		Col4 m_verticesColor;			// 頂点色
		Col4 m_emissiveColor;			// エミッシブ色
		VertexData m_vertex;	// 頂点データ

	public:

		/*!
		@brief コンストラクタ
		@param ステージポインタ
		*/
		Billboard(const shared_ptr<Stage>& stagePtr) :
			GameObject(stagePtr)
		{
			m_scale.zero();
			m_position.zero();
			m_rotation.zero();
			m_texWstr = L"WHITE";
			Utility::SimpleVerticesIndices(m_vertex);
		}

		/*!
		@brief コンストラクタ
		@param ステージポインタ
		@param テクスチャキー
		@param スケール
		@param ポジション
		*/
		Billboard(const shared_ptr<Stage>& stagePtr,
			const wstring& texWstr, const Vec2& scale,
			const Vec3& position
			) :
			GameObject(stagePtr),
			m_texWstr(texWstr),
			m_scale(scale),
			m_position(position)
		{
			Utility::SimpleVerticesIndices(m_vertex);
		}

		/*!
		@brief デストラクタ
		*/
		virtual ~Billboard() {}

		/*!
		@brief 生成時に一度だけ呼び出される関数
		*/
		virtual void OnCreate() override;

		/*!
		@brief ビルボード更新関数
		*/
		void UpdateBillboard();

		/*!
		@brief ディフューズ色を使って色を変更する関数
		@param 色
		*/
		void SetDiffuseColor(const Col4& color);

		/*!
		@brief 頂点色を使って色を変更する関数
		@param 色
		*/
		void SetVerticesColor(const Col4& color);

		/*!
		@brief エミッシブ色を使って色を変更する関数
		@param 色
		*/
		void SetEmissiveColor(const Col4& color);
		
		/*!
		@brief スケールを変更する関数
		@param scale
		*/
		void SetScale(const Vec2& scale)
		{
			auto ptrTrans = GetComponent<Transform>();
			ptrTrans->SetScale(scale.x, scale.y, 1.0f);
			m_scale = scale;
		}

		/*!
		@brief スケールを変更する関数
		@param x
		@param y
		*/
		void SetScale(const float x, const float y)
		{
			auto ptrTrans = GetComponent<Transform>();
			ptrTrans->SetScale(x, y, 1.0f);
			m_scale.x = x;
			m_scale.y = y;
		}

		/*!
		@brief ポジションを変更する関数
		@param ポジション
		*/
		void SetPosition(const Vec3& position)
		{
			auto ptrTrans = GetComponent<Transform>();
			ptrTrans->SetPosition(position);
			m_position = position;
		}

		/*!
		@brief ポジションを変更する関数
		@param ポジション
		*/
		void SetPosition(float x, float y , float z)
		{
			SetPosition(Vec3(x, y, z));
		}

		/*!
		@brief ローテーションを変更する関数
		@param ローテーション
		*/
		void SetRotation(const Vec3& rotation)
		{
			auto ptrTrans = GetComponent<Transform>();
			ptrTrans->SetRotation(rotation);
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
		@brief テクスチャを変更する関数
		@param テクスチャキー
		*/
		void SetTexture(const wstring& texWstr)
		{
			auto ptrDraw = GetComponent<PCTStaticDraw>();
			ptrDraw->SetTextureResource(texWstr);
			m_texWstr = texWstr;
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