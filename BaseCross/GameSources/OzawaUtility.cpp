/*!
@file OzawaUtility.cpp
@brief ���[�e�B���e�B
@author ���V���M
*/

#include "stdafx.h"
#include "OzawaUtility.h"

namespace Utility
{
	/*!
	@brief �V���v���Ȓ��_�f�[�^�ƒ��_�C���f�b�N�X���쐬
	@param (vertices) ���_�f�[�^
	@param (indices) ���_�C���f�b�N�X
	*/
	void SimpleVerticesIndices(vector<VertexPositionColorTexture>& vertices, vector<uint16_t>& indices)
	{
		// �����̃T�C�Y�ƃf�t�H���g�̐F
		const float HALF_SIZE = 0.5f;
		const Col4 DEFAULT_COLOR = Col4(1.0f);

		// ���_�f�[�^
		vertices = {
			{ Vec3(-HALF_SIZE, HALF_SIZE, 0.0f), DEFAULT_COLOR, Vec2(0.0f, 0.0f)},
			{ Vec3(HALF_SIZE, HALF_SIZE, 0.0f), DEFAULT_COLOR , Vec2(1.0f, 0.0f)},
			{ Vec3(-HALF_SIZE, -HALF_SIZE, 0.0f), DEFAULT_COLOR, Vec2(0.0f, 1.0f)},
			{ Vec3(HALF_SIZE, -HALF_SIZE, 0.0f), DEFAULT_COLOR, Vec2(1.0f, 1.0f)},
		};

		// ���_�̕��я�
		indices = {
			0, 1, 2,
			1, 3, 2
		};
	}

	/*!
	@brief �V���v���Ȓ��_�f�[�^�ƒ��_�C���f�b�N�X���쐬
	@param (vertices) ���_�f�[�^
	@param (indices) ���_�C���f�b�N�X
	*/
	void SimpleVerticesIndices(VertexData& vertex)
	{
		// �����̃T�C�Y�ƃf�t�H���g�̐F
		const float HALF_SIZE = 0.5f;
		const Col4 DEFAULT_COLOR = Col4(1.0f);

		// ���_�f�[�^
		vertex.vertices = {
			{ Vec3(-HALF_SIZE, HALF_SIZE, 0.0f), DEFAULT_COLOR, Vec2(0.0f, 0.0f)},
			{ Vec3(HALF_SIZE, HALF_SIZE, 0.0f), DEFAULT_COLOR , Vec2(1.0f, 0.0f)},
			{ Vec3(-HALF_SIZE, -HALF_SIZE, 0.0f), DEFAULT_COLOR, Vec2(0.0f, 1.0f)},
			{ Vec3(HALF_SIZE, -HALF_SIZE, 0.0f), DEFAULT_COLOR, Vec2(1.0f, 1.0f)},
		};

		// ���_�̕��я�
		vertex.indices = {
			0, 1, 2,
			1, 3, 2
		};
	}

	/*!
	@brief �V���v���Ȓ��_�f�[�^�ƒ��_�C���f�b�N�X���쐬
	@param (vertices) ���_�f�[�^
	@param (indices) ���_�C���f�b�N�X
	@param (color) �Œ�F
	*/
	void SimpleVerticesIndices(vector<VertexPositionColorTexture>& vertices, vector<uint16_t>& indices, const Col4& color)
	{
		// �����̃T�C�Y�ƃf�t�H���g�̐F
		const float HELF_SIZE = 0.5f;
		const Col4 DEFAULT_COLOR = color;

		// ���_�f�[�^
		vertices = {
			{ Vec3(-HELF_SIZE, HELF_SIZE, 0), DEFAULT_COLOR, Vec2(0.0f, 0.0f)},
			{ Vec3(HELF_SIZE, HELF_SIZE, 0), DEFAULT_COLOR , Vec2(1.0f, 0.0f)},
			{ Vec3(-HELF_SIZE, -HELF_SIZE, 0), DEFAULT_COLOR, Vec2(0.0f, 1.0f)},
			{ Vec3(HELF_SIZE, -HELF_SIZE, 0), DEFAULT_COLOR, Vec2(1.0f, 1.0f)},
		};

		// ���_�̕��я�
		indices = {
			0, 1, 2,
			1, 3, 2
		};
	}

