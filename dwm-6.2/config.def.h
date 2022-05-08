/* See LICENSE file for copyright and license details. */

/* appearance */
static const unsigned int borderpx       = 2;        /* border pixel of windows */
static const unsigned int gappx          = 6;        /* gaps between windows */
static const unsigned int snap           = 32;       /* snap pixel */
static const unsigned int systraypinning = 0;   /* 0: sloppy systray follows selected monitor, >0: pin systray to monitor X */
static const unsigned int systrayonleft  = 0;   	/* 0: systray in the right corner, >0: systray on left of status text */
static const unsigned int systrayspacing = 1;   /* systray spacing */
static const int systraypinningfailfirst = 1;   /* 1: if pinning fails, display systray on the first monitor, False: display systray on the last monitor*/
static const int showsystray             = 1;        /* 0 means no systray */
static const int showbar                 = 1;        /* 0 means no bar */
static const int topbar                  = 1;        /* 0 means bottom bar */
static const int user_bh                 = 25;       /* 0 means that dwm will calculate bar height, >= 1 means dwm will user_bh as bar height */
#define ICONSIZE 16   /* icon size */
#define ICONSPACING 5 /* space between icon and title */
static const double activeopacity   = 1.0f;     /* Window opacity when it's focused (0 <= opacity <= 1) */
static const double inactiveopacity = 1.0f;     //0.875f;   /* Window opacity when it's inactive (0 <= opacity <= 1) */
static const char *fonts[]          = { "Ubuntu Mono Bold:size=10", "Font Awesome 5 Free Solid:size=12"};
static const char dmenufont[]       = "Ubuntu Mono:size=14";
static const char col_gray1[]       = "#22232e"; //#222222";
static const char col_gray2[]       = "#22232e"; //#444444";
static const char col_gray3[]       = "#CAA9FA"; //#bbbbbb";
static const char col_gray4[]       = "#CAA9FA"; //#eeeeee";
static const char col_cyan[]        = "#CAA9FA"; //"#504C67"; //"#005577";
static const char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm] = { col_gray3, col_gray1, col_gray2 },
	[SchemeSel]  = { col_gray1, col_cyan,  col_cyan  },
};
static const XPoint stickyicon[]    = { {0,0}, {4,0}, {4,8}, {2,6}, {0,8}, {0,0} }; /* represents the icon as an array of vertices */
static const XPoint stickyiconbb    = {4,8};	/* defines the bottom right corner of the polygon's bounding box (speeds up scaling) */

/* tagging */
static const char *tags[] = { "", "", "", "", "", "" , "", "", ""};

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class               instance    title       tags mask     isfloating   monitor */
	{ "Gimp",              NULL,       NULL,       0,            1,           -1 },
  //Browsers
	{ "Firefox",           NULL,       NULL,       1 << 2,       0,           -1 },
	{ "Transmission-gtk",  NULL,       "Transmission", 1 << 8,       0,           -1 },
	{ "Google-chrome",     NULL,       NULL,       1 << 2,       0,           -1 },
	{ "Brave-browser",     NULL,       NULL,       1 << 2,       0,           -1 },
	{ "Chromium-browser",  NULL,       NULL,       1 << 2,       0,           -1 },
  //file manager
	{ "Pcmanfm",           NULL,       NULL,       1 << 4,       0,           -1 },
	{ "Thunar",            NULL,       NULL,       1 << 4,       0,           -1 },
	{ "Org.gnome.Nautilus",NULL,       NULL,       1 << 4,       0,           -1 },
  //Center
	{ "Gnome-calculator",  NULL,       NULL,       0,       1,           -1 },
	{ "RiseupVPN",         NULL,       NULL,       0,       1,           -1 },
	{ "Transmission-gtk",  NULL,       "Torrent Options",           0,  1,  -1 },
	{ "Pcmanfm",           NULL,       "Confirm File Replacing",    0,  1,  -1 },
	{ "Pcmanfm",           NULL,       "Copying files",             0,  1,  -1 },
	{ "Pcmanfm",           NULL,       "Moving files",              0,  1,  -1 },
  //Libreoffice
	{ NULL,                "libreoffice",NULL,       1 << 6,       0,        -1 },
  //Video player
  { "mpv",               NULL,        NULL,       0,            1,-1},
};

/* layout(s) */
static const float mfact     = 0.5; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */
static const int decorhints  = 1;    /* 1 means respect decoration hints */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */

#include "grid.c"
static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
	{ "HHH",      grid },
  { NULL,       NULL},
};

#include "dwm_config_pulseaudio.h"
/* key definitions */
#define MODKEY Mod4Mask
#define ALTMOD Mod1Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} }, \
  { ALTMOD,                       KEY,      focusnthmon,    {.i  = TAG } }, \
  { ALTMOD|ShiftMask,             KEY,      tagnthmon,      {.i  = TAG } },


/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }
#define PrintScreenDWM      0x0000ff61
/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[]     = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray3, "-sb", col_gray4, "-sf", col_gray1, topbar ? NULL: "-b", NULL };
static const char *termcmd[]      = { "alacritty", NULL };
static const char *roficmd[]      = { "rofi", "-modi","drun", "-show", "drun", "-p", "Launch", "-show-icons", "-lines", "5", "-width", "40", NULL };
static const char *rofipass[]     = { "/home/avdzm/.rofi_scripts/rofi-pass/rofi-pass", NULL };
static const char *rofivideo[]    = { "/home/avdzm/.rofi_scripts/rofi_video_player.sh", NULL };
static const char *torrentcmd[]   = {"/home/avdzm/.rofi_scripts/rofi_1337x.sh", NULL };
static const char *scrotcmd[]     = {"scrot","-e","'mv $f ~/Picures/'", NULL };
static const char *powercmd[]     = {"/home/avdzm/.rofi_scripts/power.sh", NULL };
static const char *documentscmd[] = {"/home/avdzm/.rofi_scripts/rofi_documents.sh", NULL };
static const char *keyboardLayoutSwitchcmd[] = {"/home/avdzm/.local/bin/keyboardLayout.sh", NULL };

static const char *mocplay[]  = {"mocp","-G", NULL };
static const char *mocprev[]  = {"mocp","-r", NULL };
static const char *mocnext[]  = {"mocp","-f", NULL };

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
	{ MODKEY|ShiftMask,             XK_j,      rotatestack,    {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_k,      rotatestack,    {.i = -1 } },
	{ MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_i,      incnmaster,     {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_i,      incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
	/*{ MODKEY,                       XK_Return, zoom,           {0} },*/
  { MODKEY|ShiftMask,             XK_Return, spawn,          {.v = roficmd } },
	{ MODKEY,                       XK_Tab,    view,           {0} },
	{ MODKEY,                       XK_q,      killclient,     {0} },
	{ MODKEY,                       XK_m,      setlayout,      {.v = &layouts[2]} },
	{ MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_f,      setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_g,      setlayout,      {.v = &layouts[3]} },
  //{ MODKEY|ControlMask,           XK_comma,  cyclelayout,    {.i = -1 } },
  //{ MODKEY|ControlMask,           XK_period, cyclelayout,    {.i = +1 } },
	//{ MODKEY,                       XK_space,  setlayout,      {0} },
	{ MODKEY|ControlMask,           XK_space,  togglefloating, {0} },
  { MODKEY,                       XK_space,  cyclelayout,    {.i = +1} },
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
