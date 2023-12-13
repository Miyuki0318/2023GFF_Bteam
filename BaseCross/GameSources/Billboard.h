/*!
@file Billboard.h
@brief ビルボードオブジェクトの継承元
@author 小澤博貴
*/

#pragma once
#include "stdafx.h"
#include "DrawObject.h"

namespace basecross
{
	// ----------------------------------------------------------------------------------
	//	Billboard : public GameObject ビルボードオブジェクトの継承元
	// ----------------------------------------------------------------------------------
	class Billboard : public DrawObject
	{
	protected:

		shared_ptr<PCTStaticDraw> m_ptrDraw;

	public:

		/*!
		@brief コンストラクタ
		@param ステージポインタ
		*/
		Billboard(const shared_ptr<Stage>& stagePtr) :
			DrawObject(stagePtr)
		{
		}

		/*!
		@brief コンストラクタ
		@param ステージポインタ
		@param テクスチャキー
		@param スケール
		@param ポジション
		*/
		Billboard(const shared_ptr<Stage>& stagePtr,
			const wstring& texWstr, const Vec2& scale, const Vec3& position
			) :
			DrawObject(stagePtr, texWstr, position, Vec3(0.0f), scale)
		{
		}

		/*!
		@brief コンストラクタ
		@param ステージポインタ
		@param テクスチャキー
		@param スケール
		@param ポジション
		@param ローテーション
		*/
		Billboard(const shared_ptr<Stage>& stagePtr,
			const wstring& texWstr, const Vec2& scale,
			const Vec3& position, const Vec3& rotation
			) :
			DrawObject(stagePtr, texWstr, position, rotation, scale)
		{
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
		@brief テクスチャを変更する関数
		@param テクスチャキー
		*/
		void SetTexture(const wstring& textureKey);

		/*!
		@brief テクスチャをベロシティに応じて回転させる関数
		*/
		void RotateTexture();
	};
}