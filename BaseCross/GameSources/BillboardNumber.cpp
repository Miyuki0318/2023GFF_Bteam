/*!
@file BillboardNumber.cpp
@brief ビルボード表示する数字オブジェクト
@author 小澤博貴
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	/*!
	@brief 生成時に一度だけ呼び出される関数
	*/
	void BillboardNumber::OnCreate()
	{
		// スケールの初期値は1.0
		BillboardNumber::SetScale(Vec2(1.0f));

		// メッシュの設定
		Utility::NumberVerticesIndices(m_vertex, m_number);
		auto ptrMesh = AddComponent<PCTStaticDraw>();
		ptrMesh->SetOriginalMeshUse(true);
		ptrMesh->CreateOriginalMesh(m_vertex);
		ptrMesh->SetTextureResource(L"NUMBER");

		// 透明色を表示
		SetAlphaActive(true);
	}
}