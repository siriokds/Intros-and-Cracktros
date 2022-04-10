#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#define WNDNAME	"Lockless"

#define RESX 320
#define RESY 240

#include "Types.h"
#include "SinTab.h"
#include "Fntz.h"

char	SCRBUF[((RESX+16)*16)+128];
char	FNTBUF[256*(16*4)];

char	tx1[] = "####";

char	text[] = 
"         * LOCKLESS *    zgPRESENTS ANOTHER FINE QUALITY RELEASE...     "
"VISIT OUR WEBSITE ATc       WWW.LOCKLESS.COM  z"
"OR GET IN TOUCH WITH US DROPPING BY OUR EFNET CHANNEL #LOCKLESS...c       "
"bRESPECT AND REGARDS TO ALL THE GROUPS AND INDIVIDUALS WHO ALWAYS SUPPORTED US "
"DURING GOOD AND BAD TIMES. "
"SIGNING OFF, SEE YA IN OUR NEXT PRODUCTION... "
"WE FLY HIGH...           ø             "
"                                                                "
"                                                                "
"                                                                "
"                                                                "
"                                                                "
"                                                                "
"                                                                "
"                                                                "
"                                                                "
"                                                                ";

