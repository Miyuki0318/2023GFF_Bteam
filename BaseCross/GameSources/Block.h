#pragma once
#include "CubeObject.h"

namespace basecross
{
	class Alpha : public CubeObject
	{
	public:

		Alpha(const shared_ptr<Stage>& stagePtr,
			const Vec2& position, const float scale
		) :
			CubeObject(stagePtr, Vec3(position.x, position.y, 0.0f), Vec3(0.0f), Vec3(scale))
		{
		}

		~Alpha() {}

		void OnCreate() override;
	};

	class Grass : public CubeObject
	{
		Mat4x4 m_bodyMat;

	public:

		Grass(const shared_ptr<Stage>& stagePtr,
			const Vec2& position, const float scale
		) :
			CubeObject(stagePtr, Vec3(position.x, position.y, 0.0f), Vec3(0.0f), Vec3(scale))
		{
			m_bodyMat.affineTransformation(
				Vec3(1.35f),
				Vec3(0.0f),
				Vec3(0.0f),
				Vec3(0.0f, -0.5f, 0.0f)
			);
		}

		~Grass() {}

		void OnCreate() override;
	};

	class Dirt : public CubeObject
	{
		Mat4x4 m_bodyMat;

	public:

		Dirt(const shared_ptr<Stage>& stagePtr,
			const Vec2& position, const float scale
		) :
			CubeObject(stagePtr, Vec3(position.x, position.y, 0.0f), Vec3(0.0f), Vec3(scale))
		{
			m_bodyMat.affineTransformation(
				Vec3(1.35f),
				Vec3(0.0f),
				Vec3(0.0f),
				Vec3(0.0f, -0.5f, 0.0f)
			);
		}

		~Dirt() {}

		void OnCreate() override;
	};

	class Rock : public CubeObject
	{
		Mat4x4 m_bodyMat;

	public:

		Rock(const shared_ptr<Stage>& stagePtr,
			const Vec2& position, const float scale
		) :
			CubeObject(stagePtr, Vec3(position.x, position.y, 0.0f), Vec3(0.0f), Vec3(scale))
		{
			m_bodyMat.affineTransformation(
				Vec3(1.35f),
				Vec3(0.0f),
				Vec3(0.0f),
				Vec3(0.0f, -0.5f, 0.0f)
			);
		}

		~Rock() {}

		void OnCreate() override;
	};

	class SandStone : public CubeObject
	{
		Mat4x4 m_bodyMat;

	public:

		SandStone(const shared_ptr<Stage>& stagePtr,
			const Vec2& position, const float scale
		) :
			CubeObject(stagePtr, Vec3(position.x, position.y, 0.0f), Vec3(0.0f), Vec3(scale))
		{
			m_bodyMat.affineTransformation(
				Vec3(1.35f),
				Vec3(0.0f),
				Vec3(0.0f),
				Vec3(0.0f, -0.5f, 0.0f)
			);
		}

		~SandStone() {}

		void OnCreate() override;
	};
}