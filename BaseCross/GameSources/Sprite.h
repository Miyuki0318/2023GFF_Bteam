/*!
@file Sprite.h
@brief スプライト親クラス
@author 小澤博貴
*/

#pragma once
#include "stdafx.h"

namespace basecross
{
	// ----------------------------------------------------------------------------------
	//	Sprite : public GameObject 単純な表示を行うスプライト親クラス
	// ----------------------------------------------------------------------------------
	class Sprite : public GameObject
	{
	protected:

		Vec2 m_scale;					// スケール
		Vec3 m_position;				// ポジション
		Col4 m_diffuseColor;			// ディヒューズ色
		Col4 m_verticesColor;			// 頂点色
		Col4 m_emissiveColor;			// エミッシブ色
		wstring m_texWstr;				// テクスチャキー

		VertexData m_vertex;	// 頂点データ

	public:

		/*!
		@brief コンストラクタ
		@param ステージポインタ
		@param テクスチャキー
		@param 透明色の描画
		@param スケール
		@param ポジション
		*/
		Sprite(const shared_ptr<Stage>& stagePtr,
			const wstring& texWstr,	const Vec2& scale, const Vec3& pos
		) :
			GameObject(stagePtr),
			m_texWstr(texWstr),
			m_scale(scale),
			m_position(pos)
		{
			Utility::SimpleVerticesIndices(m_vertex);
		}

		/*!
		@brief デストラクタ
		*/
		virtual ~Sprite() {}

		/*!
		@brief 生成時に一度だけ呼び出される関数
		*/
		virtual void OnCreate() override;
		
		/*!
		@brief 色の範囲が0.0f～1.0fの間になってるかの修正関数
		@param 色
		*/
		void CorrectionColor(Col4& color);

		/*!
		@brief ディフューズ色を使って色を変更する関数
		@param 色
		*/
		void SetDiffuseColor(Col4& color);

		/*!
		@brief 頂点色を使って色を変更する関数
		@param 色
		*/
		void SetVerticesColor(Col4& color);

		/*!
		@brief エミッシブ色を使って色を変更する関数
		@param 色
		*/
		void SetEmissiveColor(Col4& color);

		/*!
		@brief フェードイン処理する関数
		@param フェードに掛かる時間
		@return フェードが完了したかの真偽
		*/
		bool FadeInColor(const float time);

		/*!
		@brief フェードイン処理する関数
		@param フェードに掛かる時間
		@param フェードの終了値
		@return フェードが完了したかの真偽
		*/
		bool FadeInColor(const float time, const float alpha);

		/*!
		@brief フェードイン処理する関数
		@param フェードに掛かる時間
		@return フェードが完了したかの真偽
		*/
		bool FadeOutColor(const float time);

		/*!
		@brief フェードイン処理する関数
		@param フェードに掛かる時間
		@param フェードの終了値
		@return フェードが完了したかの真偽
		*/
		bool FadeOutColor(const float time, const float alpha);

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
		@brief テクスチャを変更する関数
		@param テクスチャキー
		*/
		void SetTexture(const wstring& texWstr)
		{
			auto ptrDraw = GetComponent<PCTSpriteDraw>();
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