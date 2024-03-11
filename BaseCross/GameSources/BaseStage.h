/*!
@file BaseStage.h
@brief ステージの継承元
*/

#pragma once
#include "Timer.h"
#include "Sprite.h"
#include "TemplateObject.h"

namespace basecross
{
	/*!
	@brief ステージの継承元
	*/
	class BaseStage : public Stage
	{
	protected:

		// CSVデータ
		vector<vector<string>> m_csvData;

		// ビュー
		shared_ptr<SingleView> m_gameView;

		// タイマーオブジェクト
		weak_ptr<Timer> m_timer;

		// BGM・SE
		weak_ptr<SoundItem> m_bgm;
		shared_ptr<SEManager> m_seManager;

		// 背景オブジェクト
		weak_ptr<TemplateObject> m_backObj;

		/*!
		@brief リソース読み込み関数
		*/
		virtual void CreateResourses();

		/*!
		@brief ビューとライトの生成関数
		*/
		virtual void CreateViewLight() = 0;

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
		@brief オブジェクトをアクティブグループに追加する関数
		@param オブジェクトのグループベクター配列
		@param アクティブグループ
		@param プレイヤーの座標
		@param 更新範囲
		*/
		void ObjectInToAvtiveGroup(const vector<weak_ptr<GameObject>>& groupVec, const shared_ptr<GameObjectGroup>& activeGroup, const Vec3& playerPos, float updateRange);

		/*!
		@brief オブジェクトのパフォーマンス管理関数
		@param オブジェクトのグループベクター配列
		@param 座標
		@param 更新範囲
		*/
		template<class T>
		void ObjectPerformance(const vector<weak_ptr<GameObject>>& groupVec, const Vec3& pos, float updateRange)
		{
			const Vec2 margin = Vec2(25.0f); // 余剰
			const Vec3 pLeft = Vec3(WINDOW_SIZE + margin, 1.0f); // 左端
			const Vec3 pRight = Vec3(-WINDOW_SIZE - margin, 0.0f); // 右端

			// オブジェクトの数ループ
			for (const auto& weakObj : groupVec)
			{
				// エラーチェック
				if (!weakObj.lock()) continue;

				// 型キャスト
				const auto& sharedObj = dynamic_pointer_cast<T>(weakObj.lock());
				if (!sharedObj) continue;

				// 座標とオブジェクトの座標の距離が更新範囲内であるか
				const Vec3& objPos = sharedObj->GetPosition();
				float length = (objPos - pos).length();

				// 範囲内かで更新処理をアクティブにするか設定
				sharedObj->SetUpdateActive(length <= updateRange);

				// 2D座標に変換し、画面内にオブジェクトがあれば描画を行う
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
			// グループからオブジェクト配列を取得
			const auto& grouoVec = group->GetGroupVector();

			// オブジェクトの数ループ
			for (auto& weakObj : grouoVec)
			{
				// エラーチェック
				if (!weakObj.lock()) continue;

				// ステージ側に破棄命令を送る
				RemoveGameObject<GameObject>(weakObj.lock());

				// グループから除外する
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
		@param BGMキー
		@param 音量
		*/
		virtual void CreateBGM(const wstring& bgmKey, float volume);

		/*!
		@brief SEマネージャーの生成関数
		*/
		virtual void CreateSEManager();

		/*!
		@brief SEの再生関数
		@param SEキー
		@param 音量
		*/
		virtual void CreateSE(const wstring& seKey, float volume);

		/*!
		@brief SEの再生関数
		@param SEキー
		@param 音量
		@param オブジェクトのポインタ
		*/
		virtual void CreateSE(const wstring& seKey, float volume, const void* objPtr);

		/*!
		@brief SEの停止関数
		@param SEキー
		*/
		virtual void StopSE(const wstring& seKey);

		/*!
		@brief SEの停止関数
		@param SEキー
		@param オブジェクトのポインタ
		*/
		virtual void StopSE(const wstring& seKey, const void* objPtr);

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
		const shared_ptr<Timer> GetTimer() const
		{
			return m_timer.lock();
		}
	};
}