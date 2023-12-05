#pragma once
#include "CubeObject.h"

namespace basecross
{
	class Alpha : public CubeObject
	{
	public:

		Alpha(const shared_ptr<Stage>& stagePtr,
			const Vec2& position, const float scale, const bool coll
		) :
			CubeObject(stagePtr, Vec3(position.x, position.y, 0.0f), Vec3(0.0f), Vec3(scale), coll)
		{
		}

		Alpha(const shared_ptr<Stage>& stagePtr,
			const Vec2& position, const Vec3& scale, const bool coll
		) :
			CubeObject(stagePtr, Vec3(position.x, position.y, 0.0f), Vec3(0.0f), scale, coll)
		{
		}

		Alpha(const shared_ptr<Stage>& stagePtr,
			const Vec2& position, const Vec3& rotation, const Vec3& scale, const bool coll
		) :
			CubeObject(stagePtr, Vec3(position.x, position.y, 0.0f), rotation, scale, coll)
		{
		}

		Alpha(const shared_ptr<Stage>& stagePtr,
			const Vec2& position, const Vec3& scale, const eType& type, const bool coll
		) :
			CubeObject(stagePtr, Vec3(position.x, position.y, 0.0f), scale, type, coll)
		{
		}

		~Alpha() {}

		void OnCreate() override;
	};

	class DeathColl : public CubeObject
	{
	public:

		DeathColl(const shared_ptr<Stage>& stagePtr,
			const Vec2& position, const float scale, const bool coll
		) :
			CubeObject(stagePtr, Vec3(position.x, position.y, 0.0f), Vec3(0.0f), Vec3(scale), coll)
		{
		}

		~DeathColl() {}

		void OnCreate() override;
	};

	class StagingColl : public CubeObject
	{
	public:

		StagingColl(const shared_ptr<Stage>& stagePtr,
			const Vec2& position, const float scale
		) :
			CubeObject(stagePtr, Vec3(position.x, position.y, 0.0f), Vec3(0.0f), Vec3(scale), true)
		{
		}

		~StagingColl() {}

		void OnCreate() override;

		void OnUpdate() override;
	};

	class InstanceBlock : public GameObject
	{
		int m_type;
		int m_size;
		int m_rowNum;
		const vector<int> m_data;
		vector<wstring> m_textures;

	public:

		InstanceBlock(const shared_ptr<Stage>& stagePtr,
			const vector<int>& data,
			const int type,
			const int size,
			const int rowNum
		) :
			GameObject(stagePtr),
			m_data(data),
			m_type(type),
			m_size(size),
			m_rowNum(rowNum)
		{
			m_textures = {
				L"GRASS_TX",
				L"DIRT_TX",
				L"ROCK_TX",
			};
		}

		virtual ~InstanceBlock() {}

		void OnCreate() override;
	};

	class InstanceSlope : public GameObject
	{
		int m_type;
		int m_size;
		int m_rowNum;
		const vector<int> m_data;
		vector<wstring> m_textures;

	public:

		InstanceSlope(const shared_ptr<Stage>& stagePtr,
			const vector<int>& data,
			const int type,
			const int size,
			const int rowNum
		) :
			GameObject(stagePtr),
			m_data(data),
			m_type(type),
			m_size(size),
			m_rowNum(rowNum)
		{
			m_textures = {
				L"GRASS_TX",
				L"DIRT_TX",
				L"ROCK_TX",
			};
		}

		virtual ~InstanceSlope() {}

		void OnCreate() override;
	};
}