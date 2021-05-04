/*
===========================================================================
Copyright (C) 1999 - 2005, Id Software, Inc.
Copyright (C) 2000 - 2013, Raven Software, Inc.
Copyright (C) 2001 - 2013, Activision, Inc.
Copyright (C) 2013 - 2015, OpenJK contributors

This file is part of the OpenJK source code.

OpenJK is free software; you can redistribute it and/or modify it
under the terms of the GNU General Public License version 2 as
published by the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, see <http://www.gnu.org/licenses/>.
===========================================================================
*/

#include "cg_headers.h"

#include "cg_media.h"
#include "../game/objectives.h"

// For printing objectives
static const short objectiveStartingYpos = 75;		// Y starting position for objective text
static const short objectiveStartingXpos = 60;		// X starting position for objective text
static const int objectiveTextBoxWidth = 500;		// Width (in pixels) of text box
static const int objectiveTextBoxHeight = 300;		// Height (in pixels) of text box
static const short missionYpos = 79;

const char *showLoadPowersName[] =
{
	"SP_INGAME_HEAL2",
	"SP_INGAME_JUMP2",
	"SP_INGAME_SPEED2",
	"SP_INGAME_PUSH2",
	"SP_INGAME_PULL2",
	"SP_INGAME_MINDTRICK2",
	"SP_INGAME_GRIP2",
	"SP_INGAME_LIGHTNING2",
	"SP_INGAME_SABER_THROW2",
	"SP_INGAME_SABER_OFFENSE2",
	"SP_INGAME_SABER_DEFENSE2",
	NULL,
};

#define MAX_OBJ_GRAPHICS 4
#define OBJ_GRAPHIC_SIZE 240
int obj_graphics[MAX_OBJ_GRAPHICS];

qboolean CG_ForcePower_Valid(int forceKnownBits, int index);

