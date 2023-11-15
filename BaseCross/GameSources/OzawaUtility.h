/*!
@file OzawaUtility.h
@brief ���[�e�B���e�B
@author ���V���M
*/

#pragma once
#include "stdafx.h"

using namespace basecross;

namespace Utility
{
/*! 
@brief ���F����
 */
#define COL_NONE	Col4(0.0f)

/*!
@brief �L�F����
*/
#define COL_ALPHA	Col4(1.0f,1.0f,1.0f,0.0f)

/*!
@brief ���F
*/
#define COL_WHITE	Col4(1.0f)

/*!
@brief ���F
*/
#define COL_BLACK	Col4(0.0f,0.0f,0.0f,1.0f)

/*!
@brief �ԐF
*/
#define COL_RED		Col4(1.0f,0.0f,0.0f,1.0f)

/*!
@brief �ΐF
*/
#define COL_GREAN	Col4(0.0f, 1.0f, 0.0f, 1.0f)

/*!
@brief �F
*/
#define COL_BLUE	Col4(0.0f,0.0f,1.0f,1.0f)

/*!
@brief ���F
*/
#define COL_YELOW	Col4(1.0f, 1.0f, 0.0f, 1.0f)

/*!
@brief �ΐF
*/
#define COL_BG	Col4(0.0f, 1.0f, 1.0f, 1.0f)

/*!
@brief �D�F
*/
#define COL_GRAY	Col4(0.5f, 0.5f, 0.5f, 1.0f)

	/*!
	@brief ���`��Ԃ��s���֐�
	@param (start) ���`��Ԃ̎n�_�̒l
	@param (end) ���`��Ԃ̏I�_�̒l
	@param (time) ���`��Ԃ̊���
	@return �����ɉ����ĕ�Ԃ��ꂽ�l
	*/
	template <class T>
	inline T Lerp(const T& start, const T& end, float time)
	{
		time = time < 0.0f ? 0.0f : time;
		time = time > 1.0f ? 1.0f : time;
		return start + (end - start) * time;
	}

	/*!
	@brief �͈͂��w�肵���T�C���J�[�u�𐶐�
	@param (time) �o�ߎ���
	@param (min) �J�[�u�̉����l
	@param (max) �J�[�u�̏���l
	@return �͈͓��Ő������ꂽ�T�C���J�[�u
	*/
	template <class T>
	inline T SinCurve(const float& time, const T& min, const T& max)
	{
		T length = max - min;
		T helfLength = length / 2.0f;
		T curve = sin(time) * helfLength + helfLength;
		return curve + min;
	}

	/*!
	@brief �����_�ȉ��̌�����Ԃ��֐�
	@param (value)�����̌����𒲂ׂ����l
	@return �����_�ȉ��̌�����int�^�ŕԂ�
	*/
	template <class T>
	inline int GetLowerDigit(const T& value)
	{
		// float/double�^�ȊO�̏ꍇ��0��Ԃ��Ċ֐����I������
		if (typeid(value) != typeid(float) && typeid(value) != typeid(double))
		{
			return 0;
		}

		// ������ŏ����_�𐸍�����ׁA������^�̕ϐ��ɕϊ�
		wstringstream stream;
		stream << value;
		wstring str = stream.str();

		// �v���p�J�E���^
		int integer = 0;
		int decimal = 0;

		// ������̒��������[�v
		for (int i = 0; i < str.size(); i++)
		{
			// �����_������������
			if (str[i] == '.')
			{
				// �z��̗v�f����ێ�����for�����~
				integer = i;
				break;
			}
		}

		// �����_��������Ȃ�������
		if (integer == 0)
		{
			// �����_�ȉ���Ԃ��J�E���^��0�ɂ���
			decimal = 0;
		}
		else
		{
			// �����_�ȉ��̌�����Ԃ��J�E���^���v�Z����
			decimal = static_cast<int>(str.size()) - integer - 1;
		}

		// �����_�ȉ��̌�����Ԃ�
		return decimal;
	}

	/*!
	@brief �����̌�����Ԃ��֐�
	@param (value)�����̌����𒲂ׂ����l
	@return �����̂̌�����int�^�ŕԂ�
	*/
	template <class T>
	inline int GetUpperDigit(const T& value)
	{
		// float/double�^�ȊO�̏ꍇ��0��Ԃ��Ċ֐����I������
		if (typeid(value) != typeid(float) && typeid(value) != typeid(double) && typeid(value) != typeid(int))
		{
			return 0;
		}

		// ������ŏ����_�𐸍�����ׁA������^�̕ϐ��ɕϊ�
		wstringstream stream;
		stream << value;
		wstring str = stream.str();

		// �v���p�J�E���^
		int integer = 0;

		// ������̒��������[�v
		for (int i = 0; i < str.size(); i++)
		{
			// �����_������������for�����~
			if (str[i] == '.' || (integer == 0 && str[i] == '0')) break;
			integer++;
		}

		// �����_�ȏ�̌�����Ԃ�
		return integer;
	}

