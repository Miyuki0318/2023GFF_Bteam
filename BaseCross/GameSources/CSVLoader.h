#pragma once
#include "stdafx.h"

// ----------------------------------------------------------------------------------
//	CSVLoader CSVファイルの読み込み用(mediaフォルダのCSVフォルダ内を読み込む)
// ----------------------------------------------------------------------------------
class CSVLoader
{
private:

	CSVLoader() {}
	~CSVLoader() {}

public:

	static vector<vector<string>>LoadFile(const string& filePath);
	static vector<vector<int>> ReadDataToInt(const vector<vector<string>>& data);
	static vector<vector<float>> ReadDataToFloat(const vector<vector<string>>& data);
	static vector<vector<double>> ReadDataToDouble(const vector<vector<string>>& data);
	static vector<int> ReadDataToInt(const vector<string>& data);
	static vector<int> ReadDataToInt(const vector<vector<string>>& data, const size_t& index);
	static vector<float> ReadDataToFloat(const vector<string>& data);
	static vector<float> ReadDataToFloat(const vector<vector<string>>& data, const size_t& index);
	static vector<double> ReadDataToDouble(const vector<string>& data);
	static vector<double> ReadDataToDouble(const vector<vector<string>>& data, const size_t& index);
};