	/*!
	@brief �V���v���Ȓ��_�f�[�^�ƒ��_�C���f�b�N�X���쐬
	@param (vertices) ���_�f�[�^
	@param (indices) ���_�C���f�b�N�X
	*/
	void SimpleVerticesIndices(VertexData& vertex, const Col4& color)
	{
		// �����̃T�C�Y�ƃf�t�H���g�̐F
		const float HALF_SIZE = 0.5f;
		const Col4 DEFAULT_COLOR = color;

		// ���_�f�[�^
		vertex.vertices = {
			{ Vec3(-HALF_SIZE, HALF_SIZE, 0.0f), DEFAULT_COLOR, Vec2(0.0f, 0.0f)},
			{ Vec3(HALF_SIZE, HALF_SIZE, 0.0f), DEFAULT_COLOR , Vec2(1.0f, 0.0f)},
			{ Vec3(-HALF_SIZE, -HALF_SIZE, 0.0f), DEFAULT_COLOR, Vec2(0.0f, 1.0f)},
			{ Vec3(HALF_SIZE, -HALF_SIZE, 0.0f), DEFAULT_COLOR, Vec2(1.0f, 1.0f)},
		};

		// ���_�̕��я�
		vertex.indices = {
			0, 1, 2,
			1, 3, 2
		};
	}

	/*!
	@brief �����p���_�f�[�^�ƒ��_�C���f�b�N�X���쐬
	@param (vertices) ���_�f�[�^
	@param (indices) ���_�C���f�b�N�X
	@param (number) ����
	*/
	void NumberVerticesIndices(vector<VertexPositionColorTexture>& vertices, vector<uint16_t>& indices, const int number)
	{
		// �����̃T�C�Y�ƃf�t�H���g�̐F
		const float HALF_SIZE = 0.5f;
		const float UV_LEFT = 1.0f / 10.0f * number;
		const float UV_RIGHT = 1.0f / 10.0f * (number + 1);
		const Col4 DEFAULT_COLOR = Col4(1.0f);

		// ���_�f�[�^
		vertices = {
			{ Vec3(-HALF_SIZE, HALF_SIZE, 0.0f), DEFAULT_COLOR, Vec2(UV_LEFT, 0.0f)},
			{ Vec3(HALF_SIZE, HALF_SIZE, 0.0f), DEFAULT_COLOR , Vec2(UV_LEFT, 0.0f)},
			{ Vec3(-HALF_SIZE, -HALF_SIZE, 0.0f), DEFAULT_COLOR, Vec2(UV_RIGHT, 1.0f)},
			{ Vec3(HALF_SIZE, -HALF_SIZE, 0.0f), DEFAULT_COLOR, Vec2(UV_RIGHT, 1.0f)},
		};

		// ���_�̕��я�
		indices = {
			0, 1, 2,
			1, 3, 2
		};
	}

	/*!
	@brief �����p���_�f�[�^�ƒ��_�C���f�b�N�X���쐬
	@param (vertex) ���_�f�[�^�ƒ��_�C���f�b�N�X�\����
	@param (number) ����
	*/
	void NumberVerticesIndices(VertexData& vertex, const int number)
	{
		// �����̃T�C�Y�ƃf�t�H���g�̐F
		const float HALF_SIZE = 0.5f;
		const float UV_LEFT = 1.0f / 10.0f * number;
		const float UV_RIGHT = 1.0f / 10.0f * (number + 1);
		const Col4 DEFAULT_COLOR = Col4(1.0f);

		// ���_�f�[�^
		vertex.vertices = {
			{ Vec3(-HALF_SIZE, HALF_SIZE, 0.0f), DEFAULT_COLOR, Vec2(UV_LEFT, 0.0f)},
			{ Vec3(HALF_SIZE, HALF_SIZE, 0.0f), DEFAULT_COLOR , Vec2(UV_LEFT, 0.0f)},
			{ Vec3(-HALF_SIZE, -HALF_SIZE, 0.0f), DEFAULT_COLOR, Vec2(UV_RIGHT, 1.0f)},
			{ Vec3(HALF_SIZE, -HALF_SIZE, 0.0f), DEFAULT_COLOR, Vec2(UV_RIGHT, 1.0f)},
		};

		// ���_�̕��я�
		vertex.indices = {
			0, 1, 2,
			1, 3, 2
		};
	}

