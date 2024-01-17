#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	void Timer::OnUpdate()
	{
		for (auto& timer : m_timers)
		{
			if (!timer.objectPtr.lock())
			{
				timer.Reset();
				continue;
			}

			timer.totalTime += DELTA_TIME;
		}
	}

	bool Timer::SetTimer(const weak_ptr<ObjectInterface>& ptr, float time, bool reset)
	{
		// ���݂��Ȃ��|�C���^���ǂ����̐^�U
		if (!ptr.lock()) return false;

		// �ݒ肳�ꂽ���Ԃ�0.0f�ȉ��������炻�������^�C�}�[�Ƃ��ĈӖ����Ȃ�
		if (time <= 0.0f) return false;

		// �`�F�b�J�[
		size_t elem = 0;
		bool check = false;

		// �����I�u�W�F�N�g���瑗��ꂽ���̃`�F�b�N
		for (size_t i = 0; i < m_timers.size(); i++)
		{
			// �|�C���^����v�Őݒ莞�Ԃ܂œ���Ȃ�
			if (m_timers.at(i).objectPtr.lock() == ptr.lock())
			{
				if (m_timers.at(i).limitTime == time)
				{
					elem = i;
					check = true;
					break;
				}
			}
		}

		// ���ꂪ�����Ȃ�V�K�o�^
		if (!check)
		{
			// �z��ɋ󂫂�����Ȃ�󂫂ɓ����
			for (auto& timer : m_timers)
			{
				if (!timer.objectPtr.lock())
				{
					timer.Set(ptr, time);
					return false;
				}
			}

			// ������Βǉ�
			TimerParam nTimer = { ptr, time };
			m_timers.push_back(nTimer);
			return false;
		}

		// �o�ߎ��Ԃ𖳎�����Set�����^�C�~���O�Ń��Z�b�g����Ȃ�
		if (reset)
		{
			// ���Ԃ��Đݒ�
			m_timers.at(elem).Set(time);
			return false;
		}
		else
		{
			// �o�ߎ��Ԃ��ݒ莞�Ԉȏ�ɂȂ����烊�Z�b�g����true��Ԃ�
			if (m_timers.at(elem).totalTime >= m_timers.at(elem).limitTime)
			{
				m_timers.at(elem).Reset();
				return true;
			}
		}

		return false;
	}
}