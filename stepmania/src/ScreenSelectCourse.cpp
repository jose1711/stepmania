#include "stdafx.h"
/*
-----------------------------------------------------------------------------
 Class: ScreenSelectCourse

 Desc: See header.

 Copyright (c) 2001-2002 by the person(s) listed below.  All rights reserved.
	Chris Danford
-----------------------------------------------------------------------------
*/


#include "ScreenSelectCourse.h"
#include "ScreenManager.h"
#include "PrefsManager.h"
#include "SongManager.h"
#include "GameManager.h"
#include "RageMusic.h"
#include "ScreenTitleMenu.h"
#include "GameConstantsAndTypes.h"
#include "PrefsManager.h"
#include "ScreenGameplay.h"
#include "ScreenPrompt.h"
#include "ScreenPlayerOptions.h"
#include "RageLog.h"
#include "InputMapper.h"
#include "InputQueue.h"
#include "ScreenStage.h"
#include "AnnouncerManager.h"
#include "GameState.h"
#include "RageMusic.h"


const float COURSE_INFO_FRAME_X	= 160;
const float COURSE_INFO_FRAME_Y	= SCREEN_TOP+118;

const float COURSE_CONTENTS_FRAME_X	= 160;
const float COURSE_CONTENTS_FRAME_Y	= CENTER_Y+70;

const float WHEEL_X		= CENTER_X+160;
const float WHEEL_Y		= CENTER_Y+8;

const float TWEEN_TIME		= 0.5f;



const ScreenMessage SM_GoToPrevState		=	ScreenMessage(SM_User+1);
const ScreenMessage SM_GoToNextState		=	ScreenMessage(SM_User+2);
const ScreenMessage SM_ConfirmChange		=	ScreenMessage(SM_User+3);



ScreenSelectCourse::ScreenSelectCourse()
{
	LOG->Trace( "ScreenSelectCourse::ScreenSelectCourse()" );

	if( !MUSIC->IsPlaying()  ||  MUSIC->GetLoadedFilePath() != THEME->GetPathTo("Sounds","select course music") )
	{
		MUSIC->Load( THEME->GetPathTo("Sounds","select course music") );
		MUSIC->Play( true );
	}

	m_bMadeChoice = false;
	m_bGoToOptions = false;

	m_Menu.Load(
		THEME->GetPathTo("Graphics","select course background"), 
		THEME->GetPathTo("Graphics","select course top edge"),
		ssprintf("%c or %c change course    then press START", char(1), char(2)),
		false, true, 60 
		);
	this->AddSubActor( &m_Menu );

	m_CourseInfoFrame.SetXY( COURSE_INFO_FRAME_X, COURSE_INFO_FRAME_Y );
	this->AddSubActor( &m_CourseInfoFrame );

	m_CourseContentsFrame.SetXY( COURSE_CONTENTS_FRAME_X, COURSE_CONTENTS_FRAME_Y );
	this->AddSubActor( &m_CourseContentsFrame );

	m_MusicWheel.SetXY( WHEEL_X, WHEEL_Y );
	this->AddSubActor( &m_MusicWheel );

	m_textHoldForOptions.LoadFromFont( THEME->GetPathTo("Fonts","Stage") );
	m_textHoldForOptions.SetXY( CENTER_X, CENTER_Y );
	m_textHoldForOptions.SetText( "press START again for options" );
	m_textHoldForOptions.SetZoom( 1 );
	m_textHoldForOptions.SetZoomY( 0 );
	m_textHoldForOptions.SetDiffuseColor( D3DXCOLOR(1,1,1,0) );
	this->AddSubActor( &m_textHoldForOptions );


	m_soundSelect.Load( THEME->GetPathTo("Sounds","menu start") );
	m_soundChangeNotes.Load( THEME->GetPathTo("Sounds","select music change notes") );

	SOUND->PlayOnceStreamedFromDir( ANNOUNCER->GetPathTo(ANNOUNCER_SELECT_COURSE_INTRO) );


	AfterCourseChange();
	TweenOnScreen();
	m_Menu.TweenOnScreenFromMenu( SM_None );
}


ScreenSelectCourse::~ScreenSelectCourse()
{
	LOG->Trace( "ScreenSelectCourse::~ScreenSelectCourse()" );

}

void ScreenSelectCourse::DrawPrimitives()
{
	m_Menu.DrawBottomLayer();
	Screen::DrawPrimitives();
	m_Menu.DrawTopLayer();
}

void ScreenSelectCourse::TweenOnScreen()
{
	m_CourseInfoFrame.SetXY( COURSE_INFO_FRAME_X - 400, COURSE_INFO_FRAME_Y );
	m_CourseInfoFrame.BeginTweening( TWEEN_TIME, Actor::TWEEN_BIAS_END );
	m_CourseInfoFrame.SetTweenXY( COURSE_INFO_FRAME_X, COURSE_INFO_FRAME_Y );

	m_CourseContentsFrame.SetXY( COURSE_CONTENTS_FRAME_X - 400, COURSE_CONTENTS_FRAME_Y );
	m_CourseContentsFrame.BeginTweeningQueued( TWEEN_TIME, Actor::TWEEN_BIAS_END );
	m_CourseContentsFrame.SetTweenXY( COURSE_CONTENTS_FRAME_X, COURSE_CONTENTS_FRAME_Y );

	m_MusicWheel.TweenOnScreen();
}

