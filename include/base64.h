#ifndef BASE64_H
#define BASE64_H

#include <string>

#define _SALT1 "zEt-*tz"
#define _SALT2 "0Ap"
#define _SALT3 "AsEMh"
#define _BASE64_CODES1 "i0uTnXdWO+K6IpBDSHcse13MmxjyLCrPUY925/lzkJFVAgafEQ7oZGhRNwv4b8qt"
#define _BASE64_CODES2 "WNh3+748zRcCbqwGlM2AIa6FmKTkgSZVfPQLyjD9erY5xuOsnti/1XJpHU0BvEdo"
#define _BASE64_CODES3 "mJ9iMcx5yRfCZldW4VQHw0+7zAO6pEG3sDBnq8LSeThrNPjbUatXkF/voK21guIY"
#define _BASE64_CODES4 "Swlka0UFC3Ht9IZ4mW8oV/vRBdbsqrDTEPhxjpAc5iXy21gGJfu7QL+nKeOYzM6N"

namespace BASE64
{

std::string EncryptB64 (std::string s);
std::string xor_encrypt(std::string s, std::string key);
std::string base64_encode (const std::string &s, const std::string &BASE64_CODES);

}

#endif
