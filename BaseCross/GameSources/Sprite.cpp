/*!
@file Sprite.cpp
@brief スプライト親クラス
@author 小澤博貴
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	/*!
	@brief 生成時に一度だけ呼び出される関数
	*/
	void Sprite::OnCreate()
	{
		// 継承元の生成時の関数を実行
		DrawObject::OnCreate();

		// 描画コンポーネントの設定
		m_ptrDraw = AddComponent<PCTSpriteDraw>(m_vertex);
		m_ptrDraw->SetTextureResource(m_texWstr);
		m_ptrDraw->SetSamplerState(SamplerState::LinearWrap);

		// 透明色の描画設定
		SetAlphaActive(true);
	}

	/*!
	@brief ディフューズ色を使って色を変更する関数
	@param 色
	*/
	void Sprite::SetDiffuseColor(Col4& color)
	{
		// 色の修正
		CorrectionColor(color);

		// ディヒューズ色に設定
		m_ptrDraw->SetDiffuse(color);

		// 色を保持
		m_diffuseColor = color;
	}

	/*!
	@brief エミッシブ色を使って色を変更する関数
	@param 色
	*/
	void Sprite::SetEmissiveColor(Col4& color)
	{
		// 色の修正
		CorrectionColor(color);

		// エミッシブ色に設定
		m_ptrDraw->SetEmissive(color);

		// 色を保持
		m_emissiveColor = color;
	}

	/*!
	@brief 頂点色を使って色を変更する関数
	@param 色
	*/
	void Sprite::SetVerticesColor(Col4& color)
	{
		// 色の修正
		CorrectionColor(color);

		// 色を頂点色に設定
		for (auto& v : m_vertex.vertices)
		{
			v.color = color;
		}

		// 頂点データの更新
		m_ptrDraw->UpdateVertices(m_vertex.vertices);

		// 色を保持
		m_verticesColor = color;
	}

	/*!
	@brief テクスチャを変更する関数
	@param テクスチャキー
	*/
	void Sprite::SetTexture(const wstring& textureKey)
	{
		m_ptrDraw->SetTextureResource(textureKey);
	}

	/*!
	@brief フェードイン処理する関数
	@param フェードに掛かる時間
	@return フェードが完了したかの真偽
	*/
	bool Sprite::FadeInColor(const float time)
	{
		// 既に透明なら終了
		if (m_diffuseColor.w >= 1.0f) return true;

		// 色の取得
		Col4 color = m_diffuseColor;

		// 色の値をデルタタイムで加算する
		color.w += DELTA_TIME / time;

		// メッシュのディフューズ色を設定
		SetDiffuseColor(color);

		// フェードが完了したかの真偽を返す
		return color.w >= 1.0f;
	}

	/*!
	@brief フェードイン処理する関数
	@param フェードの終了値
	@param フェードに掛かる時間
	@return フェードが完了したかの真偽
	*/
	bool Sprite::FadeInColor(const float time, const float alpha)
	{
		// 既に透明なら終了
		if (m_diffuseColor.w >= alpha) return true;

		// 色の取得
		Col4 color = m_diffuseColor;

		// 色の値をデルタタイムで加算する
		color.w += DELTA_TIME / time;

		// メッシュのディフューズ色を設定
		SetDiffuseColor(color);

		// フェードが完了したかの真偽を返す
		return color.w >= alpha;
	}

	/*!
	@brief フェードイン処理する関数
	@param フェードに掛かる時間
	@return フェードが完了したかの真偽
	*/
	bool Sprite::FadeOutColor(const float time)
	{
		// 既に透明なら終了
		if (m_diffuseColor.w <= 0.0f) return true;

		// 色の取得
		Col4 color = m_diffuseColor;

		// 色の値をデルタタイムで加算する
		color.w -= DELTA_TIME / time;

		// メッシュのディフューズ色を設定
		SetDiffuseColor(color);

		// フェードが完了したかの真偽を返す
		return color.w <= 0.0f;
	}

	/*!
	@brief フェードイン処理する関数
	@param フェードの終了値
	@param フェードに掛かる時間
	@return フェードが完了したかの真偽
	*/
	bool Sprite::FadeOutColor(const float time, const float alpha)
	{
		// 既に透明なら終了
		if (m_diffuseColor.w <= alpha) return true;

		// 色の取得
		Col4 color = m_diffuseColor;

		// 色の値をデルタタイムで加算する
		color.w -= DELTA_TIME / time;

		// メッシュのディフューズ色を設定
		SetDiffuseColor(color);

		// フェードが完了したかの真偽を返す
		return color.w <= alpha;
	}

	/*!
	@brief テクスチャをベロシティに応じて回転させる関数
	*/
	void Sprite::RotateTexture()
	{
		// 頂点の数分ループ
		for (auto& v : m_vertex.vertices)
		{
			// 頂点のUV座標を移動量×デルタタイムで加算
			v.textureCoordinate += m_velocity * DELTA_TIME;
		}

		// 頂点の更新
		m_ptrDraw->UpdateVertices(m_vertex.vertices);
	}
}