/*!
@file block.h
@brief ブロックのコリジョンと描画
*/

#pragma once
#include "CubeObject.h"

namespace basecross
{
	/*!
	@brief 透明のコリジョンオブジェクト
	*/
	class Alpha : public CubeObject
	{
	public:

		/*!
		@brief コンストラクタ
		@param ステージポインタ
		@param ポジション
		@param スケール
		*/
		Alpha(const shared_ptr<Stage>& stagePtr,
			const Vec2& position, const float scale
		) :
			CubeObject(stagePtr, Vec3(position.x, position.y, 0.0f), Vec3(0.0f), Vec3(scale), true)
		{
		}

		/*!
		@brief コンストラクタ
		@param ステージポインタ
		@param ポジション
		@param スケール
		@param タイプ
		*/
		Alpha(const shared_ptr<Stage>& stagePtr,
			const Vec2& position, const Vec3& scale, const CubeParam::eCubeType& type
		) :
			CubeObject(stagePtr, Vec3(position.x, position.y, 0.0f), scale, type, true)
		{
		}

		/*!
		@brief デストラクタ
		*/
		virtual ~Alpha() {}

		/*!
		@brief 生成時に一度だけ呼び出される関数
		*/
		void OnCreate() override;
	};

	/*!
	@brief 衝突したら死亡させる用のコリジョン
	*/
	class DeathColl : public CubeObject
	{
	public:

		/*!
		@brief コンストラクタ
		@param ステージポインタ
		@param ポジション
		@param スケール
		@param コリジョンの有無
		*/
		DeathColl(const shared_ptr<Stage>& stagePtr,
			const Vec2& position, const float scale
		) :
			CubeObject(stagePtr, Vec3(position.x, position.y, 0.0f), Vec3(0.0f), Vec3(scale), true)
		{
		}

		/*!
		@brief デストラクタ
		*/
		virtual ~DeathColl() {}

		/*!
		@brief 生成時に一度だけ呼び出される関数
		*/
		void OnCreate() override;
	};

	/*!
	@brief 演出時は衝突処理を行わないコリジョン
	*/
	class StagingColl : public CubeObject
	{
	public:

		/*!
		@brief コンストラクタ
		@param ステージポインタ
		@param ポジション
		@param スケール
		*/
		StagingColl(const shared_ptr<Stage>& stagePtr,
			const Vec2& position, const float scale
		) :
			CubeObject(stagePtr, Vec3(position.x, position.y, 0.0f), Vec3(0.0f), Vec3(scale), true)
		{
		}

		/*!
		@brief デストラクタ
		*/
		virtual ~StagingColl() {}

		/*!
		@brief 生成時に一度だけ呼び出される関数
		*/
		void OnCreate() override;

		/*!
		@brief 毎フレーム度に呼び出される関数
		*/
		void OnUpdate() override;
	};

	/*!
	@brief ブロックのインスタンス描画
	*/
	class InstanceBlock : public GameObject
	{
	public:

		// テクスチャタイプenum
		enum eTextureType : size_t
		{
			Iron,	// 鉄
			Metal,	// 鋼
			Dark,	// 黒鋼
			Size,	// enumのサイズ指定子
		};

	protected:

		const int m_size;			// CSVの行のサイズ
		const int m_rowNum;			// CSVの行の数値
		const eTextureType m_type;	// テクスチャタイプ
		const vector<int> m_data;	// ブロックの値のみ保持した配列

		// テクスチャ配列
		const array<wstring, static_cast<size_t>(eTextureType::Size)> m_textures;

	public:

		/*!
		@brief コンストラクタ
		@param ステージポインタ
		@param ブロックの値のみ保持した配列
		@param テクスチャタイプ
		@param CSVの行のサイズ
		@param CSVの行の数値
		*/
		InstanceBlock(const shared_ptr<Stage>& stagePtr,
			const vector<int>& data,
			const eTextureType type,
			const int size,
			const int rowNum
		) :
			GameObject(stagePtr),
			m_data(data),
			m_type(type),
			m_size(size),
			m_rowNum(rowNum),
			m_textures({ 
				L"GRASS_TX",
				L"DIRT_TX",
				L"ROCK_TX",
			})
		{
		}

		/*!
		@brief デストラクタ
		*/
		virtual ~InstanceBlock() {}

		/*!
		@brief 生成時に一度だけ呼び出される関数
		*/
		virtual void OnCreate() override;
	};

	/*!
	@brief スロープのインスタンス描画
	*/
	class InstanceSlope : public InstanceBlock
	{
	public:

		/*!
		@brief コンストラクタ
		@param ステージポインタ
		@param ブロックの値のみ保持した配列
		@param テクスチャタイプ
		@param CSVの行のサイズ
		@param CSVの行の数値
		*/
		InstanceSlope(const shared_ptr<Stage>& stagePtr,
			const vector<int>& data,
			const eTextureType type,
			const int size,
			const int rowNum
		) :
			InstanceBlock(stagePtr, data, type, size, rowNum)
		{
		}

		/*!
		@brief デストラクタ
		*/
		virtual ~InstanceSlope() {}

		/*!
		@brief 生成時に一度だけ呼び出される関数
		*/
		void OnCreate() override;
	};
}