// FileReader.h
// �ļ���ȡ����
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

	// ���Ƿ�ɹ�
	if (!File.is_open())
	{
		printf("[ERROR] Failed to open file: %s \n", FilePath);
		OutCount = 0;
		return nullptr;
	}
	
	MySTL::TVector<float> Temp;
	
	// ���ж�ȡ
	std::string Line;
	while (std::getline(File, Line))
	{
		std::istringstream LineStream(Line);
		std::string Token;

		// �����ŷָ�
		while (std::getline(LineStream, Token, ','))
		{
			// �Ƴ��ո�
			Token.erase(0, Token.find_first_not_of(" \t"));
			Token.erase(Token.find_last_not_of(" \t") + 1);

			// �Ƴ�β��f
			if (!Token.empty() && (Token.back() == 'f' || Token.back() == 'F'))
			{
				Token.pop_back();
			}

			// ת��Ϊfloat
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

	// ת��Ϊָ��
	float* Result = new float[OutCount];
	memcpy(Result, Temp.GetRawData(), OutCount * sizeof(float));

	File.close();
	return Result;
}

int* MyFS::ReadIntFromFile(const char* FilePath, int& OutCount)
{
	std::fstream File(FilePath);

	// ���Ƿ�ɹ�
	if (!File.is_open())
	{
		printf("[ERROR] Failed to open file: %s \n", FilePath);
		OutCount = 0;
		return nullptr;
	}

	MySTL::TVector<int> Temp;

	// ���ж�ȡ
	std::string Line;
	while (std::getline(File, Line))
	{
		std::istringstream LineStream(Line);
		std::string Token;

		// �����ŷָ�
		while (std::getline(LineStream, Token, ','))
		{
			// �Ƴ��ո�
			Token.erase(0, Token.find_first_not_of(" \t"));
			Token.erase(Token.find_last_not_of(" \t") + 1);

			// ת��Ϊint
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

	// ת��Ϊָ��
	int* Result = new int[OutCount];
	memcpy(Result, Temp.GetRawData(), OutCount * sizeof(int));

	File.close();
	return Result;
}
