/* See LICENSE file for copyright and license details. */

/* appearance */
static unsigned int borderpx              = 4;    /* border pixel of windows */
static const unsigned int gappx           = 6;    /* gaps between windows */
static unsigned int snap                  = 32;   /* snap pixel */
static int showbar                        = 1;    /* 0 means no bar */
static const unsigned int systraypinning  = 0;    /* 0: sloppy systray follows selected monitor, >0: pin systray to monitor X */
static const unsigned int systrayonleft   = 0;    /* 0: systray in the right corner, >0: systray on left of status text */
static const unsigned int systrayspacing  = 2;    /* systray spacing */
static const int systraypinningfailfirst  = 1;    /* 1: if pinning fails, display systray on the first monitor, False: display systray on the last monitor*/
static const int showsystray              = 1;    /* 0 means no systray */
static int topbar                         = 1;    /* 0 means bottom bar */
static const int user_bh                  = 25;   /* 0 means that dwm will calculate bar height, >=1 means dwm will user_bh as bar height */
static const Bool viewontag               = True; /* Switch view on tag switch */
#define ICONSIZE 16                               /* icon size */
#define ICONSPACING 5                             /* space between icon and title */
//static char font[]                      = "Ubuntu Mono:size=12";
static const char *fonts[]                = { "Ubuntu Mono Bold:size=10", "Font Awesome 5 Free Solid:size=12"};
static char dmenufont[]                   = "Ubuntu Mono:size=17";

//static const char *fonts[]                = { font };
static char normbgcolor[]                 = "#222222";
static char normbordercolor[]             = "#444444";
static char normfgcolor[]                 = "#bbbbbb";
static char selfgcolor[]                  = "#eeeeee";
static char selbordercolor[]              = "#005577";
static char selbgcolor[]                  = "#005577";
static char urgentborder[]                = "#BF616A";
static char *colors[][4] = {
       /*               fg           bg           border            float */
       [SchemeNorm] = { normfgcolor, normbgcolor,  normbordercolor, normbordercolor },
       [SchemeSel]  = { selfgcolor,  selbgcolor,   selbordercolor,  urgentborder },
       [SchemeUrg]  = { urgentborder,  urgentborder, urgentborder,    selbgcolor },
};
static const XPoint stickyicon[]    = { {0,0}, {4,0}, {4,8}, {2,6}, {0,8}, {0,0} }; /* represents the icon as an array of vertices */
static const XPoint stickyiconbb    = {4,8};	/* defines the bottom right corner of the polygon's bounding box (speeds up scaling) */

/* tagging */
//static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };
static const char *tags[] = { "", "", "", "", "", "" , "", "", ""};

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class               instance     title                         tags mask     isfloating   issticky   monitor */
  //Float
	{ "Gimp",              NULL,        NULL,                         0,            1,           0,         -1 },
	{ "RiseupVPN",         NULL,        NULL,                         0,            1,           0,         -1 },
	{ "mpv",               NULL,        NULL,                         0,            1,           1,         -1 },
	{ "Transmission-gtk",  NULL,        NULL,                         1 << 8,       0,           0,         -1 },
	{ "xev",               "xev",       "Event Tester",               0,            0,           0,         -1 },
  //Browser
	{ "Firefox",           NULL,        NULL,                         1 << 2,       0,           0,         -1 },
	{ "Brave-browser",     NULL,        NULL,                         1 << 2,       0,           0,         -1 },
	{ "Google-chrome",     NULL,        NULL,                         1 << 2,       0,           0,         -1 },
	{ "Chromium-browser",  NULL,        NULL,                         1 << 2,       0,           0,         -1 },
  //file manager
	{ "Caja",              NULL,        NULL,                         1 << 4,       0,           0,         -1 },
	{ "Pcmanfm",           NULL,        NULL,                         1 << 4,       0,           0,         -1 },
	{ "Thunar",            NULL,        NULL,                         1 << 4,       0,           0,         -1 },
	{ "Org.gnome.Nautilus",NULL,        NULL,                         1 << 4,       0,           0,         -1 },
  //Center
	{ "Mate-calc",         NULL,        NULL,                         0,            1,           1,         -1 },
	{ "RiseupVPN",         NULL,        NULL,                         0,            1,           0,         -1 },
	{ "Transmission-gtk",  NULL,        "Torrent Options",            0,            1,           0,         -1 },
	{ "Pcmanfm",           NULL,        "Confirm File Replacing",     0,            1,           0,         -1 },
	{ "Pcmanfm",           NULL,        "Copying files",              0,            1,           0,         -1 },
	{ "Pcmanfm",           NULL,        "Moving files",               0,            1,           0,         -1 },
  //Libreoffice
	{  NULL,               "libreoffice",NULL,                        1 << 6,       0,           0,         -1 },
};

/* layout(s) */
static float mfact              = 0.5; /* factor of master area size [0.05..0.95] */
static int nmaster              = 1;   /* number of clients in master area */
static int resizehints          = 1;   /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1;   /* 1 will force focus on the fullscreen window */

#include "layouts.c"
static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
	{ "HHH",      grid },
  { NULL,       NULL },
};