/*
====================
ObjectivePrint_Line

Print a single mission objective
====================
*/
static void ObjectivePrint_Line(const int color, const int objectIndex, int& missionYcnt)
{
	char* str, * strBegin;
	int y, pixelLen, charLen, i;
	const int maxHoldText = 1024;
	char holdText[maxHoldText];
	char finalText[2048];
	qhandle_t	graphic;

	int iYPixelsPerLine = cgi_R_Font_HeightPixels(cgs.media.qhFontMedium, 1.0f);

	cgi_SP_GetStringTextString(va("OBJECTIVES_%s", objectiveTable[objectIndex].name), finalText, sizeof(finalText));

	// A hack to be able to count prisoners
	if (objectIndex == T2_RANCOR_OBJ5)
	{
		char value[64];
		int	currTotal, minTotal;

		gi.Cvar_VariableStringBuffer("ui_prisonerobj_currtotal", value, sizeof(value));
		currTotal = atoi(value);
		gi.Cvar_VariableStringBuffer("ui_prisonerobj_maxtotal", value, sizeof(value));
		minTotal = atoi(value);

		Q_strncpyz(finalText, va(finalText, currTotal, minTotal), sizeof(finalText));
	}

	pixelLen = cgi_R_Font_StrLenPixels(finalText, cgs.media.qhFontMedium, 1.0f);

	str = finalText;

	if (cgi_Language_IsAsian())
	{
		// this is execrable, and should NOT have had to've been done now, but...
		//
		extern const char* CG_DisplayBoxedText(int iBoxX, int iBoxY, int iBoxWidth, int iBoxHeight,
			const char* psText, int iFontHandle, float fScale,
			const vec4_t v4Color);
		extern int giLinesOutput;
		extern float gfAdvanceHack;

		gfAdvanceHack = 1.0f;	// override internal vertical advance
		y = objectiveStartingYpos + (iYPixelsPerLine * missionYcnt);

		// Advance line if a graphic has printed
		for (i = 0; i < MAX_OBJ_GRAPHICS; i++)
		{
			if (obj_graphics[i])
			{
				y += OBJ_GRAPHIC_SIZE + 4;
			}
		}

		CG_DisplayBoxedText(
			objectiveStartingXpos,
			y,
			objectiveTextBoxWidth,
			objectiveTextBoxHeight,
			finalText,	// int iBoxX, int iBoxY, int iBoxWidth, int iBoxHeight, const char *psText
			cgs.media.qhFontMedium,		// int iFontHandle,
			1.0f,						// float fScale,
			colorTable[color]			// const vec4_t v4Color
		);

		gfAdvanceHack = 0.0f;	// restore
		missionYcnt += giLinesOutput;
	}
	else
	{
		// western...
		//
		if (pixelLen < objectiveTextBoxWidth)	// One shot - small enough to print entirely on one line
		{
			y = objectiveStartingYpos + (iYPixelsPerLine * (missionYcnt));

			cgi_R_Font_DrawString(
				objectiveStartingXpos,
				y,
				str,
				colorTable[color],
				cgs.media.qhFontMedium,
				-1,
				1.0f);

			++missionYcnt;
		}
		// Text is too long, break into lines.
		else
		{
			char holdText2[2];
			pixelLen = 0;
			charLen = 0;
			holdText2[1] = '\0';
			strBegin = str;

			while (*str)
			{
				holdText2[0] = *str;
				pixelLen += cgi_R_Font_StrLenPixels(holdText2, cgs.media.qhFontMedium, 1.0f);

				pixelLen += 2; // For kerning
				++charLen;

				if (pixelLen > objectiveTextBoxWidth)
				{	//Reached max length of this line
					//step back until we find a space
					while ((charLen > 10) && (*str != ' '))
					{
						--str;
						--charLen;
					}

					if (*str == ' ')
					{
						++str;	// To get past space
					}

					assert(charLen < maxHoldText);	// Too big?

					Q_strncpyz(holdText, strBegin, charLen);
					holdText[charLen] = '\0';
					strBegin = str;
					pixelLen = 0;
					charLen = 1;

					y = objectiveStartingYpos + (iYPixelsPerLine * missionYcnt);

					CG_DrawProportionalString(
						objectiveStartingXpos,
						y,
						holdText,
						CG_SMALLFONT,
						colorTable[color]);

					++missionYcnt;
				}
				else if (*(str + 1) == '\0')
				{
					++charLen;

					assert(charLen < maxHoldText);	// Too big?

					y = objectiveStartingYpos + (iYPixelsPerLine * missionYcnt);

					Q_strncpyz(holdText, strBegin, charLen);
					CG_DrawProportionalString(
						objectiveStartingXpos,
						y, holdText,
						CG_SMALLFONT,
						colorTable[color]);

					++missionYcnt;
					break;
				}
				++str;


			}
		}
	}

	if (objectIndex == T3_BOUNTY_OBJ1)
	{
		y = objectiveStartingYpos + (iYPixelsPerLine * missionYcnt);
		if (obj_graphics[1])
		{
			y += OBJ_GRAPHIC_SIZE + 4;
		}
		if (obj_graphics[2])
		{
			y += OBJ_GRAPHIC_SIZE + 4;
		}
		graphic = cgi_R_RegisterShaderNoMip("textures/system/viewscreen1");
		CG_DrawPic(355, 50, OBJ_GRAPHIC_SIZE, OBJ_GRAPHIC_SIZE, graphic);
		obj_graphics[3] = qtrue;
	}//// Special case hack
	else if (objectIndex == DOOM_COMM_OBJ4)
	{
		y = missionYpos + (iYPixelsPerLine * missionYcnt);
		graphic = cgi_R_RegisterShaderNoMip("textures/system/securitycode");
		CG_DrawPic(320 - (128 / 2), y + 8, 128, 32, graphic);
		obj_graphics[0] = qtrue;
	}
	else if (objectIndex == KEJIM_POST_OBJ3)
	{
		y = missionYpos + (iYPixelsPerLine * missionYcnt);
		graphic = cgi_R_RegisterShaderNoMip("textures/system/securitycode_red");
		CG_DrawPic(320 - (32 / 2), y + 8, 32, 32, graphic);
		obj_graphics[1] = qtrue;
	}
	else if (objectIndex == KEJIM_POST_OBJ4)
	{
		y = missionYpos + (iYPixelsPerLine * missionYcnt);
		if (obj_graphics[1])
		{
			y += 32 + 4;
		}
		graphic = cgi_R_RegisterShaderNoMip("textures/system/securitycode_green");
		CG_DrawPic(320 - (32 / 2), y + 8, 32, 32, graphic);
		obj_graphics[2] = qtrue;
	}
	else if (objectIndex == KEJIM_POST_OBJ5)
	{
		y = missionYpos + (iYPixelsPerLine * missionYcnt);
		if (obj_graphics[1])
		{
			y += 32 + 4;
		}
		if (obj_graphics[2])
		{
			y += 32 + 4;
		}
		graphic = cgi_R_RegisterShaderNoMip("textures/system/securitycode_blue");
		CG_DrawPic(320 - (32 / 2), y + 8, 32, 32, graphic);
		obj_graphics[3] = qtrue;
	}
}

