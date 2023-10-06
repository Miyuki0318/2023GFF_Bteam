/*!
@file Params.h
@brief ���p�������ȃf�[�^�^�\���̂̒�`
@author ���V���M
*/

#pragma once
#include "stdafx.h"

using namespace basecross;

namespace Params {

	/*!
	@brief Transform��Vec3�O��
	@param position
	@param rotation
	@param scale
	*/
	struct TransData
	{
		bsm::Vec3 position;	// ���W
		bsm::Vec3 rotation;	// ����
		bsm::Vec3 scale;		// �K��

		TransData()
		{
			position.zero();
			rotation.zero();
			scale.zero();
		}

		TransData(const bsm::Vec3& pos, const bsm::Vec3& rot, const bsm::Vec3& sc)
		{
			position = pos;
			rotation = rot;
			scale = sc;
		}
	};

	/*!
	@brief ���_�f�[�^
	@param vertices
	@param indices
	*/
	struct VertexData
	{
		vector<VertexPositionColorTexture> vertices; // ���_
		vector<uint16_t> indices; // �C���f�b�N�X
	};
}