	/*!
	@brief �͈͂��w�肵�������𐶐�(�����_�ȉ����Ή�)
	@param (max) �����̏���l
	@param (min) �����̉����l
	@return �͈͓��Ő������ꂽ����
	*/
	template <class T>
	T RangeRand(const T& max, const T& min)
	{
		// ���������^�̏ꍇ�͏����̌������v�Z����
		int maxDigit = GetLowerDigit(max);
		int minDigit = GetLowerDigit(min);
		int digit = maxDigit > minDigit ? maxDigit : minDigit;

		// ���������߂�ׂɈ���������10�ׂ̂���Ōv�Z
		T divisionDigit = static_cast<T>(std::pow(10.0f, digit));

		// ��������x�����ɖ߂�
		int maxNum = static_cast<int>(max) * static_cast<int>(divisionDigit);
		int minNum = static_cast<int>(min) * static_cast<int>(divisionDigit);

		// �����������ɖ߂�(rand�֐������������������Ȃ���)
		return static_cast<T>(static_cast<float>((rand() % (maxNum - minNum + 1)) + minNum) / static_cast<float>(divisionDigit));
	}

	/*!
	@brief �V���v���Ȓ��_�f�[�^�ƒ��_�C���f�b�N�X���쐬
	@param (vertices) ���_�f�[�^
	@param (indices) ���_�C���f�b�N�X
	*/
	void SimpleVerticesIndices(vector<VertexPositionColorTexture>& vertices, vector<uint16_t>& indices);

	/*!
	@brief �V���v���Ȓ��_�f�[�^�ƒ��_�C���f�b�N�X���쐬
	@param (vertex) ���_�f�[�^�ƒ��_�C���f�b�N�X�\����
	*/
	void SimpleVerticesIndices(VertexData& vertex);

	/*!
	@brief �V���v���Ȓ��_�f�[�^�ƒ��_�C���f�b�N�X���쐬
	@param (vertices) ���_�f�[�^
	@param (indices) ���_�C���f�b�N�X
	@param (color) �Œ�F
	*/
	void SimpleVerticesIndices(vector<VertexPositionColorTexture>& vertices, vector<uint16_t>& indices, const Col4& color);

	/*!
	@brief �V���v���Ȓ��_�f�[�^�ƒ��_�C���f�b�N�X���쐬
	@param (vertices) ���_�f�[�^
	@param (indices) ���_�C���f�b�N�X
	@param (color) �Œ�F
	*/
	void SimpleVerticesIndices(VertexData& vertex, const Col4& color);

	/*!
	@brief �����p���_�f�[�^�ƒ��_�C���f�b�N�X���쐬
	@param (vertices) ���_�f�[�^
	@param (indices) ���_�C���f�b�N�X
	@param (number) ����
	*/
	void NumberVerticesIndices(vector<VertexPositionColorTexture>& vertices, vector<uint16_t>& indices, const int number);

	/*!
	@brief �����p���_�f�[�^�ƒ��_�C���f�b�N�X���쐬
	@param (vertex) ���_�f�[�^�ƒ��_�C���f�b�N�X�\����
	@param (number) ����
	*/
	void NumberVerticesIndices(VertexData& vertex, const int number);

	/*!
	@brief ���[���h���W���X�N���[�����W�ɕϊ�(BaseCross�̍��W�p�A����ȊO��DirectX�t���̊֐�����l)
	@param (viewPtr) view��shared_ptr
	@param (position) ���[���h���W
	@return �ϊ����ꂽ�X�N���[�����W
	*/
	Vec3 ConvertToWorldPosition(const shared_ptr<ViewBase>& viewPtr, const Vec3& position);
	
	/*!
	@brief �f�B�O���[�p�����W�A���p�ɕϊ�
	@param (deg) �f�B�O���[�p
	@return ���W�A���p�ɕϊ����ꂽ�f�B�O���[�p
	*/
	template<class T>
	T DegToRad(const T& deg)
	{
		return deg * (XM_PI / 180.0f);
	}
	float DegToRad(const float& deg);
	Vec2 DegToRad(const Vec2& deg);
	Vec3 DegToRad(const Vec3& deg);
	Vec4 DegToRad(const Vec4& deg);


	/*!
	@brief ���W�A���p���f�B�O���[�p�ɕϊ�
	@param (rad) ���W�A���p
	@return �f�B�O���[�p�ɕϊ����ꂽ���W�A���p
	*/
	template<class T>
	T RadToDeg(const T& deg)
	{
		return rad * (180.0f / XM_PI);
	}
	float RadToDeg(const float& rad);
	Vec2 RadToDeg(const Vec2& rad);
	Vec3 RadToDeg(const Vec3& rad);
	Vec4 RadToDeg(const Vec4& rad);

	/*!
	@brief �r���{�[�h���N�H�[�^�j�I���ō쐬���Ԃ�
	@param (Line)�@�J�����̒����_ - �J�����̈ʒu
	@return �쐬���ꂽ�N�H�[�^�j�I��
	*/
	Quat Billboard(const Vec3& Line);
}