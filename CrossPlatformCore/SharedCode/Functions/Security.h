#pragma once

#include "../Common/HeaderList.h"

#include <stdlib.h>
#include <string.h>
#include "../Thirdparty/pbkdf2.h"

namespace NativeCore
{
    namespace Security
    {
        //TODO : 보안 관련 처리 로직 구현 예정

        // 원하는 해시 길이
        const size_t HashLenghth = 64;
        // PBKDF2 해시 생성 (iterations = 1000)
        const int iterations = 1000;
        const char* stored_base64_salt = "BzqIwrZ6Q5O1mK2iJbXghg==";

        /// <summary>
        /// 주어진 비밀번호를 사용하여 PBKDF2-SHA1 해시를 생성합니다.
        /// </summary>
        /// <param name="password">비밀번호 문자열</param>
        /// <returns>생성된 해시값을 포함하는 문자열 포인터</returns>
        const char* GenerateHash(const char* password)
        {
            size_t password_len = strlen(password);
            size_t salt_len = strlen(stored_base64_salt);

            char output[HashLenghth];

            pbkdf2_sha1(password, password_len, stored_base64_salt, salt_len, iterations, output, HashLenghth);

            // 생성된 해시 출력
            return output;
        }

        /// <summary>
        /// 입력된 비밀번호가 저장된 해시와 일치하는지 확인합니다.
        /// </summary>
        /// <param name="input_password">입력된 비밀번호 문자열</param>
        /// <param name="stored_hash">저장된 비밀번호 해시 값</param>
        /// <returns>비밀번호가 일치하면 true, 그렇지 않으면 false를 반환</returns>
        bool PasswordVerification(const char* input_password, const char* stored_hash)
        {
            char input_hash[HashLenghth];

            // 입력된 비밀번호로 해시 생성
            pbkdf2_sha1((char*)input_password, strlen(input_password), stored_base64_salt, HashLenghth, iterations, input_hash, HashLenghth);

            // 저장된 해시와 비교
            for (size_t i = 0; i < HashLenghth; ++i)
            {
                if (input_hash[i] != stored_hash[i]) 
                {
                    // 비밀번호가 일치하지 않음
                    return false;
                }
            }
            return true;
        }
    }
}