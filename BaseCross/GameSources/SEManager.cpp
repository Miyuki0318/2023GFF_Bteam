#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	void SEManager::StartSE(const wstring& seKey, float volume, const void* objPtr)
	{
		if (seKey != L"" && objPtr != nullptr)
		{
			const auto& audioPtr = App::GetApp()->GetXAudio2Manager();
			m_seList.push_back(SE(audioPtr->Start(seKey, 0, volume), seKey, objPtr));
		}
	}

	void SEManager::StopSE(const wstring& seKey, const void* objPtr)
	{
		if (seKey != L"" && objPtr != nullptr)
		{
			for (size_t i = 0; i < m_seList.size(); i++)
			{
				if (m_seList.at(i).seKey == seKey)
				{
					if (m_seList.at(i).objectPtr == objPtr)
					{
						const auto& audioPtr = App::GetApp()->GetXAudio2Manager();
						audioPtr->Stop(m_seList.at(i).item.lock());
						m_seList.at(i).Reset();
						break;
					}
				}
			}
		}
	}
}