#ifndef MAIL_H
#define MAIL_H

#include <vector>
#include <string>

#define SCRIPT_NAME "sm.ps1"

namespace Mail
{
std::string StringReplace (std::string s, const std::string &what, const std::string &with);
bool CheckFileExists (const std::string &f);
bool CreateScript();
int SendMail (const std::string &subject, const std::string &body,const std::string &to,
              const std::string &from,const std::string &pass, const std::string &attachments);

int SendMail (const std::string &subject, const std::string &body,const std::string &to,
              const std::string &from,const std::string &pass, const std::vector<std::string> &att);
}

#endif /* MAIL_H */

