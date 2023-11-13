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

	class Slope : public CubeObject
	{
	public:

		enum eBlock
		{
			Iron,
			Metal,
			DarkMetal,
		};

	private:

		eBlock m_block;
		Mat4x4 m_modelMat;

	public:

		Slope(const shared_ptr<Stage>& stagePtr, const Vec2& position,
			const Vec3& scale, const eBlock block, const eType type, const bool coll
		) :
			CubeObject(stagePtr, Vec3(position.x, position.y, 0.0f), scale, type, coll),
			m_block(block)
		{
			m_modelMat.affineTransformation(
				Vec3(1.0f),
				Vec3(0.0f),
				Vec3(0.0f, 0.0f, 0.0f),
				Vec3(0.0f, 0.0f, 0.0f)
			);
		}

		~Slope() {}

		void OnCreate() override;
	};

	class InstanceBlock : public GameObject
	{
		int m_size;
		int m_rowNum;
		wstring m_texture;
		vector<int> m_data;

	public:

		InstanceBlock(const shared_ptr<Stage>& stagePtr,
			const vector<int>& data,
			const wstring& texture,
			const int size,
			const int rowNum
		) :
			GameObject(stagePtr),
			m_data(data),
			m_texture(texture),
			m_size(size),
			m_rowNum(rowNum)
		{
		}

		virtual ~InstanceBlock() {}

		void OnCreate() override;
	};
}