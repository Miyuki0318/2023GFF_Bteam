/*!
@file Cannon.h
@brief ステージギミックの大砲
*/

#pragma once
#include "Gimmick.h"

namespace basecross
{
	/*!
	@brief 大砲のパラメータ
	*/
	namespace CannonParam
	{
		// 大砲の種類enum
		enum class eFireType : int8_t
		{
			Normal,	 // 通常
			Rapid,	 // 速射
			NRotate, // 通常+回転
			RRotate, // 速射+回転
		};

		// 難易度選択用大砲タイプ
		enum class eDifficulty : int8_t
		{
			Easy,	// イージー
			Normal, // ノーマル
			Hard,   // ハード
		};

		// 大砲の状態enum
		enum class eStatus : uint8_t
		{
			IsFire,	  // 発射したか
			IsRotate, // 回転中か
		};

		/*!
		@brief 大砲の発射時間取得関数
		@param eFireType
		@return 発射時間
		*/
		static float GetFireTime(const eFireType fireType)
		{
			// 大砲の種類で分岐
			switch (fireType)
			{
			case eFireType::Normal:  // 通常タイプ
			case eFireType::NRotate: // 通常+回転タイプ
				return 1.4f; // 通常タイプの発射時間を返す
				break;

			case eFireType::Rapid:   // 速射タイプ
			case eFireType::RRotate: // 速射+回転タイプ
				return 0.4f; // 速射タイプの発射時間を返す
				break;

			default:
				return 0.0f;
				break;
			}
		}

		/*!
		@brief 大砲のアニメーションキー取得関数
		@param eFireType
		@return アニメーションキー
		*/
		static wstring GetAnimeKey(const eFireType fireType)
		{
			// 大砲の種類で分岐
			switch (fireType)
			{
			case eFireType::Normal:  // 通常タイプ
			case eFireType::NRotate: // 通常+回転タイプ
				return L"FIRE"; // 通常タイプのアニメーションキーを返す
				break;

			case eFireType::Rapid:   // 速射タイプ
			case eFireType::RRotate: // 速射+回転タイプ
				return  L"RAPID"; // 速射タイプのアニメーションキーを返す
				break;

			default:
				return L"";
				break;
			}
		}
	}

	/*!
	@brief 大砲
	*/
	class Cannon : public Gimmick
	{
	protected:

		float m_currentRot; // 前フレームまでの回転角
		Bool8_t<CannonParam::eStatus> m_status; // 状態真偽

		weak_ptr<MultiParticle> m_particle; // パーティクルオブジェクト
		shared_ptr<PNTBoneModelDraw> m_ptrDraw; // 描画コンポーネント

		const float m_particleTime; // パーティクルの再生時間
		const CannonParam::eFireType m_fireType; // 大砲の種類

	public:

		/*!
		@brief コンストラクタ
		@param ステージポインタ
		@param ポジション
		@param スケール
		@param 角度
		@param 大砲タイプ
		*/
		Cannon(const shared_ptr<Stage>& stagePtr,
			const Vec2& position, const float scale, 
			const GimmickAngle::eAngle& angle,
			const CannonParam::eFireType& fire
		) :
			Gimmick(stagePtr, Vec3(position.x, position.y, 0.0f), Vec3(scale), angle),
			m_fireType(fire),
			m_particleTime(0.04f)
		{
			m_currentRot = 0.0f;
			m_modelMat.affineTransformation(
				Vec3(1.0f),
				Vec3(0.0f),
				Vec3(0.0f, XM_PI, 0.0f),
				Vec3(0.0f, -1.0f, 0.0f)
			);
		}

		/*!
		@brief デストラクタ
		*/
		virtual ~Cannon() {}

		/*!
		@brief 生成時に一度だけ呼び出される関数
		*/
		virtual void OnCreate() override;

		/*!
		@brief 毎フレーム度に呼び出される関数
		*/
		void OnUpdate() override;

		/*!
		@brief 大砲の回転関数
		*/
		void RotateCannon();

		/*!
		@brief 大砲発射時のアニメーション実行関数
		*/
		void FireAnimation();

		/*!
		@brief エフェクト更新関数
		*/
		virtual void EffectUpdate();

		/*!
		@brief 大砲発射関数
		*/
		void OnFire()
		{
			m_status.Set(CannonParam::eStatus::IsFire) = true; // 発射してるかの真偽をオン

			// アニメーションを設定
			m_ptrDraw->ChangeCurrentAnimation(CannonParam::GetAnimeKey(m_fireType), 0.0f);
		}

		/*!
		@brief 大砲が発射してるかの真偽取得関数
		@return 大砲が発射中かの真偽
		*/
		bool GetFire()
		{
			return m_status(CannonParam::eStatus::IsFire);
		}

		/*!
		@brief 大砲の発射時間取得関数
		@return 大砲の発射時間(s)
		*/
		const float GetFireTime() const
		{
			return CannonParam::GetFireTime(m_fireType);
		}

		/*!
		@brief 大砲のアニメーション経過時間取得関数
		@return アニメーション経過時間
		*/
		const float GetAnimationTime() const
		{
			return m_ptrDraw->GetCurrentAnimationTime();
		}
	};

	/*!
	@brief ゴール用大砲
	*/
	class GoalCannon : public Cannon
	{
	public:

		/*!
		@brief コンストラクタ
		@param ステージポインタ
		@param ポジション
		@param スケール
		*/
		GoalCannon(const shared_ptr<Stage>& stagePtr,
			const Vec2& position, const float scale
		) :
			Cannon(stagePtr, position, scale, GimmickAngle::eAngle::Uright, CannonParam::eFireType::Normal)
		{
		}

		/*!
		@brief デストラクタ
		*/
		virtual ~GoalCannon() {}

		/*!
		@brief 生成時に一度だけ呼び出される関数
		*/
		void OnCreate() override;

		/*!
		@brief エフェクト更新関数
		*/
		void EffectUpdate() override;
	};

	/*!
	@brief タイトル画面用大砲
	*/
	class TitleCannon : public Cannon
	{
	private:

		// 大砲の難易度タイプ
		const CannonParam::eDifficulty m_diffType;

	public:

		/*!
		@brief コンストラクタ
		@param ステージポインタ
		@param ポジション
		@param スケール
		@param 難易度
		*/
		TitleCannon(const shared_ptr<Stage>& stagePtr,
			const Vec2& position, const float scale,
			const CannonParam::eDifficulty& diffType
		) :
			Cannon(stagePtr, position, scale, GimmickAngle::eAngle::Up, CannonParam::eFireType::Normal),
			m_diffType(diffType)
		{
		}

		/*!
		@brief デストラクタ
		*/
		virtual ~TitleCannon() {}

		/*!
		@brief 生成時に一度だけ呼び出される関数
		*/
		void OnCreate() override;
	};
}