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
	@brief ディフューズ色を使って色を変更する関数
	@param 色
	*/
	void Sprite::SetDiffuseColor(const Col4& color)
	{
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
	void Sprite::SetEmissiveColor(const Col4& color)
	{
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
	void Sprite::SetVerticesColor(const Col4& color)
	{
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
}