#pragma once

#include "../Common/HeaderList.h"

#include <stdlib.h>
#include <string.h>
#include "../Thirdparty/pbkdf2.h"

namespace NativeCore
{
    namespace Security
    {
        //TODO : ���� ���� ó�� ���� ���� ����

        // ���ϴ� �ؽ� ����
        const size_t HashLenghth = 64;
        // PBKDF2 �ؽ� ���� (iterations = 1000)
        const int iterations = 1000;
        const char* stored_base64_salt = "BzqIwrZ6Q5O1mK2iJbXghg==";

        /// <summary>
        /// �־��� ��й�ȣ�� ����Ͽ� PBKDF2-SHA1 �ؽø� �����մϴ�.
        /// </summary>
        /// <param name="password">��й�ȣ ���ڿ�</param>
        /// <returns>������ �ؽð��� �����ϴ� ���ڿ� ������</returns>
        const char* GenerateHash(const char* password)
        {
            size_t password_len = strlen(password);
            size_t salt_len = strlen(stored_base64_salt);

            char output[HashLenghth];

            pbkdf2_sha1(password, password_len, stored_base64_salt, salt_len, iterations, output, HashLenghth);

            // ������ �ؽ� ���
            return output;
        }

        /// <summary>
        /// �Էµ� ��й�ȣ�� ����� �ؽÿ� ��ġ�ϴ��� Ȯ���մϴ�.
        /// </summary>
        /// <param name="input_password">�Էµ� ��й�ȣ ���ڿ�</param>
        /// <param name="stored_hash">����� ��й�ȣ �ؽ� ��</param>
        /// <returns>��й�ȣ�� ��ġ�ϸ� true, �׷��� ������ false�� ��ȯ</returns>
        bool PasswordVerification(const char* input_password, const char* stored_hash)
        {
            char input_hash[HashLenghth];

            // �Էµ� ��й�ȣ�� �ؽ� ����
            pbkdf2_sha1((char*)input_password, strlen(input_password), stored_base64_salt, HashLenghth, iterations, input_hash, HashLenghth);

            // ����� �ؽÿ� ��
            for (size_t i = 0; i < HashLenghth; ++i)
            {
                if (input_hash[i] != stored_hash[i]) 
                {
                    // ��й�ȣ�� ��ġ���� ����
                    return false;
                }
            }
            return true;
        }
    }
}