
#ifndef TOOLS_H
#define TOOLS_H

#include <string>

#define TOOLS_DIR "App.log"


namespace TOOLS
{

template <class T>
std::string ToString (const T &);

std::string int_to_hex( unsigned long i );

std::string GetDateTimeString (const std::string &sep);

void WriteAppLog (const std::string &s);

std::string GetOurPath (const bool append_separarator);

bool MkOneDr (std::string path);

bool MkDir (std::string path);

void WriteFile (const std::string &s, const std::string &filename);

}
#endif /* TOOLS_H */
