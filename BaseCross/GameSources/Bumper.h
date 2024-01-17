#pragma once
#include "stdafx.h"
#include "TemplateObject.h"

namespace basecross
{
	class Bumper : public TemplateObject
	{
		vector<weak_ptr<GameObject>> m_targetObj;

		shared_ptr<CollisionSphere> m_ptrColl;
		shared_ptr<PNTBoneModelDraw> m_ptrDraw;
		Mat4x4 m_modelMat;

	public:

		Bumper(const shared_ptr<Stage>& stagePtr,
			const Vec2& position, float scale
		) :
			TemplateObject(stagePtr, Vec3(position), Vec3(0.0f), Vec3(scale))
		{
			m_modelMat.affineTransformation(
				Vec3(1.35f),
				Vec3(0.0f),
				Vec3(-XM_PIDIV2, 0.0f, 0.0f),
				Vec3(0.0f, 0.0f, 0.5f)
			);
		}

		/*!
		@brief �f�X�g���N�^
		*/
		virtual ~Bumper() {}

		/*!
		@brief �������Ɉ�x�����Ăяo�����֐�
		*/
		void OnCreate() override;

		/*!
		@brief ���t���[���x�ɌĂяo�����֐�
		*/
		void OnUpdate() override;

		/*!
		@brief �Փˎ��ɃA�j���[�V���������Z�b�g����֐�
		*/
		void OnHit()
		{
			StartSE(L"BUMPER_SE", 1.0f);
			m_ptrDraw->ChangeCurrentAnimation(L"PUSH");
		}
	};
}