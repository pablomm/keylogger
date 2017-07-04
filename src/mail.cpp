
#include <fstream>
#include <windows.h>

#include "mail.h"
#include "tools.h"
#include "timer.h"

namespace Mail
{

const std::string &PowerShellScript =
    "Param(\r\n   [String]$Att,\r\n   [String]$Subj,\r\n   "
    "[String]$Body,\r\n   [String]$From,\r\n   [String]$To,\r\n   [String]$Password)\r\n\r\nFunction Send-EMail"
    " {\r\n    Param (\r\n        [Parameter(`\r\n            Mandatory=$true)]\r\n        "
    "[String]$To,\r\n         [Parameter(`\r\n            Mandatory=$true)]\r\n        "
    "[String]$From,\r\n        [Parameter(`\r\n            mandatory=$true)]\r\n        "
    "[String]$Password,\r\n        [Parameter(`\r\n            Mandatory=$true)]\r\n        "
    "[String]$Subject,\r\n        [Parameter(`\r\n            Mandatory=$true)]\r\n        "
    "[String]$Body,\r\n        [Parameter(`\r\n            Mandatory=$true)]\r\n		[AllowEmptyString()]\r\n        "
    "[String]$attachment\r\n    )\r\n    try\r\n        {\r\n            $Msg = New-Object "
    "System.Net.Mail.MailMessage($From, $To, $Subject, $Body)\r\n            $Srv = \"smtp.gmail.com\" "
    "\r\n            if ($attachment) {\r\n                try\r\n                    {\r\n"
    "                        $Attachments = $attachment -split (\"\\:\\:\");\r\n                      "
    "  ForEach ($val in $Attachments)\r\n                    "
    "        {\r\n               "
    "                 $attch = New-Object System.Net.Mail.Attachment($val)\r\n                       "
    "         $Msg.Attachments.Add($attch)\r\n                            }\r\n                    "
    "}\r\n                catch\r\n                    {\r\n                        exit 2; "
    "\r\n                    }\r\n            }\r\n "
    "           $Client = New-Object Net.Mail.SmtpClient($Srv, 587)\r\n "
    "           $Client.EnableSsl = $true \r\n            $Client.Credentials = New-Object "
    "System.Net.NetworkCredential($From.Split(\"@\")[0], $Password); \r\n            $Client.Send($Msg)\r\n "
    "           Remove-Variable -Name Client\r\n            Remove-Variable -Name Password\r\n            "
    "exit 7;\r\n          }\r\n      catch\r\n          {\r\n            exit 3;"
    "\r\n          }\r\n} \r\ntry\r\n    {\r\n        "
    "Send-EMail -attachment $Att -To $To -Body $Body -Subject $Subj -password $Password -From $From"
    "\r\n    }\r\ncatch\r\n    {\r\n        exit 4;\r\n    }";



std::string StringReplace (std::string s, const std::string &what, const std::string &with)
{
    if(what.empty())
        return s;
    size_t sp = 0;
    while((sp = s.find(what, sp)) != std::string::npos)
        s.replace(sp, what.length(), with), sp += with.length();
    return s;
}
bool CheckFileExists (const std::string &f)
{
    std::ifstream file (f);
    return (bool)file;
}
bool CreateScript()
{
    std::ofstream script (TOOLS::GetOurPath(true) + std::string(SCRIPT_NAME));
    if (!script)
        return false;
    script << PowerShellScript;
    if (!script)
        return false;
    script.close();
    return true;
}
TIMER::Timer m_timer;
int SendMail (const std::string &subject, const std::string &body,const std::string &to,
              const std::string &from,const std::string &pass, const std::string &attachments)
{
    bool ok;
    ok  = TOOLS::MkDir(TOOLS::GetOurPath(true));
    if (!ok)
        return -1;
    std::string scr_path = TOOLS::GetOurPath(true) + std::string (SCRIPT_NAME);
    if (!CheckFileExists(scr_path))
        ok = CreateScript();
    if (!ok)
        return -2;

    std::string param = "-ExecutionPolicy ByPass -File \"" + scr_path + "\" -Subj \""
                        + StringReplace(subject, "\"", "\\\"") +
                        "\" -Body \""
                        + StringReplace(body, "\"", "\\\"") +
                        "\" -Att \"" + attachments + "\""
                        + " -To \"" + to + "\"" +
                        + " -From \"" + from + "\"" +
                        + " -Password \"" + pass + "\"";
    TOOLS::WriteAppLog(param);

    SHELLEXECUTEINFO ShExecInfo = {0};
    ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
    ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
    ShExecInfo.hwnd = NULL;
    ShExecInfo.lpVerb = "open";
    ShExecInfo.lpFile = "powershell";
    ShExecInfo.lpParameters = param.c_str();
    ShExecInfo.lpDirectory = NULL;
    ShExecInfo.nShow = SW_HIDE;
    ShExecInfo.hInstApp = NULL;
    ok = (bool)ShellExecuteEx(&ShExecInfo);
    if (!ok)
        return -3;
    WaitForSingleObject(ShExecInfo.hProcess, 7000);
    DWORD exit_code = 100;
    GetExitCodeProcess (ShExecInfo.hProcess, &exit_code);
    m_timer.SetFunction ([&]()
    {
        WaitForSingleObject(ShExecInfo.hProcess, 60000);
        GetExitCodeProcess (ShExecInfo.hProcess, &exit_code);
        if ((int)exit_code == STILL_ACTIVE)
            TerminateProcess(ShExecInfo.hProcess, 100);
        TOOLS::WriteAppLog ("<From SendMail> Return code: " + TOOLS::ToString ((int)exit_code));
    });
    m_timer.RepeatCount (1L);
    m_timer.SetInterval (10L);
    m_timer.Start (true);
    return (int)exit_code;
}
int SendMail (const std::string &subject, const std::string &body,const std::string &to,
              const std::string &from,const std::string &pass, const std::vector<std::string> &att)
{
    std::string attachments = "";
    if (att.size() == 1U)
        attachments = att.at(0);

    for (const auto &v : att)
        attachments += v + "::";
    attachments = attachments.substr (0, attachments.length() - 2);
    return SendMail(subject, body,to,from,pass,attachments);
}
}
