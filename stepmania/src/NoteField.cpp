#include "stdafx.h"
/*
-----------------------------------------------------------------------------
 Class: NoteField

 Desc: See header.

 Copyright (c) 2001-2002 by the person(s) listed below.  All rights reserved.
	Chris Danford
-----------------------------------------------------------------------------
*/

#include "NoteField.h"
#include "RageUtil.h"
#include "GameConstantsAndTypes.h"
#include "PrefsManager.h"
#include "ArrowEffects.h"
#include "PrefsManager.h"
#include "GameManager.h"
#include "GameState.h"
#include "RageException.h"
#include "RageTimer.h"


const float HOLD_NOTE_BITS_PER_BEAT	= 6;
const float HOLD_NOTE_BITS_PER_ROW	= HOLD_NOTE_BITS_PER_BEAT / ELEMENTS_PER_BEAT;
const float ROWS_BETWEEN_HOLD_BITS	= 1 / HOLD_NOTE_BITS_PER_ROW;	

NoteField::NoteField()
{
	m_rectMeasureBar.TurnShadowOff();

	m_textMeasureNumber.LoadFromFont( THEME->GetPathTo("Fonts","normal") );
	m_textMeasureNumber.SetZoom( 1.0f );

	m_rectMarkerBar.TurnShadowOff();
	m_rectMarkerBar.SetEffectGlowing();

	m_fBeginMarker = m_fEndMarker = -1;

	m_fPercentFadeToFail = -1;
}


void NoteField::Load( NoteData* pNoteData, PlayerNumber pn, int iPixelsToDrawBehind, int iPixelsToDrawAhead )
{
	m_PlayerNumber = pn;
	m_iPixelsToDrawBehind = iPixelsToDrawBehind;
	m_iPixelsToDrawAhead = iPixelsToDrawAhead;

	m_fPercentFadeToFail = -1;

	NoteDataWithScoring::Init();

	for( int i=0; i<MAX_HOLD_NOTES; i++ )
		m_bIsHoldingHoldNote[i] = false;

	StyleDef* pStyleDef = GAMESTATE->GetCurrentStyleDef();

	this->CopyAll( pNoteData );

	// init note displays
	for( int c=0; c<m_iNumTracks; c++ ) 
		m_NoteDisplay[c].Load( c, pn );


	for( i=0; i<MAX_HOLD_NOTES; i++ )
		m_fHoldNoteLife[i] = 1;		// start with full life


	ASSERT( m_iNumTracks == GAMESTATE->GetCurrentStyleDef()->m_iColsPerPlayer );
}

void NoteField::Update( float fDeltaTime )
{
	m_rectMarkerBar.Update( fDeltaTime );

	if( m_fPercentFadeToFail >= 0 )
		m_fPercentFadeToFail = min( m_fPercentFadeToFail + fDeltaTime/3, 1 );	// take 3 seconds to totally fade
}




/*
void NoteField::CreateTapNoteInstance( ColorNoteInstance &cni, const int iCol, const float fIndex, const bool bUseHoldNoteBeginColor )
{
	const float fYOffset	= ArrowGetYOffset(	m_PlayerNumber, fIndex );
	const float fYPos		= ArrowGetYPos(		m_PlayerNumber, fYOffset );
	const float fRotation	= ArrowGetRotation(	m_PlayerNumber, iCol, fYOffset );
	const float fXPos		= ArrowGetXPos(		m_PlayerNumber, iCol, fYOffset );
	      float fAlpha		= ArrowGetAlpha(	m_PlayerNumber, fYPos );

	if( m_fPercentFadeToFail != -1 )
		fAlpha = 1-m_fPercentFadeToFail;

	D3DXCOLOR colorLeading, colorTrailing;	// of the color part.  Alpha here be overwritten with fAlpha!
	if( bUseHoldNoteBeginColor )
		colorLeading = colorTrailing = m_ColorNote[iCol].GetHoldColorFromPercentIntoHold( 0 );
	else
		m_ColorNote[iCol].GetEdgeColorsFromIndexAndBeat( roundf(fIndex), colorLeading, colorTrailing );

	float fAddAlpha = m_ColorNote[iCol].GetAddAlphaFromDiffuseAlpha( fAlpha );
	int iColorPartFrameNo = m_ColorNote[iCol].GetColorPartFrameNoFromIndexAndBeat( roundf(fIndex), GAMESTATE->m_fSongBeat );
	int iGrayPartFrameNo  = m_ColorNote[iCol].GetGrayPartFrameNoFromIndexAndBeat(  roundf(fIndex), GAMESTATE->m_fSongBeat );

	if( iCol == 2 )
		printf( "iColorPartFrameNo = %d\n", iColorPartFrameNo );

	cni = ColorNoteInstance( fXPos, fYPos, fRotation, fAlpha, colorLeading, colorTrailing, fAddAlpha, iColorPartFrameNo, iGrayPartFrameNo );
}

void NoteField::CreateHoldNoteInstance( ColorNoteInstance &cni, const bool bActive, const float fIndex, const HoldNote &hn, const float fHoldNoteLife )
{
}
*/