void ScreenSelectCourse::TweenOffScreen()
{
	m_CourseInfoFrame.BeginTweeningQueued( TWEEN_TIME*1.5f, Actor::TWEEN_BOUNCE_BEGIN );
	m_CourseInfoFrame.SetTweenXY( COURSE_INFO_FRAME_X - 400, COURSE_INFO_FRAME_Y );

	m_CourseContentsFrame.BeginTweeningQueued( TWEEN_TIME, Actor::TWEEN_BOUNCE_BEGIN );
	m_CourseContentsFrame.SetTweenXY( COURSE_CONTENTS_FRAME_X - 400, COURSE_CONTENTS_FRAME_Y );

	m_MusicWheel.TweenOffScreen();
}


void ScreenSelectCourse::Input( const DeviceInput& DeviceI, const InputEventType type, const GameInput &GameI, const MenuInput &MenuI, const StyleInput &StyleI )
{
	LOG->Trace( "ScreenSelectCourse::Input()" );

	if( m_Menu.IsClosing() )
		return;		// ignore

	if( MenuI.player == PLAYER_INVALID )
		return;

	if( m_bMadeChoice && !m_bGoToOptions && MenuI.button == MENU_BUTTON_START )
	{
		m_bGoToOptions = true;
		m_textHoldForOptions.SetText( "Entering Options..." );
		SOUND->PlayOnceStreamed( THEME->GetPathTo("Sounds","menu start") );
		return;
	}

	if( m_bMadeChoice )
		return;

	Screen::Input( DeviceI, type, GameI, MenuI, StyleI );	// default input handler
}


void ScreenSelectCourse::HandleScreenMessage( const ScreenMessage SM )
{
	Screen::HandleScreenMessage( SM );

	switch( SM )
	{
	case SM_MenuTimer:
		MenuStart(PLAYER_1);
		break;
	case SM_GoToPrevState:
		SCREENMAN->SetNewScreen( new ScreenTitleMenu );
		break;
	case SM_GoToNextState:

		// find out if the Next button is being held down on any of the pads
		bool bIsHoldingNext;
		bIsHoldingNext = false;
		int player;
		for( player=0; player<NUM_PLAYERS; player++ )
		{
			MenuInput mi( (PlayerNumber)player, MENU_BUTTON_START );
			if( INPUTMAPPER->IsButtonDown( mi ) )
				bIsHoldingNext = true;
		}

		if( bIsHoldingNext || m_bGoToOptions )
		{
			SCREENMAN->SetNewScreen( new ScreenPlayerOptions );
		}
		else
		{
			SCREENMAN->SetNewScreen( new ScreenStage );
		}
		break;
	}
}

void ScreenSelectCourse::MenuLeft( const PlayerNumber p, const InputEventType type )
{
	m_MusicWheel.PrevMusic();
	
	AfterCourseChange();
}


void ScreenSelectCourse::MenuRight( const PlayerNumber p, const InputEventType type )
{
	m_MusicWheel.NextMusic();

	AfterCourseChange();
}

void ScreenSelectCourse::MenuStart( const PlayerNumber p )
{
	// this needs to check whether valid Notes are selected!
	m_MusicWheel.Select();

	switch( m_MusicWheel.GetSelectedType() )
	{
	case TYPE_COURSE:
		SOUND->PlayOnceStreamedFromDir( ANNOUNCER->GetPathTo(ANNOUNCER_SELECT_MUSIC_COMMENT_GENERAL) );
	
		TweenOffScreen();

		m_soundSelect.PlayRandom();

		m_bMadeChoice = true;

		// show "hold START for options"
		m_textHoldForOptions.SetDiffuseColor( D3DXCOLOR(1,1,1,0) );
		m_textHoldForOptions.BeginTweeningQueued( 0.25f );	// fade in
		m_textHoldForOptions.SetTweenZoomY( 1 );
		m_textHoldForOptions.SetTweenDiffuseColor( D3DXCOLOR(1,1,1,1) );
		m_textHoldForOptions.BeginTweeningQueued( 2.0f );	// sleep
		m_textHoldForOptions.BeginTweeningQueued( 0.25f );	// fade out
		m_textHoldForOptions.SetTweenDiffuseColor( D3DXCOLOR(1,1,1,0) );
		m_textHoldForOptions.SetTweenZoomY( 0 );

		m_Menu.TweenOffScreenToBlack( SM_None, false );

		Course* pCourse = m_MusicWheel.GetSelectedCourse();
		GAMESTATE->m_pCurCourse = pCourse;
		for( int p=0; p<NUM_PLAYERS; p++ )
			pCourse->GetPlayerOptions( &GAMESTATE->m_PlayerOptions[p] );
		pCourse->GetSongOptions( &GAMESTATE->m_SongOptions );

		this->SendScreenMessage( SM_GoToNextState, 2.5f );
		
		break;
	}
}


void ScreenSelectCourse::MenuBack( const PlayerNumber p )
{
	MUSIC->Stop();

	m_Menu.TweenOffScreenToBlack( SM_GoToPrevState, true );
}


void ScreenSelectCourse::AfterCourseChange()
{
	switch( m_MusicWheel.GetSelectedType() )
	{
	case TYPE_COURSE:
		{
			Course* pCourse = m_MusicWheel.GetSelectedCourse();
			m_CourseInfoFrame.SetFromCourse( pCourse );
			m_CourseContentsFrame.SetFromCourse( pCourse );
		}
		break;
	case TYPE_SECTION:	// this probably means "no courses"
		break;
	default:
		ASSERT(0);
	}
}

