/*!
@file Sprite.h
@brief スプライト親クラス
*/

#pragma once
#include "stdafx.h"
#include "DrawObject.h"

namespace basecross
{
	// ----------------------------------------------------------------------------------
	//	Sprite : public GameObject 単純な表示を行うスプライト親クラス
	// ----------------------------------------------------------------------------------
	class Sprite : public DrawObject
	{
	protected:

		shared_ptr<PCTSpriteDraw> m_ptrDraw; // 描画コンポーネント

	public:

		/*!
		@brief コンストラクタ
		@param ステージポインタ
		*/
		Sprite(const shared_ptr<Stage>& stagePtr) :
			DrawObject(stagePtr)
		{
		}

		/*!
		@brief コンストラクタ
		@param ステージポインタ
		@param テクスチャキー
		@param 透明色の描画
		@param スケール
		@param ポジション
		*/
		Sprite(const shared_ptr<Stage>& stagePtr,
			const wstring& texWstr, const Vec2& scale, 
			const Vec3& position
		) :
			DrawObject(stagePtr, texWstr, position, Vec3(0.0f), scale)
		{
		}

		/*!
		@brief コンストラクタ
		@param ステージポインタ
		@param テクスチャキー
		@param 透明色の描画
		@param スケール
		@param ポジション
		@param ローテーションZ
		*/
		Sprite(const shared_ptr<Stage>& stagePtr,
			const wstring& texWstr,	const Vec2& scale, 
			const Vec3& position, const float rotZ
		) :
			DrawObject(stagePtr, texWstr, position, Vec3(0.0f, 0.0f, rotZ), scale)
		{
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
		@brief テクスチャを変更する関数
		@param テクスチャキー
		*/
		void SetTexture(const wstring& textureKey);

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
		@brief テクスチャをベロシティに応じて回転させる関数
		*/
		void RotateTexture();
	};
}