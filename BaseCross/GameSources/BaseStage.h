#pragma once
#include "stdafx.h"
#include "Sprite.h"
#include "Timer.h"
#include "TemplateObject.h"

namespace basecross
{
	class BaseStage : public Stage
	{
	protected:

		// SE管理構造体
		struct SE
		{
			weak_ptr<SoundItem> item; // サウンドアイテム
			wstring seKey = L""; // ファイルキー

			// コンストラクタ
			SE(const weak_ptr<SoundItem>& ptr, const wstring& key
			) :
				item(ptr),
				seKey(key)
			{
			}

			// コンストラクタ
			SE(const wstring& key, const weak_ptr<SoundItem>& ptr
			) :
				item(ptr),
				seKey(key)
			{
			}

		public:

			// リセット関数
			void Reset()
			{
				item.reset();
				seKey = L"";
			}
		};

		// CSVデータ
		vector<vector<string>> m_csvData;

		// ビュー
		shared_ptr<SingleView> m_gameView;

		// タイマーオブジェクト
		shared_ptr<Timer> m_timer;

		// BGM・SE
		weak_ptr<SoundItem> m_bgm;
		vector<SE> m_seList;

		// 背景オブジェクト
		weak_ptr<TemplateObject> m_backObj;

		/*!
		@brief リソース読み込み関数
		*/
		virtual void CreateResourses();

		/*!
		@brief ビューとライトの生成関数
		*/
		virtual void CreateViewLight();

		/*!
		@brief プレイヤーの生成関数
		*/
		virtual void CreatePlayer();

		/*!
		@brief ステージの生成関数
		@param ファイルネーム
		*/
		void CreateStage(const string& fileName);
		void CreateEnemy(const string& fileName);
		void CreateInstanceBlock(const string& fileName);

		/*!
		@brief オブジェクトのアクティブグループに追加する関数
		@param オブジェクトのグループベクター配列
		@param プレイヤーの座標
		@param 更新範囲
		*/
		void ObjectInToAvtiveGroup(const vector<weak_ptr<GameObject>>& groupVec, const shared_ptr<GameObjectGroup>& activeGroup, const Vec3& playerPos, float updateRange);

		/*!
		@brief オブジェクトのパフォーマンス管理関数
		@param オブジェクトのグループベクター配列
		@param 更新範囲
		*/
		template<class T>
		void ObjectPerformance(const vector<weak_ptr<GameObject>>& groupVec, const Vec3& pos, float updateRange)
		{
			const Vec2 margin = Vec2(25.0f);
			const Vec3 pLeft = Vec3(WINDOW_SIZE + margin, 1.0f);
			const Vec3 pRight = Vec3(-WINDOW_SIZE - margin, 0.0f);

			for (const auto& weakObj : groupVec)
			{
				if (!weakObj.lock()) continue;

				const auto& sharedObj = dynamic_pointer_cast<T>(weakObj.lock());
				if (!sharedObj) continue;

				const Vec3& objPos = sharedObj->GetPosition();
				float length = (objPos - pos).length();
				sharedObj->SetUpdateActive(length <= updateRange);

				Vec3 point = Utility::ConvertToWorldPosition(m_gameView, objPos);
				sharedObj->SetDrawActive(Utility::GetBetween(point, pLeft, pRight));
			}
		}

		/*!
		@brief オブジェクトグループの中身を空っぽにする関数
		@param オブジェクトのグループ
		*/
		void GroupObjectRemove(const shared_ptr<GameObjectGroup>& group)
		{
			const auto& grouoVec = group->GetGroupVector();
			for (auto& weakObj : grouoVec)
			{
				if (!weakObj.lock()) continue;

				RemoveGameObject<GameObject>(weakObj.lock());
				group->OutoGroup(weakObj.lock());
			}
		}

	public:

		/*!
		@brief コンストラクタ
		*/
		BaseStage() :Stage() {}

		/*!
		@brief デストラクタ
		*/
		virtual ~BaseStage() {}

		/*!
		@brief 生成時に一度だけ呼び出される関数
		*/
		virtual void OnCreate() override;

		/*!
		@brief 生成時に一度だけ呼び出される関数
		*/
		virtual void OnDestroy() override;

		/*!
		@brief 毎フレーム度に呼び出される関数
		*/
		virtual void OnUpdate() override;

		/*!
		@brief 描画更新関数
		*/
		virtual void OnDraw() override;

		/*!
		@brief BGMの再生関数
		*/
		virtual void CreateBGM(const wstring& bgmKey, float volume);

		/*!
		@brief SEの再生関数
		*/
		virtual void CreateSE(const wstring& seKey, float volume);

		/*!
		@brief SEの再生関数
		*/
		virtual void StopSE(const wstring& seKey);

		/*!
		@brief ゲームビュー取得関数
		@return const shared_ptr<SingleView>
		*/
		const shared_ptr<SingleView>& GetGameView() const
		{
			return m_gameView;
		}

		/*!
		@brief タイマークラス取得関数
		@return const shared_ptr<Timer>
		*/
		const shared_ptr<Timer>& GetTimer() const
		{
			return m_timer;
		}
	};
}