void NoteField::DrawMeasureBar( int iMeasureIndex )
{
	const int iMeasureNoDisplay = iMeasureIndex+1;
	const float fBeat = float(iMeasureIndex * BEATS_PER_MEASURE);

	const float fYOffset	= ArrowGetYOffset2(	m_PlayerNumber, fBeat );
	const float fYPos		= ArrowGetYPos(		m_PlayerNumber, fYOffset );

	m_rectMeasureBar.SetXY( 0, fYPos );
	m_rectMeasureBar.SetWidth( (float)(m_iNumTracks+1) * ARROW_SIZE );
	m_rectMeasureBar.SetHeight( 20 );
	m_rectMeasureBar.SetDiffuseColor( D3DXCOLOR(0,0,0,0.5f) );
	m_rectMeasureBar.Draw();

	m_textMeasureNumber.SetDiffuseColor( D3DXCOLOR(1,1,1,1) );
	m_textMeasureNumber.SetAddColor( D3DXCOLOR(1,1,1,0) );
	m_textMeasureNumber.SetText( ssprintf("%d", iMeasureNoDisplay) );
	m_textMeasureNumber.SetXY( -m_rectMeasureBar.GetZoomedWidth()/2 + 10, fYPos );
	m_textMeasureNumber.Draw();
}

void NoteField::DrawMarkerBar( const float fBeat )
{
	const float fYOffset	= ArrowGetYOffset2(	m_PlayerNumber, fBeat );
	const float fYPos		= ArrowGetYPos(		m_PlayerNumber, fYOffset );

	m_rectMarkerBar.SetXY( 0, fYPos );
	m_rectMarkerBar.SetWidth( (float)(m_iNumTracks+1) * ARROW_SIZE );
	m_rectMarkerBar.SetHeight( 20 );
	m_rectMarkerBar.SetDiffuseColor( D3DXCOLOR(0,0,0,0.5f) );
	m_rectMarkerBar.Draw();
}

void NoteField::DrawBPMText( const float fBeat, const float fBPM )
{
	const float fYOffset	= ArrowGetYOffset2(	m_PlayerNumber, fBeat );
	const float fYPos		= ArrowGetYPos(		m_PlayerNumber, fYOffset );

	m_textMeasureNumber.SetDiffuseColor( D3DXCOLOR(1,0,0,1) );
	m_textMeasureNumber.SetAddColor( D3DXCOLOR(1,1,1,cosf(TIMER->GetTimeSinceStart()*2)/2+0.5f) );
	m_textMeasureNumber.SetText( ssprintf("%.2f", fBPM) );
	m_textMeasureNumber.SetXY( -m_rectMeasureBar.GetZoomedWidth()/2 - 60, fYPos );
	m_textMeasureNumber.Draw();
}