/*
====================
CG_DrawDataPadObjectives

Draw routine for the objective info screen of the data pad.
====================
*/
void CG_DrawDataPadObjectives(const centity_t* cent)
{
	int		i, totalY;
	int		iYPixelsPerLine = cgi_R_Font_HeightPixels(cgs.media.qhFontMedium, 1.0f);

	const short titleXPos = objectiveStartingXpos - 22;		// X starting position for title text
	const short titleYPos = objectiveStartingYpos - 23;		// Y starting position for title text
	const short graphic_size = 16;							// Size (width and height) of graphic used to show status of objective
	const short graphicXpos = objectiveStartingXpos - graphic_size - 8;	// Amount of X to backup from text starting position
	const short graphicYOffset = (iYPixelsPerLine - graphic_size) / 2;	// Amount of Y to raise graphic so it's in the center of the text line

	missionInfo_Updated = qfalse;		// This will stop the text from flashing
	cg.missionInfoFlashTime = 0;

	// zero out objective graphics
	for (i = 0; i < MAX_OBJ_GRAPHICS; i++)
	{
		obj_graphics[i] = qfalse;
	}

	// Title Text at the top
	char text[1024] = { 0 };
	cgi_SP_GetStringTextString("SP_INGAME_OBJECTIVES", text, sizeof(text));
	cgi_R_Font_DrawString(titleXPos, titleYPos, text, colorTable[CT_TITLE], cgs.media.qhFontMedium, -1, 1.0f);

	int missionYcnt = 0;

	// Print all active objectives
	for (i = 0; i < MAX_OBJECTIVES; i++)
	{
		// Is there an objective to see?
		if (cent->gent->client->sess.mission_objectives[i].display)
		{
			// Calculate the Y position
			totalY = objectiveStartingYpos + (iYPixelsPerLine * (missionYcnt)) + (iYPixelsPerLine / 2);

			//	Draw graphics that show if mission has been accomplished or not
			cgi_R_SetColor(colorTable[CT_BLUE3]);
			CG_DrawPic((graphicXpos), (totalY - graphicYOffset), graphic_size, graphic_size, cgs.media.messageObjCircle);	// Circle in front
			if (cent->gent->client->sess.mission_objectives[i].status == OBJECTIVE_STAT_SUCCEEDED)
			{
				CG_DrawPic((graphicXpos), (totalY - graphicYOffset), graphic_size, graphic_size, cgs.media.messageLitOn);	// Center Dot
			}

			// Print current objective text
			ObjectivePrint_Line(CT_BLUE3, i, missionYcnt);
		}
	}

	// No mission text?
	if (!missionYcnt)
	{
		// Set the message a quarter of the way down and in the center of the text box
		int messageYPosition = objectiveStartingYpos + (objectiveTextBoxHeight / 4);

		cgi_SP_GetStringTextString("SP_INGAME_OBJNONE", text, sizeof(text));
		int messageXPosition = objectiveStartingXpos + (objectiveTextBoxWidth / 2) - (cgi_R_Font_StrLenPixels(text, cgs.media.qhFontMedium, 1.0f) / 2);

		cgi_R_Font_DrawString(
			messageXPosition,
			messageYPosition,
			text,
			colorTable[CT_LTBLUE1],
			cgs.media.qhFontMedium,
			-1,
			1.0f);
	}
}