	/*!
	@brief ���{���^���_�f�[�^�ƒ��_�C���f�b�N�X���쐬
	@param (point) �ߖڂ̍��W�z��
	@param (vertices) ���_�f�[�^
	@param (indices) ���_�C���f�b�N�X
	@param (axisVec) ���s���x�N�g��
	@param (weight) ��
	@param (texLoop) �e�N�X�`�����[�v
	*/
	bool RibonVerticesIndices(const vector<Vec3>& point, vector<VertexPositionColorTexture>& vertices, vector<uint16_t>& indices, const Vec3& axisVec, float weight, int texLoop)
	{
		const int& size = static_cast<int>(point.size());
		if (size == 0) return false;

		vertices.clear();

		for (int i = 0; i < size; i++)
		{
			const int loop = texLoop > 0 ? texLoop : 1;
			int front = (i - 1) < 0 ? size - 1 : i - 1;
			int rear = (i + 1) % size;
			Vec3 dir = point.at(front) - point.at(rear);
			Vec3 cross = dir.cross(axisVec).normalize();
		
			Vec3 pos = point.at(i) - cross * weight;
			VertexPositionColorTexture vertexLeft(pos, COL_WHITE, Vec2(0.0f, static_cast<float>(i) / size * loop));
			vertices.push_back(vertexLeft);

			pos = point.at(i) + cross * weight;
			VertexPositionColorTexture vertexRight(pos, COL_WHITE, Vec2(1.0f, static_cast<float>(i) / size * loop));
			vertices.push_back(vertexRight);
		}

		const vector<uint16_t> baseIndices = {
			0, 1, 2,
			2, 1, 3
		};

		indices.clear();

		for (int i = 0; i < point.size() - 1; i++)
		{
			for (int j = 0; j < 6; j++)
			{
				indices.push_back(baseIndices.at(j) + (2 * i));
			}
		}

		return true;
	}

	/*!
	@brief ���{���^���_�f�[�^�ƒ��_�C���f�b�N�X���쐬
	@param (point) �ߖڂ̍��W�z��
	@param (vertex) ���_�f�[�^�ƒ��_�C���f�b�N�X
	@param (axisVec) ���s���x�N�g��
	@param (weight) ��
	@param (texLoop) �e�N�X�`�����[�v
	*/
	bool RibonVerticesIndices(const vector<Vec3>& point, VertexData& vertex, const Vec3& axisVec, float weight, int texLoop)
	{
		return RibonVerticesIndices(point, vertex.vertices, vertex.indices, axisVec, weight, texLoop);
	}

	/*!
	@brief ���[���h���W���X�N���[�����W�ɕϊ�(BaseCross�̍��W�p�A����ȊO��DirectX�t���̊֐�����l)
	@param (viewPtr) view��shared_ptr
	@param (position) ���[���h���W
	@return �ϊ����ꂽ�X�N���[�����W
	*/
	Vec3 ConvertToWorldPosition(const shared_ptr<ViewBase>& viewPtr, const Vec3& position)
	{
		// �r���[����J�����ƃr���[�|�[�g���擾
		const auto& ptrCamera = viewPtr->GetTargetCamera();
		const auto& viewPort = viewPtr->GetTargetViewport();

		// �}�g���b�N�X�̒�`
		const Mat4x4 viewMat = ptrCamera->GetViewMatrix();
		const Mat4x4 projMat = ptrCamera->GetProjMatrix();
		Mat4x4 worldMat;
		worldMat.affineTransformation(
			Vec3(1.0f),
			Vec3(0.0f),
			Vec3(0.0f),
			Vec3(0.0f)
		);

		// 3D���W��2D���W�ɕϊ�(����0,0�)
		XMVECTOR xmWinPos = DirectX::XMVector3Project(
			position,
			viewPort.TopLeftX,
			viewPort.TopLeftY,
			viewPort.Width,
			viewPort.Height,
			viewPort.MinDepth,
			viewPort.MaxDepth,
			projMat,
			viewMat,
			worldMat
		);

		// BaseCross�p�ɕϊ�(����0,0�)
		Vec3 windowPos = Vec3(xmWinPos);
		windowPos.x -= static_cast<float>(viewPort.Width) / 2.0f;
		windowPos.y -= static_cast<float>(viewPort.Height) / 2.0f;
		windowPos.y *= -1.0f;

		// �ϊ����ꂽ���W
		return windowPos;
	}

