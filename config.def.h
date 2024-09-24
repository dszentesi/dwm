/* See LICENSE file for copyright and license details. */

#include <X11/XF86keysym.h>

#include "colors/rose-pine.h"

/* appearance */
static const unsigned int borderpx  = 1;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const char *fonts[]          = { "CommitMono:size=12" };
static const char dmenufont[]       = "CommitMono:size=12";
static const char col_gray1[]       = "#222222";
static const char col_gray2[]       = "#444444";
static const char col_gray3[]       = "#bbbbbb";
static const char col_gray4[]       = "#eeeeee";
static const char col_cyan[]        = "#005577";

static const char *colors[][3]  = {
  /*                     fg   bg   border */
  [SchemeNorm]       = { fg1, bg0, fg2 },
  [SchemeSel]        = { bg1, red, bluu },
  [SchemeStatus]     = { fg1, bg0,  "#000000"  }, // Statusbar right {text,background,not used but cannot be empty}
  [SchemeTagsSel]    = { bg1, red,  "#000000"  }, // Tagbar left selected {text,background,not used but cannot be empty}
  [SchemeTagsNorm]   = { fg1, bg0,  "#000000"  }, // Tagbar left unselected {text,background,not used but cannot be empty}
  [SchemeInfoSel]    = { fg1, bg0,  "#000000"  }, // infobar middle  selected {text,background,not used but cannot be empty}
  [SchemeInfoNorm]   = { fg1, bg0,  "#000000"  }, // infobar middle  unselected {text,background,not used but cannot be empty}

  //[SchemeTag]        = { bg0, bg1, bg0 },
  //[SchemeTag1]       = { rose, bg0,  bg1 },
  //[SchemeTag2]       = { red, bg0, bg1 },
  //[SchemeTag3]       = { yellow, bg0, bg1 },
  //[SchemeTag4]       = { bluu, bg0, bg1 },
  //[SchemeTag5]       = { purple, bg0, bg1 },
  //[SchemeTag6]       = { cyan, bg0, bg1 },
  //[SchemeLayout]     = { rose, bg0, bg1 },
  //[SchemeTitle]      = { fg0, bg0, bg1 },
  //[SchemeTitle1]     = { rose, bg0, bg1 },
  //[SchemeTitle2]     = { red, bg0, bg1 },
  //[SchemeTitle3]     = { yellow, bg0, bg1 },
  //[SchemeTitle4]     = { bluu, bg0, bg1 },
  //[SchemeTitle5]     = { purple, bg0, bg1 },
  //[SchemeTitle6]     = { cyan, bg0, bg1 },
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance    title       tags mask     isfloating   monitor */
	{ "Gimp",     NULL,       NULL,       0,            1,           -1 },
	{ "Firefox",  NULL,       NULL,       1 << 8,       0,           -1 },
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      comboview,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      combotag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
// Programs
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray3, "-sb", col_cyan, "-sf", col_gray4, NULL };
static const char *termcmd[]  = { "kitty", NULL };

// Volume
static const char *mutecmd[] = { "pactl", "set-sink-mute", "0", "toggle", NULL };
static const char *volupcmd[] = { "pactl", "set-sink-volume", "0", "+5%", NULL };
static const char *voldowncmd[] = { "pactl", "set-sink-volume", "0", "-5%", NULL };

// System
static const char *logoutcmd[] = { "loginctl", "lock-session", NULL };

static const Key keys[] = {
	/* modifier                     key        function        argument */
    
    // Audio
    { 0,                   XF86XK_AudioMute,   spawn,          {.v = mutecmd } },
    { 0,                   XF86XK_AudioMute,   spawn,          {.v = mutecmd } },
    { 0,            XF86XK_AudioLowerVolume,   spawn,          {.v = voldowncmd } },
    { 0,            XF86XK_AudioLowerVolume,   spawn,          {.v = voldowncmd } },
    { 0,            XF86XK_AudioRaiseVolume,   spawn,          {.v = volupcmd } },
    { 0,            XF86XK_AudioRaiseVolume,   spawn,          {.v = volupcmd } },

    // Window manager
	{ MODKEY,                       XK_space,  spawn,          {.v = dmenucmd } },
	{ MODKEY,                       XK_Return, spawn,          {.v = termcmd } },
	{ MODKEY,                       XK_b,      togglebar,      {0} },
	{ MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_i,      incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_d,      incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
	{ MODKEY|ShiftMask,             XK_Return, zoom,           {0} },
	{ MODKEY,                       XK_Tab,    comboview,           {0} },
	{ MODKEY|ShiftMask,             XK_c,      killclient,     {0} },
	{ MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_f,      setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_m,      setlayout,      {.v = &layouts[2]} },
	{ MODKEY,                       XK_y,      setlayout,      {0} },
	{ MODKEY|ShiftMask,             XK_n,      togglefloating, {0} },
	{ MODKEY,                       XK_0,      comboview,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      combotag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period, focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
	TAGKEYS(                        XK_exclam,                  0)
	TAGKEYS(                        XK_at,                      1)
	TAGKEYS(                        XK_numbersign,              2)
	TAGKEYS(                        XK_dollar,                  3)
	TAGKEYS(                        XK_percent,                 4)
	TAGKEYS(                        XK_asciicircum,             5)
	TAGKEYS(                        XK_ampersand,               6)
	TAGKEYS(                        XK_asterisk,                7)
	TAGKEYS(                        XK_parenleft,               8)
	{ MODKEY|ShiftMask,             XK_q,      quit,           {0} },
    { MODKEY,                       XK_Escape, spawn,          {.v = logoutcmd} },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static const Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
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

