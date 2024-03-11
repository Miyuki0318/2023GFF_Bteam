/*!
@file CubeObject.h
@brief コリジョンOBBを使用するオブジェクトの継承元
*/

#pragma once
#include "TemplateObject.h"

namespace basecross
{
	/*!
	@brief キューブパラメータ群
	*/
	namespace CubeParam
	{
		//// C++17なら構造体のメンバ変数として宣言
		//// 角度配列
		//static inline const array<Vec3, 7> m_angles = {
		//	Vec3(0.0f),					// 通常
		//	Vec3(0.0f, 0.0f, 45.0f),	// 左
		//	Vec3(0.0f, 0.0f, -45.0f),	// 右
		//	Vec3(0.0f, 0.0f, 45.0f),	// スロープ左上
		//	Vec3(0.0f, 180.0f, 45.0f),	// スロープ右上
		//	Vec3(0.0f, 0.0f, 135.0f),	// スロープ左下
		//	Vec3(0.0f, 180.0f, 135.0f)	// スロープ右下
		//};

		//// スロープの座標差分
		//static inline const array<Vec3, 4> m_slopeDiff = {
		//	Vec3(0.5f, -0.5f, 0.0f),	// スロープ左上
		//	Vec3(-0.5f, -0.5f, 0.0f),	// スロープ右上
		//	Vec3(0.5f, 0.5f, 0.0f),		// スロープ左下
		//	Vec3(-0.5f, 0.5f, 0.0f)		// スロープ右下
		//};

		// キューブタイプenum
		enum class eCubeType : int8_t
		{
			Normal,		// 通常
			Left,		// 左
			Right,		// 右
			SlopeUL,	// スロープ左上
			SlopeUR,	// スロープ右上
			SlopeDL,	// スロープ左下
			SlopeDR,	// スロープ右下
		};

		/*!
		@brief ローテーション取得関数
		@param eCubeType
		@return ローテーション
		*/
		static const Vec3 GetAngle(const eCubeType& type)
		{
			switch (type)
			{
			case eCubeType::Left:
				return Vec3(0.0f, 0.0f, 45.0f);	// 左のローテーションを返す
				break;

			case eCubeType::Right:
				return Vec3(0.0f, 0.0f, -45.0f); // 右のローテーションを返す
				break;

			case eCubeType::SlopeUL:
				return Vec3(0.0f, 0.0f, 45.0f); // スロープ左上のローテーションを返す
				break;

			case eCubeType::SlopeUR:
				return Vec3(0.0f, 180.0f, 45.0f); // スロープ右上のローテーションを返す
				break;

			case eCubeType::SlopeDL:
				return Vec3(0.0f, 0.0f, 135.0f); // スロープ左下のローテーションを返す
				break;

			case eCubeType::SlopeDR:
				return Vec3(0.0f, 180.0f, 135.0f); // スロープ右下のローテーションを返す
				break;

			default:
				return Vec3(0.0f);
				break;
			}
		}

		/*!
		@brief スロープの座標差分取得関数
		@param eCubeType
		@return m_slopeDiff
		*/
		static const Vec3 GetSlopeDiff(const eCubeType& type)
		{
			switch (type)
			{
			case eCubeType::SlopeUL:
				return Vec3(0.5f, -0.5f, 0.0f); // スロープ左上の座標差分を返す
				break;

			case eCubeType::SlopeUR:
				return Vec3(-0.5f, -0.5f, 0.0f); // スロープ右上の座標差分を返す
				break;

			case eCubeType::SlopeDL:
				return Vec3(0.5f, 0.55f, 0.0f); // スロープ左下の座標差分を返す
				break;

			case eCubeType::SlopeDR:
				return Vec3(-0.5f, 0.55f, 0.0f); // スロープ右下の座標差分を返す
				break;

			default:

				// エラーメッセージを送る
				throw BaseException(
					L"スロープ以外のenum列挙子を指定しています",
					L"引数に入れるeCubeTypeの列挙子を確認してください",
					L"CubeObject.h/GetSlopeDiff(const eCubeType& type)"
				);
				break;
			}
		}
	};

	/*!
	@brief キューブオブジェクト(コリジョンOBBを使うオブジェクトの継承元)
	*/
	class CubeObject : public TemplateObject
	{
	protected:

