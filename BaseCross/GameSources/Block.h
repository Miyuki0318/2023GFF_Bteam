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

	class Block : public CubeObject
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
		Mat4x4 m_normalMat;
		Mat4x4 m_slopeMat;

	public:

		Block(const shared_ptr<Stage>& stagePtr, const Vec2& position,
			const Vec3& scale, const eBlock block, const eType type, const bool coll
		) :
			CubeObject(stagePtr, Vec3(position.x, position.y, 0.0f), scale, type, coll),
			m_block(block)
		{
			m_normalMat.affineTransformation(
				Vec3(1.35f),
				Vec3(0.0f),
				Vec3(0.0f),
				Vec3(0.0f, -0.5f, 0.0f)
			);

			m_slopeMat.affineTransformation(
				Vec3(1.0f),
				Vec3(0.0f),
				Utility::DegToRad(Vec3(0.0f, 0.0f, 0.0f)),
				Vec3(0.0f, 0.0f, 0.0f)
			);
		}

		~Block() {}

		void OnCreate() override;
	};
}