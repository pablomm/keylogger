
#include <vector>
#include "base64.h"

namespace BASE64
{

const std::string &SALT1 = _SALT1;
const std::string &SALT2 = _SALT2;
const std::string &SALT3 = _SALT3;
const std::string &BASE64_CODES1 = _BASE64_CODES1;
const std::string &BASE64_CODES2 = _BASE64_CODES2;
const std::string &BASE64_CODES3 = _BASE64_CODES3;
const std::string &BASE64_CODES4 = _BASE64_CODES4;

std::string xor_encrypt(std::string s, std::string key)
{
    const char *key_str = key.c_str();
    int key_len = key.length();
    int i=0;
    for ( std::string::iterator it=s.begin(); it!=s.end(); ++it, ++i)
        *it ^= key_str[i % key_len];

    return s;
}

std::string EncryptB64 (std::string s)
{

    s = xor_encrypt(s,SALT1);
    s = xor_encrypt(s,SALT2);
    s = base64_encode (s,BASE64_CODES1);
    s = xor_encrypt(s,SALT1);
    s = xor_encrypt(s,SALT3);
    s = base64_encode (s,BASE64_CODES2);
    s = xor_encrypt(s,SALT2);
    s = xor_encrypt(s,SALT3);
    s = base64_encode (s,BASE64_CODES3);
    s = xor_encrypt(s,xor_encrypt(s.substr(0,4),s.substr(s.length()-5,s.length()-1)));
    s = base64_encode (s,BASE64_CODES4);
    s.insert(5,"H");
    s.insert(1,"a");
    return s;
}

std::string base64_encode (const std::string &s, const std::string &BASE64_CODES)
{
    std::string ret;
    int val = 0;
    int bits = -6;
    const unsigned int b63 = 0x3F;
    for (const auto &c : s)
    {
        val = (val << 8) + c;
        bits += 8;
        while (bits >= 0)
        {
            ret.push_back(BASE64_CODES[(val >> bits) & b63]);
            bits -= 6;
        }
    }
    if (bits > -6)
        ret.push_back(BASE64_CODES[((val << 8) >> (bits + 8)) & b63]);

    while (ret.size() % 4)
        ret.push_back('#');

    return ret;
}
}
