#pragma once
/*
-----------------------------------------------------------------------------
 Class: ThemeManager

 Desc: Manages which graphics and sounds are chosed to load.  Every time 
	a sound or graphic is loaded, it gets the path from the ThemeManager.

 Copyright (c) 2001-2002 by the person(s) listed below.  All rights reserved.
	Chris Danford
-----------------------------------------------------------------------------
*/

#include "RageUtil.h"
#include "D3DX8Math.h"	// for D3DXCOLOR
class IniFile;


enum ThemeElement { 
/*
	GRAPHIC_ALL_MUSIC_BANNER,
	GRAPHIC_ARROWS_LEFT,
	GRAPHIC_ARROWS_RIGHT,
	GRAPHIC_CAUTION,
	GRAPHIC_GAMEPLAY_CLOSING_STAR,
	GRAPHIC_EDIT_BACKGROUND,
	GRAPHIC_EDIT_TOP_EDGE,
	GRAPHIC_EDIT_SNAP_INDICATOR,
	GRAPHIC_MUSIC_SCROLL_BACKGROUND,
	GRAPHIC_FALLBACK_BACKGROUND,
	"Graphics","fallback banner",
	"Graphics","fallback cd title",
	GRAPHIC_OPTIONS_CURSOR,
	GRAPHIC_OPTIONS_UNDERLINE,
	GRAPHIC_GAME_OPTIONS_BACKGROUND,
	GRAPHIC_GAME_OPTIONS_TOP_EDGE,
	GRAPHIC_GAME_OVER,
	GRAPHIC_GAMEPLAY_CLEARED,
	GRAPHIC_GAMEPLAY_COMBO,		
	GRAPHIC_GAMEPLAY_DANGER_BACKGROUND,
	GRAPHIC_GAMEPLAY_DANGER_TEXT,
	GRAPHIC_GAMEPLAY_DEMONSTRATION,
	GRAPHIC_GAMEPLAY_FAILED,	
	GRAPHIC_GAMEPLAY_HOLD_JUDGEMENT,	
	GRAPHIC_GAMEPLAY_JUDGEMENT,	
	GRAPHIC_GAMEPLAY_READY,	
	GRAPHIC_GAMEPLAY_HERE_WE_GO,	
	GRAPHIC_GAMEPLAY_BOTTOM_FRAME,
	GRAPHIC_GAMEPLAY_DIFFICULTY_BANNERS,
	GRAPHIC_GAMEPLAY_TOP_FRAME,
	GRAPHIC_GAMEPLAY_MIDDLE_FRAME,
	GRAPHIC_GAMEPLAY_LIFEMETER_BAR,
	GRAPHIC_GAMEPLAY_LIFEMETER_ONI,
	GRAPHIC_GAMEPLAY_LIFEMETER_BATTERY,
	GRAPHIC_GAMEPLAY_LIFEMETER_STREAM_NORMAL,
	GRAPHIC_GAMEPLAY_LIFEMETER_STREAM_HOT,
	GRAPHIC_GAMEPLAY_ONI_GAMEOVER,
	GRAPHIC_GAMEPLAY_OPENING_STAR,
	GRAPHIC_GRAPHIC_OPTIONS_BACKGROUND,
	GRAPHIC_GRAPHIC_OPTIONS_TOP_EDGE,
	GRAPHIC_KEEP_ALIVE,	
	GRAPHIC_MAP_CONTROLLERS_BACKGROUND,
	GRAPHIC_MAP_CONTROLLERS_TOP_EDGE,
	GRAPHIC_MENU_BOTTOM_EDGE,
	GRAPHIC_MENU_STYLE_ICONS,
	GRAPHIC_MUSIC_SORT_ICONS,
	GRAPHIC_MUSIC_STATUS_ICONS,
	GRAPHIC_PLAYER_OPTIONS_BACKGROUND,
	GRAPHIC_PLAYER_OPTIONS_TOP_EDGE,
	GRAPHIC_EVALUATION_BACKGROUND,
	GRAPHIC_EVALUATION_BANNER_FRAME,
	GRAPHIC_EVALUATION_BONUS_FRAME_P1,
	GRAPHIC_EVALUATION_BONUS_FRAME_P2,
	GRAPHIC_EVALUATION_STAGE_FRAME_P1,
	GRAPHIC_EVALUATION_STAGE_FRAME_P2,
	GRAPHIC_EVALUATION_GRADE_FRAME,
	GRAPHIC_EVALUATION_GRADES,	
	GRAPHIC_EVALUATION_JUDGE_LABELS,
	GRAPHIC_EVALUATION_SCORE_LABELS,
	GRAPHIC_EVALUATION_SUMMARY_TOP_EDGE,
	GRAPHIC_EVALUATION_TOP_EDGE,
	GRAPHIC_SELECT_COURSE_INFO_FRAME,
	GRAPHIC_SELECT_COURSE_TOP_EDGE,
	GRAPHIC_SELECT_COURSE_BACKGROUND,
	GRAPHIC_SELECT_COURSE_CONTENT_BAR,
	GRAPHIC_SELECT_DIFFICULTY_ARROWS,
	GRAPHIC_SELECT_DIFFICULTY_BACKGROUND,
	GRAPHIC_SELECT_DIFFICULTY_EASY_HEADER,
	GRAPHIC_SELECT_DIFFICULTY_EASY_PICTURE,
	GRAPHIC_SELECT_DIFFICULTY_EXPLANATION,
	GRAPHIC_SELECT_DIFFICULTY_HARD_HEADER,
	GRAPHIC_SELECT_DIFFICULTY_HARD_PICTURE,
	GRAPHIC_SELECT_DIFFICULTY_MEDIUM_HEADER,
	GRAPHIC_SELECT_DIFFICULTY_MEDIUM_PICTURE,
	GRAPHIC_SELECT_DIFFICULTY_ONI_HEADER,
	GRAPHIC_SELECT_DIFFICULTY_ONI_PICTURE,
	GRAPHIC_SELECT_DIFFICULTY_ENDLESS_HEADER,
	GRAPHIC_SELECT_DIFFICULTY_ENDLESS_PICTURE,
	GRAPHIC_SELECT_DIFFICULTY_OK,
	GRAPHIC_SELECT_DIFFICULTY_TOP_EDGE,
	GRAPHIC_SELECT_GAME_BACKGROUND,
	GRAPHIC_SELECT_GAME_TOP_EDGE,
	GRAPHIC_SELECT_GROUP_BACKGROUND,
	GRAPHIC_SELECT_GROUP_BUTTON,
	GRAPHIC_SELECT_GROUP_CONTENTS_HEADER,
	GRAPHIC_SELECT_GROUP_EXPLANATION,
	GRAPHIC_SELECT_GROUP_INFO_FRAME,
	GRAPHIC_SELECT_GROUP_TOP_EDGE,
	GRAPHIC_SELECT_MUSIC_BACKGROUND,
	GRAPHIC_SELECT_MUSIC_DIFFICULTY_FRAME,
	GRAPHIC_SELECT_MUSIC_DIFFICULTY_ICONS,
	GRAPHIC_SELECT_MUSIC_INFO_FRAME,
	GRAPHIC_SELECT_MUSIC_METER_FRAME,
	GRAPHIC_SELECT_MUSIC_RADAR_BASE,
	GRAPHIC_SELECT_MUSIC_RADAR_WORDS,
	GRAPHIC_SELECT_MUSIC_ROULETTE_BANNER,
	GRAPHIC_SELECT_MUSIC_SCORE_FRAME,
	GRAPHIC_SELECT_MUSIC_SECTION_BAR,
	GRAPHIC_SELECT_MUSIC_SCROLLBAR,		
	GRAPHIC_SELECT_MUSIC_SECTION_BANNER,
	GRAPHIC_SELECT_MUSIC_SMALL_GRADES,	
	GRAPHIC_SELECT_MUSIC_SONG_BAR,
	GRAPHIC_SELECT_MUSIC_SONG_HIGHLIGHT,
	GRAPHIC_SELECT_MUSIC_TOP_EDGE,
	GRAPHIC_SELECT_MUSIC_LONG_BALLOON,
	GRAPHIC_SELECT_MUSIC_MARATHON_BALLOON,
	GRAPHIC_SELECT_STYLE_BACKGROUND,
	GRAPHIC_SELECT_STYLE_TOP_EDGE,
	GRAPHIC_SELECT_STYLE_EXPLANATION,
	GRAPHIC_SELECT_STYLE_ICONS,
	GRAPHIC_SELECT_STYLE_INFO_GAME_0_STYLE_0,
	GRAPHIC_SELECT_STYLE_INFO_GAME_0_STYLE_1,
	GRAPHIC_SELECT_STYLE_INFO_GAME_0_STYLE_2,
	GRAPHIC_SELECT_STYLE_INFO_GAME_0_STYLE_3,
	GRAPHIC_SELECT_STYLE_INFO_GAME_0_STYLE_4,
	GRAPHIC_SELECT_STYLE_INFO_GAME_1_STYLE_0,
	GRAPHIC_SELECT_STYLE_INFO_GAME_1_STYLE_1,
	GRAPHIC_SELECT_STYLE_INFO_GAME_1_STYLE_2,
	GRAPHIC_SELECT_STYLE_INFO_GAME_2_STYLE_0,
	GRAPHIC_SELECT_STYLE_INFO_GAME_2_STYLE_1,
	GRAPHIC_SELECT_STYLE_INFO_GAME_2_STYLE_2,
	GRAPHIC_SELECT_STYLE_INFO_GAME_2_STYLE_3,
	GRAPHIC_SELECT_STYLE_PREVIEW_GAME_0_STYLE_0,
	GRAPHIC_SELECT_STYLE_PREVIEW_GAME_0_STYLE_1,
	GRAPHIC_SELECT_STYLE_PREVIEW_GAME_0_STYLE_2,
	GRAPHIC_SELECT_STYLE_PREVIEW_GAME_0_STYLE_3,
	GRAPHIC_SELECT_STYLE_PREVIEW_GAME_0_STYLE_4,
	GRAPHIC_SELECT_STYLE_PREVIEW_GAME_1_STYLE_0,
	GRAPHIC_SELECT_STYLE_PREVIEW_GAME_1_STYLE_1,
	GRAPHIC_SELECT_STYLE_PREVIEW_GAME_1_STYLE_2,
	GRAPHIC_SELECT_STYLE_PREVIEW_GAME_2_STYLE_0,
	GRAPHIC_SELECT_STYLE_PREVIEW_GAME_2_STYLE_1,
	GRAPHIC_SELECT_STYLE_PREVIEW_GAME_2_STYLE_2,
	GRAPHIC_SELECT_STYLE_PREVIEW_GAME_2_STYLE_3,
	GRAPHIC_SONG_OPTIONS_BACKGROUND,
	GRAPHIC_SONG_OPTIONS_TOP_EDGE,
	GRAPHIC_STAGE_ENDLESS,
	GRAPHIC_STAGE_EXTRA1,
	GRAPHIC_STAGE_EXTRA2,
	GRAPHIC_STAGE_FINAL,
	GRAPHIC_STAGE_NUMBERS,
	GRAPHIC_STAGE_ONI,
	GRAPHIC_STAGE_STAGE,
	GRAPHIC_TITLE_MENU_BACKGROUND,
	GRAPHIC_TITLE_MENU_LOGO_GAME_0,
	GRAPHIC_TITLE_MENU_LOGO_GAME_1,
	GRAPHIC_TITLE_MENU_LOGO_GAME_2,

	SOUND_ATTRACT_INSERT_COIN,	
	SOUND_EDIT_CHANGE_BPM,	
	SOUND_EDIT_CHANGE_FREEZE,	
	SOUND_EDIT_CHANGE_LINE,	
	SOUND_EDIT_CHANGE_SNAP,	
	SOUND_EDIT_SAVE,	
	SOUND_EVALUATION_EXTRA_STAGE,	
	SOUND_INSERT_COIN,
	SOUND_GAMEPLAY_ASSIST_TICK,	
	SOUND_GAMEPLAY_FAILED,	
	SOUND_GAMEPLAY_ONI_GAIN_LIFE,	
	SOUND_GAMEPLAY_ONI_LOSE_LIFE,	
	SOUND_GAMEPLAY_ONI_DIE,	
	"Sounds","menu back",	
	SOUND_MENU_INVALID,	
	SOUND_MENU_MUSIC,	
	SOUND_MENU_PROMPT,	
	"Sounds","menu start",	
	SOUND_MENU_SWOOSH,
	SOUND_MENU_TIMER,
	SOUND_MUSIC_SCROLL_MUSIC,
	SOUND_OPTION_CHANGE_COL,	
	SOUND_OPTION_CHANGE_ROW,	
	SOUND_SELECT_DIFFICULTY_CHANGE,	
	SOUND_SELECT_GROUP_CHANGE,	
	SOUND_SELECT_MUSIC_CHANGE_MUSIC,	
	SOUND_SELECT_MUSIC_CHANGE_SORT,	
	SOUND_SELECT_MUSIC_CHANGE_NOTES,	
	SOUND_SELECT_MUSIC_CHANGE_OPTIONS,	
	SOUND_SELECT_MUSIC_WHEEL_LOCKED,	
	SOUND_SELECT_MUSIC_SECTION_EXPAND,	
	SOUND_SELECT_STYLE_CHANGE,	
	SOUND_TITLE_MENU_CHANGE,

	"Fonts","header1",	
	"Fonts","Header2",	
	"Fonts","normal",
	FONT_ITALIC,
	FONT_COMBO_NUMBERS,	
	"Fonts","meter",
	"Fonts","score numbers",	
	"Fonts","timer numbers",	
	"Fonts","text banner",	
	FONT_STAGE,	
*/
	NUM_THEME_ELEMENTS	// leave this at the end
};


