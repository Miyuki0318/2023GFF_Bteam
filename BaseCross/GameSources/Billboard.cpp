/*!
@file Billboard.cpp
@brief ビルボードオブジェクトの継承元
@author 小澤博貴
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	/*!
	@brief 生成時に一度だけ呼び出される関数
	*/
	void Billboard::OnCreate()
	{
		DrawObject::OnCreate();

		// 描画コンポーネントの設定
		m_ptrDraw = AddComponent<PCTStaticDraw>();
		m_ptrDraw->SetOriginalMeshUse(true);
		m_ptrDraw->CreateOriginalMesh(m_vertex);
		m_ptrDraw->SetTextureResource(m_texWstr);

		// 透明色の描画を可能に
		SetAlphaActive(true);
	}

	/*!
	@brief ビルボード更新関数
	*/
	void Billboard::UpdateBillboard()
	{
		// カメラを元にビルボード関数でクォータニオンを設定
		const auto& ptrCamera = GetStage()->GetView()->GetTargetCamera();
		Quat qt = Utility::GetBillboardQuat(ptrCamera->GetAt() - ptrCamera->GetEye());
		m_ptrTrans->SetQuaternion(qt);
	}

	/*!
	@brief ディフューズ色を使って色を変更する関数
	@param 色
	*/
	void Billboard::SetDiffuseColor(const Col4& color)
	{
		// 描画コンポーネントの取得してディヒューズ色に設定
		m_ptrDraw->SetDiffuse(color);

		// 色を保持
		m_diffuseColor = color;
	}

	/*!
	@brief エミッシブ色を使って色を変更する関数
	@param 色
	*/
	void Billboard::SetEmissiveColor(const Col4& color)
	{
		// 描画コンポーネントの取得してエミッシブ色に設定
		m_ptrDraw->SetEmissive(color);

		// 色を保持
		m_emissiveColor = color;
	}

	/*!
	@brief 頂点色を使って色を変更する関数
	@param 色
	*/
	void Billboard::SetVerticesColor(const Col4& color)
	{
		// 色を頂点色に設定
		for (auto& v : m_vertex.vertices)
		{
			v.color = color;
		}

		// 描画コンポーネントの取得して頂点データの更新
		m_ptrDraw->UpdateVertices(m_vertex.vertices);

		// 色を保持
		m_verticesColor = color;
	}
}