		CubeParam::eCubeType m_type;				// キューブタイプ
		const float m_collRange;					// コリジョンアクティブ距離
		const bool m_collActive;					// コリジョンをアクティブにするか
		shared_ptr<CollisionObb> m_ptrColl;			// コリジョンOBBコンポーネント
		vector<weak_ptr<GameObject>> m_targetObj;	// コリジョン判定を取るターゲット配列

	public:

		/*!
		@brief コンストラクタ
		@param ステージポインタ
		*/
		CubeObject(const shared_ptr<Stage>& stagePtr) :
			TemplateObject(stagePtr),
			m_collRange(2.0f),
			m_collActive(true)
		{
			m_type = CubeParam::eCubeType::Normal;
		}

		/*!
		@brief コンストラクタ
		@param ステージポインタ
		@param ポジション
		@param ローテーション
		@param スケール
		@param コリジョンアクティブ
		*/
		CubeObject(const shared_ptr<Stage>& stagePtr,
			const Vec3& position, const Vec3& rotation, const Vec3& scale, const bool active
		) :
			TemplateObject(stagePtr, position, rotation, scale),
			m_collRange(2.0f),
			m_collActive(active)
		{
			m_type = CubeParam::eCubeType::Normal;
		}

		/*!
		@brief コンストラクタ
		@param ステージポインタ
		@param ポジション
		@param スケール
		@param キューブタイプ
		@param コリジョンアクティブ
		*/
		CubeObject(const shared_ptr<Stage>& stagePtr,
			const Vec3& position, const Vec3& scale, const CubeParam::eCubeType& type, const bool active
		) :
			TemplateObject(stagePtr, position, Vec3(0.0f), scale),
			m_type(type),
			m_collRange(2.0f),
			m_collActive(active)
		{
		}

		/*!
		@brief デストラクタ
		*/
		virtual ~CubeObject() {}

		/*!
		@brief 生成時に一度だけ呼び出される関数
		*/
		void OnCreate() override;

		/*!
		@brief 毎フレーム度に呼び出される関数
		*/
		void OnUpdate() override;

		/*!
		@brief コリジョンのパフォーマンス管理関数
		@param コリジョン判定距離
		*/
		void CollisionPerformance(const float range);

		/*!
		@brief コリジョンアクティブ設定関数
		@param アクティブかの真偽
		*/
		void SetCollActive(bool b)
		{
			m_ptrColl->SetUpdateActive(b);
		}

		/*!
		@brief キューブタイプ取得関数
		@return m_type
		*/
		const CubeParam::eCubeType& GetCubeType() const
		{
			return m_type;
		}

		/*!
		@brief コリジョン判定ターゲット追加関数
		@param ターゲットのポインタ
		*/
		void AddTarget(const shared_ptr<GameObject>& objPtr)
		{
			m_targetObj.push_back(objPtr);
		}

		/*!
		@brief コリジョン判定ターゲット追加関数
		@param ターゲットのポインタ配列
		*/
		void AddTarget(const vector<weak_ptr<GameObject>>& objVec)
		{
			for (const auto& ptr : objVec)
			{
				m_targetObj.push_back(ptr.lock());
			}
		}

		/*!
		@brief コリジョン判定ターゲット配列リセット関数
		*/
		void RemoveTarget()
		{
			m_targetObj.clear();
		}

		/*!
		@brief コリジョン判定ターゲット配列取得数
		@return m_targetObj
		*/
		const vector<weak_ptr<GameObject>>& GetTargetVec() const
		{
			return m_targetObj;
		}

		/*!
		@brief スロープの座標取得関数
		@return 
		*/
		const Vec3 GetSlopePos() const
		{
			// キューブタイプがスロープなのか
			if (Utility::GetBetween(m_type, CubeParam::eCubeType::SlopeUL, CubeParam::eCubeType::SlopeDR))
			{
				// スロープの座標差分を取得
				Vec3 slope = CubeParam::GetSlopeDiff(m_type);

				// 現在の座標にスロープの座標差分を加算して返す
				return (GetPosition() + slope);
			}
			else
			{
				// スロープ以外なら現在の座標をそのまま返す
				return GetPosition();
			}
		}
	};
}