static void CG_LoadBar(void)
{
	const int numticks = 9, tickwidth = 40, tickheight = 8;
	const int tickpadx = 20, tickpady = 12;
	const int capwidth = 8;
	const int barwidth = numticks*tickwidth+tickpadx*2+capwidth*2, barleft = ((640-barwidth)/2);
	const int barheight = tickheight + tickpady*2, bartop = 475-barheight;
	const int capleft = barleft+tickpadx, tickleft = capleft+capwidth, ticktop = bartop+tickpady;

	cgi_R_SetColor( colorTable[CT_WHITE]);
	// Draw background
	CG_DrawPic(barleft, bartop, barwidth, barheight, cgs.media.levelLoad);

	// Draw left cap (backwards)
	CG_DrawPic(tickleft, ticktop, -capwidth, tickheight, cgs.media.loadTickCap);

	// Draw bar
	CG_DrawPic(tickleft, ticktop, tickwidth*cg.loadLCARSStage, tickheight, cgs.media.loadTick);

	// Draw right cap
	CG_DrawPic(tickleft+tickwidth*cg.loadLCARSStage, ticktop, capwidth, tickheight, cgs.media.loadTickCap);
}

int CG_WeaponCheck( int weaponIndex );

// For printing load screen icons
const int	MAXLOADICONSPERROW = 8;		// Max icons displayed per row
const int	MAXLOADWEAPONS = WP_NUM_WEAPONS;
const int	MAXLOAD_FORCEICONSIZE = 40;	// Size of force power icons
const int	MAXLOAD_FORCEICONPAD = 12;	// Padding space between icons

static int CG_DrawLoadWeaponsPrintRow( const char *itemName, char *weapons,int rowIconCnt, int startIndex)
{
	int		i,endIndex=0, printedIconCnt=0;
	int		iconSize;
	int		holdX,x,y,pad;
	int		yOffset = 0;
	int		width,height;
	vec4_t	color;
	qhandle_t	background;

	if (!cgi_UI_GetMenuItemInfo(
		"loadScreen",
		itemName,
		&x,
		&y,
		&width,
		&height,
		color,
		&background))
	{
		return(0);
	}

	cgi_R_SetColor( color );

	iconSize = 60;
	pad = 12;

	// calculate placement of weapon icons
	holdX = x + (width - ((iconSize*rowIconCnt) + (pad * (rowIconCnt-1))))/2;

	for (i=startIndex;i<MAXLOADWEAPONS;i++)
	{
		if ( !(weapons[i]))	// Does he have this weapon?
		{
			continue;
		}

		if (weaponData[i].weaponIcon[0])
		{
			weaponInfo_t	*weaponInfo;
			CG_RegisterWeapon( i );
			weaponInfo = &cg_weapons[i];
			endIndex = i;

	// NOTE : during loading screen always show the have ammo icon
	//		if (!CG_WeaponCheck(i))
	//		{
	//			CG_DrawPic( holdX, y+yOffset, iconSize, iconSize, weaponInfo->weaponIconNoAmmo );
	//		}
	//		else
			{
				CG_DrawPic( holdX, y+yOffset, iconSize, iconSize, weaponInfo->weaponIcon );
			}

			printedIconCnt++;
			if (printedIconCnt==MAXLOADICONSPERROW)
			{
				break;
			}

			holdX += (iconSize+pad);
		}
	}

	return (endIndex);
}

// Print weapons the player is carrying
// Two rows print if there are too many
static void CG_DrawLoadWeapons( char *weapons )
{
	int		i,endIndex=0;
	int		iconCnt,rowIconCnt;

	// count the number of weapons owned
	iconCnt = 0;
	for ( i = 1 ; i < MAXLOADWEAPONS ; i++ )
	{
		if ( weapons[i] )
		{
			iconCnt++;
		}
	}

	if (!iconCnt)	// If no weapons, don't display
	{
		return;
	}

	//TODO: allow for more rows of icons??
	// Single line of icons
	if (iconCnt<=MAXLOADICONSPERROW)
	{
		CG_DrawLoadWeaponsPrintRow("weaponicons_singlerow", weapons, iconCnt,0);
	}
	// Two lines of icons
	else
	{
		// Print top row
		endIndex = CG_DrawLoadWeaponsPrintRow("weaponicons_row1", weapons, MAXLOADICONSPERROW,0);

		// Print second row
		rowIconCnt = iconCnt - MAXLOADICONSPERROW;
		CG_DrawLoadWeaponsPrintRow("weaponicons_row2", weapons, rowIconCnt,endIndex+1);
	}

	cgi_R_SetColor( NULL );
}