void NoteField::DrawFreezeText( const float fBeat, const float fSecs )
{
	const float fYOffset	= ArrowGetYOffset2(	m_PlayerNumber, fBeat );
	const float fYPos		= ArrowGetYPos(		m_PlayerNumber, fYOffset );

	m_textMeasureNumber.SetDiffuseColor( D3DXCOLOR(0.8f,0.8f,0,1) );
	m_textMeasureNumber.SetAddColor( D3DXCOLOR(1,1,1,cosf(TIMER->GetTimeSinceStart()*2)/2+0.5f) );
	m_textMeasureNumber.SetText( ssprintf("%.2f", fSecs) );
	m_textMeasureNumber.SetXY( -m_rectMeasureBar.GetZoomedWidth()/2 - 10, fYPos );
	m_textMeasureNumber.Draw();
}

void NoteField::DrawPrimitives()
{
	//LOG->Trace( "NoteField::DrawPrimitives()" );

	float fSongBeat = max( 0, GAMESTATE->m_fSongBeat );
	
	const float fBeatsToDrawBehind = m_iPixelsToDrawBehind * (1/(float)ARROW_SIZE) * (1/GAMESTATE->m_PlayerOptions[m_PlayerNumber].m_fArrowScrollSpeed);
	const float fBeatsToDrawAhead  = m_iPixelsToDrawAhead  * (1/(float)ARROW_SIZE) * (1/GAMESTATE->m_PlayerOptions[m_PlayerNumber].m_fArrowScrollSpeed);
	const float fFirstBeatToDraw = max( 0, fSongBeat - fBeatsToDrawBehind );
	const float fLastBeatToDraw  = fSongBeat + fBeatsToDrawAhead;
	const int iFirstIndexToDraw  = BeatToNoteRow(fFirstBeatToDraw);
	const int iLastIndexToDraw   = BeatToNoteRow(fLastBeatToDraw);

	//LOG->Trace( "Drawing elements %d through %d", iIndexFirstArrowToDraw, iIndexLastArrowToDraw );

	if( GAMESTATE->m_bEditing )
	{
		int i;

		//
		// Draw measure bars
		//
		for( float b=fFirstBeatToDraw; b<=fLastBeatToDraw; b+=1 )
			DrawMeasureBar( int(b) );

		//
		// BPM text
		//
		CArray<BPMSegment,BPMSegment&> &aBPMSegments = GAMESTATE->m_pCurSong->m_BPMSegments;
		for( i=0; i<aBPMSegments.GetSize(); i++ )
			DrawBPMText( aBPMSegments[i].m_fStartBeat, aBPMSegments[i].m_fBPM );

		//
		// Freeze text
		//
		CArray<StopSegment,StopSegment&> &aStopSegments = GAMESTATE->m_pCurSong->m_StopSegments;
		for( i=0; i<aStopSegments.GetSize(); i++ )
			DrawFreezeText( aStopSegments[i].m_fStartBeat, aStopSegments[i].m_fStopSeconds );

		//
		// Draw marker bars
		//
		if( m_fBeginMarker != -1 )
			DrawMarkerBar( m_fBeginMarker );
		if( m_fEndMarker != -1 )
			DrawMarkerBar( m_fEndMarker );

	}


	//
	// Optimization is very important here because there are so many arrows to draw.  We're going 
	// to draw the arrows in order of column.  This will let us fill up a vertex buffer of arrows 
	// so we can draw them in one swoop without texture or state changes.
	//
	// Change:  Optimization made this code very unreadable and unmanagable, so we're going to 
	// try a more global, higher-level optimization.  Our goal is to make fewer calls to 
	// DrawPrimitive.  Instead of filling the vertex buffer all at once (the old optimization)
	// we'll add something to Display that will automatically group together DrawPrimitive 
	// calls as long as the texture or render states have not changed.
	//
	// To mimimize state and texture changes, we're going to draw one column at a time:
	// HoldNotes, then TapNotes.
	//



	for( int c=0; c<m_iNumTracks; c++ )	// for each arrow column
	{
//		const int MAX_COLOR_NOTE_INSTANCES = 300;
//		ColorNoteInstance instances[MAX_COLOR_NOTE_INSTANCES];
//		int iCount = 0;		// number of valid elements in the instances array



		/////////////////////////////////
		// Draw all HoldNotes in this column (so that they appear under the tap notes)
		/////////////////////////////////
		for( int i=0; i<m_iNumHoldNotes; i++ )
		{
			HoldNote &hn = m_HoldNotes[i];
			HoldNoteScore &hns = m_HoldNoteScores[i];
			const float fLife = m_fHoldNoteLife[i];
			const bool bIsHoldingNote = m_bIsHoldingHoldNote[i];	// hack: added -1 because hn.m_iStartIndex changes as note is held
			
			if( hns == HNS_OK )	// if this HoldNote was completed
				continue;	// don't draw anything

			if( hn.m_iTrack != c )	// this HoldNote doesn't belong to this column
				continue;

			// If no part of this HoldNote is on the screen, skip it
			if( !( fFirstBeatToDraw <= hn.m_fEndBeat && hn.m_fEndBeat <= fLastBeatToDraw  ||
				fFirstBeatToDraw <= hn.m_fStartBeat  && hn.m_fStartBeat <= fLastBeatToDraw  ||
				hn.m_fStartBeat < fFirstBeatToDraw   && hn.m_fEndBeat > fLastBeatToDraw ) )
			{
				continue;	// skip
			}


			m_NoteDisplay[c].DrawHold( hn, bIsHoldingNote, fLife, m_fPercentFadeToFail );

//			// If this note was in the past and has life > 0, then it was completed and don't draw it!
//			if( hn.m_iEndIndex < BeatToNoteRow(fSongBeat)  &&  fLife > 0  &&  m_bIsHoldingHoldNote[i] )
//				continue;	// skip

			/*
			// parts of the hold
			const float fStartDrawingAtBeat = froundf( (float)hn.m_iStartIndex, ROWS_BETWEEN_HOLD_BITS/GAMESTATE->m_PlayerOptions[m_PlayerNumber].m_fArrowScrollSpeed );
			for( float j=fStartDrawingAtBeat; 
				 j<=hn.m_iEndIndex; 
				 j+=ROWS_BETWEEN_HOLD_BITS/GAMESTATE->m_PlayerOptions[m_PlayerNumber].m_fArrowScrollSpeed )	// for each bit of the hold
			{
 				// check if this arrow is off the the screen
				if( j < iIndexFirstArrowToDraw || iIndexLastArrowToDraw < j)
					continue;	// skip this arrow

				if( bActive  &&  NoteRowToBeat(j) < fSongBeat )
					continue;

				CreateHoldNoteInstance( instances[iCount++], bActive, (float)j, hn, fHoldNoteLife );
			}
			*/

		}
		
//		const bool bDrawAddPass = GAMESTATE->m_PlayerOptions[m_PlayerNumber].m_AppearanceType != PlayerOptions::APPEARANCE_VISIBLE;
//		if( iCount > 0 )
//			m_ColorNote[c].DrawList( iCount, instances, bDrawAddPass );



//		iCount = 0;		// reset count

		///////////////////////////////////
		// Draw all TapNotes in this column
		///////////////////////////////////
		for( i=iFirstIndexToDraw; i<=iLastIndexToDraw; i++ )	//	 for each row
		{	
			if( m_TapNotes[c][i] == '0' )	// no note here
				continue;	// skip
			
			if( m_TapNotes[c][i] == '2' )	// this is a HoldNote begin marker.  Grade it, but don't draw
				continue;	// skip

			// See if there is a hold step that begins on this index.
			bool bHoldNoteBeginsOnThisBeat = false;
			for( int c2=0; c2<m_iNumTracks; c2++ )
			{
				if( m_TapNotes[c2][i] == '2' )
				{
					bHoldNoteBeginsOnThisBeat = true;
					break;
				}
			}



			m_NoteDisplay[c].DrawTap( c, NoteRowToBeat(i), bHoldNoteBeginsOnThisBeat, m_fPercentFadeToFail );
		}
	}

}




void NoteField::RemoveTapNoteRow( int iIndex )
{
	for( int c=0; c<m_iNumTracks; c++ )
		m_TapNotes[c][iIndex] = '0';
}

void NoteField::FadeToFail()
{
	m_fPercentFadeToFail = max( 0.0f, m_fPercentFadeToFail );	// this will slowly increase every Update()
		// don't fade all over again if this is called twice
}