#include "stdafx.h"
/*
-----------------------------------------------------------------------------
 Class: StyleDef

 Desc: A data structure that holds the definition of a GameMode.

 Copyright (c) 2001-2002 by the person(s) listed below.  All rights reserved.
	Chris Danford
-----------------------------------------------------------------------------
*/

#include "StyleDef.h"
#include "RageLog.h"
#include "RageUtil.h"
#include "GameDef.h"
#include "IniFile.h"


void StyleDef::GetTransformedNoteDataForStyle( PlayerNumber p, NoteData* pOriginal, NoteData* pNoteDataOut )
{
	int iNewToOriginalTrack[MAX_COLS_PER_PLAYER];
	for( int col=0; col<m_iColsPerPlayer; col++ )
	{
		ColumnInfo colInfo = m_ColumnInfo[p][col];
		int originalTrack = colInfo.track;
		
		iNewToOriginalTrack[col] = originalTrack;
	}
	
	pNoteDataOut->LoadTransformed( pOriginal, m_iColsPerPlayer, iNewToOriginalTrack );
}


GameInput StyleDef::StyleInputToGameInput( const StyleInput StyleI )
{
	GameController c = m_ColumnInfo[StyleI.player][StyleI.col].controller;
	GameButton b = m_ColumnInfo[StyleI.player][StyleI.col].button;
	return GameInput( c, b );
};

StyleInput StyleDef::GameInputToStyleInput( const GameInput &GameI )
{
	for( int p=0; p<NUM_PLAYERS; p++ )
	{
		for( int t=0; t<MAX_NOTE_TRACKS; t++ )
		{
			if( m_ColumnInfo[p][t].controller == GameI.controller  &&
				m_ColumnInfo[p][t].button == GameI.button )
			{
				return StyleInput( (PlayerNumber)p, t );
			}
		}
	}
	return StyleInput();	// Didn't find a match.  Return invalid.
}


