#pragma once
#ifndef _ENCRYPTION_H_
#define _ENCRYPTION_H_

using byte = unsigned char;
using word = unsigned short;
using dword = unsigned long;
using qword = unsigned long long;

namespace Encryptation
{
	enum EncryptionFlags_
	{
		EncryptionFlags_Length_Out_of_Range = -2,
		EncryptionFlags_Invalid_Packet = -1,
		EncryptionFlags_Hello_Packet = 0
	};

	int Decrypt(char* dst, unsigned long length);
	int Encrypt(char* dst, char* src, unsigned long length);
}

#endif