#include "dwm_config_pulseaudio.h"
/* key definitions */
#define MODKEY Mod4Mask
#define ALTMOD Mod1Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }
#define PrintScreenDWM      0x0000ff61

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[]                 = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", normbgcolor, "-nf", normfgcolor, "-sb", selbordercolor, "-sf", selfgcolor, NULL };
static const char *termcmd[]                  = { "alacritty", NULL };
static const char *roficmd[]                  = { "rofi", "-modi","drun", "-show", "drun", "-p", "Launch", "-show-icons", "-lines", "5", "-width", "40", NULL };
static const char *rofipass[]                 = { "/home/avdzm/.rofi_scripts/rofi-pass/rofi-pass", NULL };
static const char *rofivideo[]                = { "/home/avdzm/.rofi_scripts/rofi_video_player.sh", NULL };
static const char *torrentcmd[]               = {"/home/avdzm/.rofi_scripts/rofi_1337x.sh", NULL };
static const char *scrotcmd[]                 = {"scrot","-e","'mv $f ~/Picures/'", NULL };
static const char *powercmd[]                 = {"/home/avdzm/.rofi_scripts/power.sh", NULL };
static const char *documentscmd[]             = {"/home/avdzm/.rofi_scripts/rofi_documents.sh", NULL };
static const char *keyboardLayoutSwitchcmd[]  = {"/home/avdzm/.local/bin/keyboardLayoutSwitchcmditcher.sh us el", NULL };

static const char *mocplay[]  = {"mocp","-G", NULL };
static const char *mocprev[]  = {"mocp","-r", NULL };
static const char *mocnext[]  = {"mocp","-f", NULL };

/*
 * Xresources preferences to load at startup
 */
ResourcePref resources[] = {
		//{ "font",               STRING,  &font },
		{ "dmenufont",          STRING,  &dmenufont },
		{ "normbgcolor",        STRING,  &normbgcolor },
		{ "normbordercolor",    STRING,  &normbordercolor },
		{ "normfgcolor",        STRING,  &normfgcolor },
		{ "selbgcolor",         STRING,  &selbgcolor },
		{ "selbordercolor",     STRING,  &selbordercolor },
		{ "selfgcolor",         STRING,  &selfgcolor },
		{ "borderpx",          	INTEGER, &borderpx },
		{ "snap",          		  INTEGER, &snap },
		{ "showbar",          	INTEGER, &showbar },
		{ "topbar",          	  INTEGER, &topbar },
		{ "nmaster",          	INTEGER, &nmaster },
		{ "resizehints",       	INTEGER, &resizehints },
		{ "mfact",      	 	    FLOAT,   &mfact },
};

#include "movestack.c"
#include "focusurgent.c"
static Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY,                       XK_p,      spawn,          {.v = dmenucmd } },
	{ MODKEY,                       XK_Return, spawn,          {.v = termcmd } },
	{ MODKEY,                       XK_x,      spawn,          {.v = torrentcmd } },
	{ MODKEY,                       XK_z,      spawn,          {.v = rofipass } },
	{ MODKEY,                       XK_v,      spawn,          {.v = rofivideo } },
  { ControlMask,                  XK_Escape, spawn,          {.v = powercmd } },
  { 0,                            PrintScreenDWM, spawn,     {.v = scrotcmd} },
	{ MODKEY,                       XK_b,      togglebar,      {0} },
	{ MODKEY,                       XK_d,      spawn,          {.v = documentscmd } },
	{ MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_i,      incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_d,      incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
  { MODKEY|ShiftMask,             XK_j,      movestack,      {.i = +1 } },
  { MODKEY|ShiftMask,             XK_k,      movestack,      {.i = -1 } },
//	{ MODKEY,                       XK_Return, zoom,           {0} },
	{ MODKEY,                       XK_Tab,    view,           {0} },
  { MODKEY|ShiftMask,             XK_Return, spawn,          {.v = roficmd } },
	{ MODKEY,                       XK_q,      killclient,     {0} },
	{ MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_f,      setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_m,      setlayout,      {.v = &layouts[2]} },
	{ MODKEY,                       XK_g,      setlayout,      {.v = &layouts[3]} },
	{ MODKEY,                       XK_space,  cyclelayout,    {.i=+1} },
	{ MODKEY|ControlMask,           XK_space,  togglefloating, {0} },
	{ MODKEY|ShiftMask,             XK_space,  togglealwaysontop, {0} },
  { MODKEY,                       XK_s,      togglesticky,   {0} },
	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period, focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
  { MODKEY,                       XK_Left,   viewtoleft,     {0} },
  { MODKEY,                       XK_Right,  viewtoright,    {0} },
  { MODKEY|ShiftMask,             XK_Left,   tagtoleft,      {0} },
  { MODKEY|ShiftMask,             XK_Right,  tagtoright,     {0} },
  { 0,          XF86XK_AudioLowerVolume,     spawn,          {.v = downvol } },
  { 0,          XF86XK_AudioMute,            spawn,          {.v = mutevol } },
  { 0,          XF86XK_AudioRaiseVolume,     spawn,          {.v = upvol   } },
  { 0,          XF86XK_MonBrightnessUp  ,    spawn,          {.v = light_up} },
  { 0,          XF86XK_MonBrightnessDown,    spawn,          {.v = light_down} },
  { 0,          XF86XK_AudioPlay,            spawn,          {.v = mocplay } },
  { 0,          XF86XK_AudioPrev,            spawn,          {.v = mocprev } },
  { 0,          XF86XK_AudioNext,            spawn,          {.v = mocnext } },
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)
	{ MODKEY|ShiftMask,             XK_q,      try_quit,       {0} },
	{ MODKEY,                       XK_u,      focusurgent,    {0} },
	{ MODKEY|ShiftMask,             XK_r,      quit,           {1} },
  { ALTMOD,                       XK_Shift_L,   spawn,          {.v = keyboardLayoutSwitchcmd } },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        cyclelayout,    {.i = -1 } },
	{ ClkLtSymbol,          0,              Button3,        cyclelayout,    {.i = +1 } },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

/* how many windows should be open when quitting? */
/* on a stock dwm install, this seems to be two; however, you'll have to
 * change it depending on how many invisible X windows exist */
/* you can get a list with `xwininfo -tree -root`. */
static const int EMPTY_WINDOW_COUNT = 2;