static int CG_DrawLoadForcePrintRow( const char *itemName, int forceBits,int rowIconCnt, int startIndex)
{
	int		i,endIndex=0, printedIconCnt=0;
	int		holdX,x,y;
	int		yOffset = 0;
	int		width,height;
	vec4_t	color;
	qhandle_t	background;

	if (!cgi_UI_GetMenuItemInfo(
		"loadScreen",
		itemName,
		&x,
		&y,
		&width,
		&height,
		color,
		&background))
	{
		return(0);
	}

	cgi_R_SetColor( color );

	// calculate placement of weapon icons
	holdX = x + (width - ((MAXLOAD_FORCEICONSIZE*rowIconCnt) + (MAXLOAD_FORCEICONPAD * (rowIconCnt-1))))/2;

	for (i=startIndex;i<MAX_SHOWPOWERS;i++)
	{
		if (!CG_ForcePower_Valid(forceBits,i))	// Does he have this power?
		{
			continue;
		}

		if (force_icons[showPowers[i]])
		{
			endIndex = i;

			CG_DrawPic( holdX, y+yOffset, MAXLOAD_FORCEICONSIZE, MAXLOAD_FORCEICONSIZE, force_icons[showPowers[i]] );

			printedIconCnt++;
			if (printedIconCnt==MAXLOADICONSPERROW)
			{
				break;
			}

			holdX += (MAXLOAD_FORCEICONSIZE+MAXLOAD_FORCEICONPAD);
		}
	}

	return (endIndex);
}

int			loadForcePowerLevel[NUM_FORCE_POWERS];

/*
===============
ForcePowerDataPad_Valid
===============
*/
qboolean CG_ForcePower_Valid(int forceKnownBits, int index)
{
	if ((forceKnownBits & (1 << showPowers[index])) &&
		loadForcePowerLevel[showPowers[index]])	// Does he have the force power?
	{
		return qtrue;
	}

	return qfalse;
}

// Print force powers the player is using
// Two rows print if there are too many
static void CG_DrawLoadForcePowers( int forceBits )
{
	int		i,endIndex=0;
	int		iconCnt=0,rowIconCnt;

	// Count the number of force powers known
	for (i=0; i<MAX_SHOWPOWERS; ++i)
	{
		if (CG_ForcePower_Valid(forceBits, i))
		{
			iconCnt++;
		}
	}

	if (!iconCnt)	// If no force powers, don't display
	{
		return;
	}

	// Single line of icons
	if (iconCnt<=MAXLOADICONSPERROW)
	{
		CG_DrawLoadForcePrintRow("forceicons_singlerow", forceBits, iconCnt,0);
	}
	// Two lines of icons
	else
	{
		// Print top row
		endIndex = CG_DrawLoadForcePrintRow("forceicons_row1", forceBits, MAXLOADICONSPERROW,0);

		// Print second row
		rowIconCnt = iconCnt - MAXLOADICONSPERROW;
		CG_DrawLoadForcePrintRow("forceicons_row2", forceBits, rowIconCnt,endIndex+1);
	}

	cgi_R_SetColor( NULL );
}

