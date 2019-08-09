// IbexFileEncryptionTests.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include "IbexFileEncryption.h"
#include <string>
#include <tchar.h>
#include <vector>

//#include <stdlib.h>
#include <time.h>

#define STR_LEN 10	//random data length
std::string GenerateStr(long int seed)
{
	std::string str;
	ibex::encryption::tstring st;
	int i, flag;
	srand(seed);
	for (i = 0; i < 10; i++)
	{
		flag = rand() % 3;
		switch (flag)
		{
		case 0:
			str +=  rand()  % 26 + 'a';
			st += (rand() % 26 + 'a');
			break;
		case 1:
			str +=  rand() % 26 + 'A';
			st += (rand() % 26 + 'A');
			break;
		case 2:
			str +=  rand() % 10 + '0';
			st+=(rand() % 10 + '0');
			break;
		}
	}
	printf("st size = %d\n", st.size());
	printf("st length = %d\n", st.length());
	printf("size of char is %d\n", sizeof(char));
	printf("size of TCHAR is %d\n", sizeof(TCHAR));
	return str;
}


int main()
{
	long int seed = static_cast<long int>(time(NULL));		//use time as seed and the result will be a random each time.
	std::string key;
	key = GenerateStr(seed);

	std::string key1;
	key1 = GenerateStr(seed);

	ibex::encryption::tstring test = _T("abcdef");
	int len = WideCharToMultiByte(CP_ACP, 0, test.c_str(), wcslen(test.c_str()), NULL, 0, NULL, NULL);
	char *m_char = new char[len + 1];
	WideCharToMultiByte(CP_ACP, 0, test.c_str(), wcslen(test.c_str()), m_char, len, NULL, NULL);
	m_char[len] = '\0';
	delete[]m_char;

	std::vector<char> testv(len);
	WideCharToMultiByte(CP_ACP, 0, test.c_str(), wcslen(test.c_str()), &testv[0], len, NULL, NULL);


	//std::string stest = (unsigned char*)test.c_str();
	
	//for (size_t i = 0; i < 20; i++)
	//{
	//	GenerateStr();
	//	//printf("current time is %ld\n", time(NULL));
	//	//Sleep(1000);
	//}	

	//ibex::encryption::CIbexFileEncryption *encrypt = new ibex::encryption::CIbexFileEncryption(L"123456789012345678901234567890aa");
	//ibex::encryption::encryptBufferData_t buff;
	//for (size_t i = 0; i < 20; i++)
	//{
	//	buff.push_back('a');
	//}

	//unsigned long ret = encrypt->encrypt(buff, L"C:\\TEST\\test.txt");
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
