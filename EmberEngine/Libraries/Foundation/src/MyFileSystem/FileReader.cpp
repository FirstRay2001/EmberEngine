// FileReader.h
// 文件读取辅助
// created by FirstRay2001, Oct/14/2025

#include "FileReader.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include "Libraries/Foundation/Include/Collection.h"



float* MyFS::ReadFloatFromFile(const char* FilePath, int& OutCount)
{
	std::ifstream File(FilePath);

	// 打开是否成功
	if (!File.is_open())
	{
		printf("[ERROR] Failed to open file: %s \n", FilePath);
		OutCount = 0;
		return nullptr;
	}
	
	MySTL::TVector<float> Temp;
	
	// 逐行读取
	std::string Line;
	while (std::getline(File, Line))
	{
		std::istringstream LineStream(Line);
		std::string Token;

		// 按逗号分割
		while (std::getline(LineStream, Token, ','))
		{
			// 移除空格
			Token.erase(0, Token.find_first_not_of(" \t"));
			Token.erase(Token.find_last_not_of(" \t") + 1);

			// 移除尾部f
			if (!Token.empty() && (Token.back() == 'f' || Token.back() == 'F'))
			{
				Token.pop_back();
			}

			// 转换为float
			if(!Token.empty())
			{
				try
				{
					float Value = std::stof(Token);
					Temp.push_back(Value);
				}
				catch (const std::invalid_argument& e)
				{
					printf("[WARN] Invalid float value: %s \n", Token.c_str());
				}
			}
		}
	}

	OutCount = Temp.Size();

	// 转化为指针
	float* Result = new float[OutCount];
	memcpy(Result, Temp.GetRawData(), OutCount * sizeof(float));

	File.close();
	return Result;
}

int* MyFS::ReadIntFromFile(const char* FilePath, int& OutCount)
{
	std::fstream File(FilePath);

	// 打开是否成功
	if (!File.is_open())
	{
		printf("[ERROR] Failed to open file: %s \n", FilePath);
		OutCount = 0;
		return nullptr;
	}

	MySTL::TVector<int> Temp;

	// 逐行读取
	std::string Line;
	while (std::getline(File, Line))
	{
		std::istringstream LineStream(Line);
		std::string Token;

		// 按逗号分割
		while (std::getline(LineStream, Token, ','))
		{
			// 移除空格
			Token.erase(0, Token.find_first_not_of(" \t"));
			Token.erase(Token.find_last_not_of(" \t") + 1);

			// 转换为int
			if (!Token.empty())
			{
				try
				{
					int Value = std::stoi(Token);
					Temp.push_back(Value);
				}
				catch (const std::invalid_argument& e)
				{
					printf("[WARN] Invalid int value: %s \n", Token.c_str());
				}
			}
		}
	}

	OutCount = Temp.Size();

	// 转化为指针
	int* Result = new int[OutCount];
	memcpy(Result, Temp.GetRawData(), OutCount * sizeof(int));

	File.close();
	return Result;
}
