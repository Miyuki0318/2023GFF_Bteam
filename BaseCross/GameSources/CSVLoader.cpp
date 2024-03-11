#include "stdafx.h"
#include "Project.h"

vector<vector<string>> CSVLoader::LoadFile(const string& filePath)
{
	ifstream ifs("../media/CSV/" + filePath + ".csv");
	if (!ifs.is_open())
	{
		ifs.open("./media/CSV/" + filePath + ".csv");
		if (!ifs.is_open())
		{
#if BASECROSS
			wstring file;
			basecross::Util::ConvertUtf8toWstring(filePath, file);
			throw basecross::BaseException(
				L"CSVÉtÉ@ÉCÉãÇ™ë∂ç›ÇµÇ‹ÇπÇÒ",
				file + L".csv",
				L"CSVLoader::LoadFile"
			);
#endif
			vector<vector<string>> empty;
			return empty;
		}
	}

	int rowCount = 0;
	string buffer;
	vector<vector<string>> data;
	while (getline(ifs, buffer))
	{
		if (rowCount > 0)
		{
			vector<string> row;
			istringstream iss(buffer);
			string col;

			while (getline(iss, col, ','))
			{
				if (col != "")
				{
					row.push_back(col);
				}
			}

			if (row.size() != 0 && row.size() != 1) data.push_back(row);
		}
		rowCount++;
	}

	return data;
}

vector<vector<int>> CSVLoader::ReadDataToInt(const vector<vector<string>>& data) 
{
	vector<vector<int>> ret;
	for (int row = 0; row < data.size(); row++)
	{
		vector<string> rec = data.at(row);
		vector<int> line;
		for (int col = 0; col < rec.size(); col++)
		{
			line.push_back(stoi(rec[col]));
		}
		ret.push_back(line);
	}
	return ret;
}

vector<int> CSVLoader::ReadDataToInt(const vector<string>& data)
{
	vector<int> ret;
	for (int col = 0; col < data.size(); col++)
	{
		ret.push_back(stoi(data[col]));
	}
	return ret;
}

vector<int> CSVLoader::ReadDataToInt(const vector<vector<string>>& data, const size_t& index)
{
	vector<int> ret;
	vector<string> rec = data[index];
	for (int col = 0; col < rec.size(); col++)
	{
		ret.push_back(stoi(rec[col]));
	}
	return ret;
}

vector<vector<float>> CSVLoader::ReadDataToFloat(const vector<vector<string>>& data)
{
	vector<vector<float>> ret;
	for (int row = 0; row < data.size(); row++)
	{
		vector<string> rec = data.at(row);
		vector<float> line;
		for (int col = 0; col < rec.size(); col++)
		{
			line.push_back(stof(rec[col]));
		}
		ret.push_back(line);
	}
	return ret;
}

vector<float> CSVLoader::ReadDataToFloat(const vector<string>& data)
{
	vector<float> ret;
	vector<string> rec = data;
	for (int col = 0; col < rec.size(); col++)
	{
		ret.push_back(stof(rec[col]));
	}
	return ret;
}

vector<float> CSVLoader::ReadDataToFloat(const vector<vector<string>>& data, const size_t& index)
{
	vector<float> ret;
	vector<string> rec = data[index];
	for (int col = 0; col < rec.size(); col++)
	{
		ret.push_back(stof(rec[col]));
	}
	return ret;
}

vector<vector<double>> CSVLoader::ReadDataToDouble(const vector<vector<string>>& data)
{
	vector<vector<double>> ret;
	for (int row = 0; row < data.size(); row++)
	{
		vector<string> rec = data.at(row);
		vector<double> line;
		for (int col = 0; col < rec.size(); col++)
		{
			line.push_back(stod(rec[col]));
		}
		ret.push_back(line);
	}
	return ret;
}

vector<double> CSVLoader::ReadDataToDouble(const vector<string>& data)
{
	vector<double> ret;
	vector<string> rec = data;
	for (int col = 0; col < rec.size(); col++)
	{
		ret.push_back(stod(rec[col]));
	}
	return ret;
}

vector<double> CSVLoader::ReadDataToDouble(const vector<vector<string>>& data, const size_t& index)
{
	vector<double> ret;
	vector<string> rec = data[index];
	for (int col = 0; col < rec.size(); col++)
	{
		ret.push_back(stod(rec[col]));
	}
	return ret;
}