// Get the player weapons and force power info 
static void CG_GetLoadScreenInfo(char *weapons,int *forceBits)
{
	char	s[MAX_STRING_CHARS];
	int		iDummy,i;
	float	fDummy;
	const char	*var;

	gi.Cvar_VariableStringBuffer( sCVARNAME_PLAYERSAVE, s, sizeof(s) );

	// Get player weapons and force powers known
	if (s[0])
	{
	//				|general info				  |-force powers 
		sscanf( s, "%i %i %i %i %i %i %f %f %f %i %i",
				&iDummy,	//	&client->ps.stats[STAT_HEALTH],
				&iDummy,	//	&client->ps.stats[STAT_ARMOR],
				&iDummy,	//	&client->ps.stats[STAT_ITEMS],
				&iDummy,	//	&client->ps.weapon,
				&iDummy,	//	&client->ps.weaponstate,
				&iDummy,	//	&client->ps.batteryCharge,
				&fDummy,	//	&client->ps.viewangles[0],
				&fDummy,	//	&client->ps.viewangles[1],
				&fDummy,	//	&client->ps.viewangles[2],
							//force power data
				&*forceBits,	//	&client->ps.forcePowersKnown,
				&iDummy		//	&client->ps.forcePower,

				);
	}
	
	gi.Cvar_VariableStringBuffer( "playerweaps", s, sizeof(s) );
	i=0;
	if (s[0])
	{
		var = strtok( s, " " );
		while( var != NULL )
		{
			/* While there are tokens in "s" */
			weapons[i++] = atoi(var);
			/* Get next token: */
			var = strtok( NULL, " " );
		}
		assert (i==WP_NUM_WEAPONS);
	}
	else
	{
		weapons[0] = -1;
	}


	// the new JK2 stuff - force powers, etc...
	//
	gi.Cvar_VariableStringBuffer( "playerfplvl", s, sizeof(s) );
	i=0;
	var = strtok( s, " " );
	while( var != NULL )
	{
		/* While there are tokens in "s" */
		loadForcePowerLevel[i++] = atoi(var);
		/* Get next token: */
		var = strtok( NULL, " " );
	}
}

/*
====================
CG_DrawLoadingScreen

Load screen displays the map pic, the mission briefing and weapons/force powers
====================
*/


int CG_MapIsAllowedToDisplayTips(void)
{
	// draw the dialog background
	const char* info = CG_ConfigString(CS_SERVERINFO);
	const char* mapname = Info_ValueForKey(info, "mapname");

	if ((!strcmp(mapname, "md_trade_federation")
		|| !strcmp(mapname, "md_dof")
		|| !strcmp(mapname, "md_Quigonfuneral")
		|| !strcmp(mapname, "md_kamino")
		|| !strcmp(mapname, "md_ka")
		|| !strcmp(mapname, "md_tc")
		|| !strcmp(mapname, "md_tcr")
		|| !strcmp(mapname, "md_roc")
		|| !strcmp(mapname, "md_utapaupaucity")
		|| !strcmp(mapname, "md_egg")
		|| !strcmp(mapname, "md_coruscantcity")
		|| !strcmp(mapname, "md_mof")
		|| !strcmp(mapname, "md_po")
		|| !strcmp(mapname, "md_jt_outside")
		|| !strcmp(mapname, "md_jeditemple")
		|| !strcmp(mapname, "md_vader_immortalization")
		|| !strcmp(mapname, "md_mus")
		|| !strcmp(mapname, "md_vr")
		|| !strcmp(mapname, "md_vad_castle")
		|| !strcmp(mapname, "md_profundity")
		|| !strcmp(mapname, "md_luke")
		|| !strcmp(mapname, "md_dd")
		|| !strcmp(mapname, "md_yavin4")
		|| !strcmp(mapname, "md_duel_yavin")
		|| !strcmp(mapname, "md_hoth")
		|| !strcmp(mapname, "md_cloudcity")
		|| !strcmp(mapname, "md_cc")
		|| !strcmp(mapname, "md_duel_bespin")
		|| !strcmp(mapname, "md_sarlacc")
		|| !strcmp(mapname, "md_jabba")
		|| !strcmp(mapname, "md_jabba_night")
		|| !strcmp(mapname, "md_emp")
		|| !strcmp(mapname, "md_star_base")
		|| !strcmp(mapname, "md_star_basef")
		|| !strcmp(mapname, "md_snoke")
		|| !strcmp(mapname, "md_crait")
		|| !strcmp(mapname, "md_mandalore")
		|| !strcmp(mapname, "md_mandalore_night")
		|| !strcmp(mapname, "md_tfu_tr")
		|| !strcmp(mapname, "md_deathstar_tfu")
		|| !strcmp(mapname, "md_votj")
		|| !strcmp(mapname, "md_kor")
		|| !strcmp(mapname, "md2_bd_ch")
		|| !strcmp(mapname, "md2_jango_ch")
		|| !strcmp(mapname, "md2_star_ch")
		|| !strcmp(mapname, "md2_jinn_ch")
		|| !strcmp(mapname, "md2_gri_ch")
		|| !strcmp(mapname, "md2_win_ch")
		|| !strcmp(mapname, "md2_dooku_ch")
		|| !strcmp(mapname, "md2_vos_ch")
		|| !strcmp(mapname, "md2_kyle_ch")
		|| !strcmp(mapname, "md2_vader_ch")
		|| !strcmp(mapname, "md2_ben_ch")
		|| !strcmp(mapname, "md2_yoda_ch")
		|| !strcmp(mapname, "md2_pal_ch")
		|| !strcmp(mapname, "md2_jedi_ch")
		|| !strcmp(mapname, "md2_sith_ch")
		|| !strcmp(mapname, "md_rancor_ch")
		|| !strcmp(mapname, "md_yun_ch")
		|| !strcmp(mapname, "md_jerec_ch")
		|| !strcmp(mapname, "md_tavion_ch")
		|| !strcmp(mapname, "md_kylo_ch")
		|| !strcmp(mapname, "md_snoke_ch")
		|| !strcmp(mapname, "md_mus_both")
		|| !strcmp(mapname, "md_cor_lp_tfu")
		|| !strcmp(mapname, "md_wote_sith")))//special case for these maps!
	{
		return qtrue;
	}
	else
	{
		return qfalse;
	}
}