class ThemeManager
{
public:
	ThemeManager();
	~ThemeManager();

	void GetThemeNamesForCurGame( CStringArray& AddTo );
	bool DoesThemeExist( CString sThemeName );
	void AssertThemeIsComplete( CString sThemeName );
	void SwitchTheme( CString sThemeName );
	CString GetCurThemeName() { return m_sCurThemeName; };

	CString GetPathTo( ThemeElement te );	// looks up the current theme in PREFSMAN
	CString GetPathTo( CString sAssetCategory, CString sFileName );	// looks up the current theme in PREFSMAN

	CString		GetMetric( CString sScreenName, CString sValueName );
	int			GetMetricI( CString sScreenName, CString sValueName );
	float		GetMetricF( CString sScreenName, CString sValueName );
	bool		GetMetricB( CString sScreenName, CString sValueName );
	D3DXCOLOR	GetMetricC( CString sScreenName, CString sValueName );

protected:
	void GetAllThemeNames( CStringArray& AddTo );

	CString GetPathTo( ThemeElement te, CString sThemeName );

	static CString GetThemeDirFromName( CString sThemeName );
	CString GetElementDir( CString sThemeName );
	static CString GetMetricsPathFromName( CString sThemeName );

	CString m_sCurThemeName;

	IniFile* m_pIniMetrics;	// make this a pointer so we don't have to include IniFile in this header!
	int m_iHashForCurThemeMetrics;
	int m_iHashForBaseThemeMetrics;
};



extern ThemeManager*	THEME;	// global and accessable from anywhere in our program
