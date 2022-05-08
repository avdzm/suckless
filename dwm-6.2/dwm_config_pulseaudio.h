
/* dwm_config_pulseaudio.h 
 * Hardware multimedia keys
 *  */
 /* Somewhere at the beginning of config.h include: */

 /* 
  *  You obviously need the X11 development package installed, but here is the upstream copy
  *   of this header if you can't bother using the contents of your own hard drive. ;-P
  *    https://cgit.freedesktop.org/xorg/proto/x11proto/tree/XF86keysym.h
  *    */

#include <X11/XF86keysym.h>

 /* Add somewhere in your constants definition section */

static const char *upvol[]   = { "pactl", "set-sink-volume", "0", "+5%",     NULL };
static const char *downvol[] = { "pactl", "set-sink-volume", "0", "-5%",     NULL };
static const char *mutevol[] = { "pactl", "set-sink-mute",   "0", "toggle",  NULL };


/* If you have a small laptop keyboard or don't want to spring your fingers too far away. */
static const char
  *light_up[] = {"brightnessctl", "s", "5%+", NULL},
  *light_down[] = {"brightnessctl", "s", "5%-", NULL};