static void CG_DrawLoadingScreen( qhandle_t	levelshot ,const char *mapName)
{
	int xPos,yPos,width,height;
	vec4_t	color;
	qhandle_t	background;
	int forcepowers=0;
	char weapons[WP_NUM_WEAPONS];

	// Get mission briefing for load screen
	if (cgi_SP_GetStringTextString( va("BRIEFINGS_%s",mapName), NULL, 0 ) == 0)
	{
		cgi_Cvar_Set("ui_missionbriefing", "@BRIEFINGS_NONE");
	}
	else
	{
		cgi_Cvar_Set( "ui_missionbriefing", va("@BRIEFINGS_%s",mapName) );
	}

	// Print background
	if (cgi_UI_GetMenuItemInfo(
		"loadScreen",
		"background",
		&xPos,
		&yPos,
		&width,
		&height,
		color,
		&background))
	{
		cgi_R_SetColor( color );
		CG_DrawPic( xPos, yPos, width, height, background );
	}

	// Print level pic
	if (cgi_UI_GetMenuItemInfo(
		"loadScreen",
		"mappic",
		&xPos,
		&yPos,
		&width,
		&height,
		color,
		&background))
	{
		cgi_R_SetColor(color);
		CG_DrawPic(xPos, yPos, width, height, levelshot);
	}

	// Get player weapons and force power info
	CG_GetLoadScreenInfo(weapons,&forcepowers);

	// Print weapon icons
	if (weapons[0] > -1)
	{
		CG_DrawLoadWeapons(weapons);
	}

	// Print force power icons
	if (forcepowers)
	{
		CG_DrawLoadForcePowers(forcepowers);
	}
}

/*
====================
CG_DrawInformation

Draw all the status / pacifier stuff during level loading
====================
*/

int SCREENTIP_NEXT_UPDATE_TIME = 0;

static void LoadTips(void)
{
	int index;
	int	time;

	index = Q_irand(1, 15);
	time = cgi_Milliseconds();


	if (SCREENTIP_NEXT_UPDATE_TIME < time)
	{
		cgi_Cvar_Set("ui_tipsbriefing", va("@LOADTIPS_TIP%d", index));
		SCREENTIP_NEXT_UPDATE_TIME = time + 2500;
	}

	return;
}

int SCREENSHOT_TOTAL = -1;
int SCREENSHOT_CHOICE = 0;
int SCREENSHOT_NEXT_UPDATE_TIME = 0;
char SCREENSHOT_CURRENT[64] = { 0 };