	/*!
	@brief �f�B�O���[�p�����W�A���p�ɕϊ�
	@param (deg) �f�B�O���[�p
	@return ���W�A���p�ɕϊ����ꂽ�f�B�O���[�p
	*/
	float DegToRad(const float& deg)
	{
		return deg * (XM_PI / 180.0f);
	}

	Vec2 DegToRad(const Vec2& deg)
	{
		return deg * (XM_PI / 180.0f);
	}

	Vec3 DegToRad(const Vec3& deg)
	{
		return deg * (XM_PI / 180.0f);
	}

	Vec4 DegToRad(const Vec4& deg)
	{
		return deg * (XM_PI / 180.0f);
	}

	/*!
	@brief ���W�A���p���f�B�O���[�p�ɕϊ�
	@param (rad) ���W�A���p
	@return �f�B�O���[�p�ɕϊ����ꂽ���W�A���p
	*/
	float RadToDeg(const float& rad)
	{
		return rad * (180.0f / XM_PI);
	}

	Vec2 RadToDeg(const Vec2& rad)
	{
		return rad * (180.0f / XM_PI);
	}

	Vec3 RadToDeg(const Vec3& rad)
	{
		return rad * (180.0f / XM_PI);
	}

	Vec4 RadToDeg(const Vec4& rad)
	{
		return rad * (180.0f / XM_PI);
	}

	/*!
	@brief �r���{�[�h���N�H�[�^�j�I���ō쐬���Ԃ�
	@param (Line)�@�J�����̒����_ - �J�����̈ʒu
	@return �쐬���ꂽ�N�H�[�^�j�I��
	*/
	Quat GetBillboardQuat(const Vec3& Line)
	{
		// �����̃R�s�[
		Vec3 temp = Line;

		// �}�g���b�N�X�̐錾
		Mat4x4 rotMatrix;

		// ��]���̐ݒ�
		Vec3 defUp(0, 1.0f, 0);

		// ������Vec2�ɕϊ�
		Vec2 tempVec(temp.x, temp.z);

		// �O�㍶�E�̒�����0.1f�ȉ��Ȃ�
		if (tempVec.length() < 0.1f)
		{
			// ��]����ύX
			defUp = Vec3(0, 0, 1.0f);
		}
		
		// �����̐��K��
		temp.normalize();

		// �}�g���b�N�X�̌v�Z
		rotMatrix = XMMatrixLookAtLH(Vec3(0, 0, 0), temp, defUp);
		rotMatrix.inverse();

		// �N�H�[�^�j�I���ɕϊ����A���K��
		Quat qt = rotMatrix.quatInMatrix();
		qt.normalize();
		return qt;
	}

	/*!
	@brief �l���͈͓�����Ԃ�
	@param (value)�@�m�F����l
	@param (a)�@�͈͂̒l�P
	@param (b)�@�͈͂̒l�Q
	@return �͈͓����̐^�U
	*/
	bool GetBetween(int value, int a, int b)
	{
		return a > b ? value <= a && value >= b : value <= b && value >= a;
	}

	bool GetBetween(float value, float a, float b)
	{
		return a > b ? value <= a && value >= b : value <= b && value >= a;
	}

	bool GetBetween(const Vec2& value, const Vec2& a, const Vec2& b)
	{
		return GetBetween(value.x, a.x, b.x) && GetBetween(value.y, a.y, b.y);
	}

	bool GetBetween(const Vec3& value, const Vec3& a, const Vec3& b)
	{
		return GetBetween(value.x, a.x, b.x) && GetBetween(value.y, a.y, b.y) && GetBetween(value.z, a.z, b.z);
	}
}