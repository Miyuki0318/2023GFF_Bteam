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
		// トランスフォームの設定
		auto ptrTrans = GetComponent<Transform>();
		ptrTrans->SetScale(m_scale.x, m_scale.y, 1.0f);
		ptrTrans->SetRotation(Vec3(0.0f));
		ptrTrans->SetPosition(m_position);

		// 描画コンポーネントの設定
		auto ptrDraw = AddComponent<PCTSpriteDraw>(m_vertex);
		ptrDraw->SetTextureResource(m_texWstr);
		m_verticesColor = COL_WHITE;
		m_diffuseColor = ptrDraw->GetDiffuse();
		m_emissiveColor = ptrDraw->GetEmissive();

		// 透明色の描画設定
		SetAlphaActive(true);
	}

	/*!
	@brief 色の範囲が0.0f～1.0fの間になってるかの修正関数
	@param 色
	*/
	void Sprite::CorrectionColor(Col4& color)
	{
		// 色の範囲が0.0f～1.0fの間になってるかの修正
		for (int i = 0; i < 4; i++)
		{
			color.setElem(i, min(color.getElem(i), 1.0f));
			color.setElem(i, max(color.getElem(i), 0.0f));
		}
	}

	/*!
	@brief ディフューズ色を使って色を変更する関数
	@param 色
	*/
	void Sprite::SetDiffuseColor(Col4& color)
	{
		// 色の修正
		CorrectionColor(color);

		// 描画コンポーネントの取得してディヒューズ色に設定
		auto ptrDraw = GetComponent<PCTSpriteDraw>();
		ptrDraw->SetDiffuse(color);

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

		// 描画コンポーネントの取得してエミッシブ色に設定
		auto ptrDraw = GetComponent<PCTSpriteDraw>();
		ptrDraw->SetEmissive(color);

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

		// 描画コンポーネントの取得して頂点データの更新
		auto ptrDraw = GetComponent<PCTSpriteDraw>();
		ptrDraw->UpdateVertices(m_vertex.vertices);

		// 色を保持
		m_verticesColor = color;
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
}