char* cg_GetCurrentLevelshot1(const char* s)
{
	int		time;
	qhandle_t	levelshot1;

	levelshot1 = cgi_R_RegisterShaderNoMip(va("levelshots/%s", s));
	time = cgi_Milliseconds();

	if (levelshot1 && SCREENSHOT_NEXT_UPDATE_TIME == 0)
	{
		SCREENSHOT_NEXT_UPDATE_TIME = time + 10000;
		memset(SCREENSHOT_CURRENT, 0, sizeof(SCREENSHOT_CURRENT));
		strcpy(SCREENSHOT_CURRENT, va("levelshots/%s", s));
		return SCREENSHOT_CURRENT;
	}

	if (SCREENSHOT_NEXT_UPDATE_TIME < time || SCREENSHOT_NEXT_UPDATE_TIME == 0)
	{
		if (SCREENSHOT_TOTAL < 0)
		{// Count and register them...
			SCREENSHOT_TOTAL = 0;

			while (1)
			{
				char screenShot[128] = { 0 };

				strcpy(screenShot, va("menu/art/unknownmap%i", SCREENSHOT_TOTAL));

				if (!cgi_R_RegisterShaderNoMip(screenShot))
				{
					break;
				}
				SCREENSHOT_TOTAL++;
			}
			SCREENSHOT_TOTAL--;
		}
		SCREENSHOT_NEXT_UPDATE_TIME = time + 10000;
		SCREENSHOT_CHOICE = Q_flrand(0, SCREENSHOT_TOTAL);
		memset(SCREENSHOT_CURRENT, 0, sizeof(SCREENSHOT_CURRENT));
		strcpy(SCREENSHOT_CURRENT, va("menu/art/unknownmap%i", SCREENSHOT_CHOICE));
	}
	return SCREENSHOT_CURRENT;
}

void CG_DrawInformation(void)
{
	int		y;
	// draw the dialog background
	const char* info = CG_ConfigString(CS_SERVERINFO);
	const char* s = Info_ValueForKey(info, "mapname");

	qhandle_t	levelshot;
	qhandle_t	Loadshot;

	extern SavedGameJustLoaded_e g_eSavedGameJustLoaded;

	levelshot = cgi_R_RegisterShaderNoMip(va("levelshots/%s", s));
	Loadshot = cgi_R_RegisterShaderNoMip("menu/art/loadshot");

	if (!levelshot)
	{
		levelshot = cgi_R_RegisterShaderNoMip(cg_GetCurrentLevelshot1(s));
	}
	if (!Loadshot)
	{
		Loadshot = cgi_R_RegisterShaderNoMip(cg_GetCurrentLevelshot1(s));
	}

	if (g_eSavedGameJustLoaded != eFULL
		&& (!strcmp(s, "yavin1")
			|| !strcmp(s, "demo")
			|| !strcmp(s, "jodemo")
			|| !strcmp(s, "01nar")
			|| !strcmp(s, "md2_bd_ch")
			|| !strcmp(s, "md_sn_jedi")
			|| !strcmp(s, "secbase")
			|| !strcmp(s, "level0")
			|| !strcmp(s, "kejim_post")))//special case for first map!
	{
		char	text[1024] = { 0 };

		CG_DrawPic(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, Loadshot);

		int w = cgi_R_Font_StrLenPixels(text, cgs.media.qhFontMedium, 1.0f);
		cgi_R_Font_DrawString((320) - (w / 2), 140, text, colorTable[CT_ICON_BLUE], cgs.media.qhFontMedium, -1, 1.0f);
	}
	else
	{
		CG_DrawLoadingScreen(levelshot, s);
		cgi_UI_Menu_Paint(cgi_UI_GetMenuByName("loadscreen"), qtrue);
		CG_LoadBar();
		LoadTips();
	}

	// draw info string information

	y = 20;
	// map-specific message (long map name)
	s = CG_ConfigString(CS_MESSAGE);

	if (s[0])
	{
		if (s[0] == '@')
		{
			char text[1024] = { 0 };
			cgi_SP_GetStringTextString(s + 1, text, sizeof(text));
			cgi_R_Font_DrawString(15, y, va("\"%s\"", text), colorTable[CT_WHITE], cgs.media.qhFontMedium, -1, 1.0f);
		}
		else
		{
			cgi_R_Font_DrawString(15, y, va("\"%s\"", s), colorTable[CT_WHITE], cgs.media.qhFontMedium, -1, 1.0f);
		}
		y += 20;
	}
}
