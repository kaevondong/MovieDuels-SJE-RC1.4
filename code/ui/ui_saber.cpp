/*
===========================================================================
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

/*
=======================================================================

USER INTERFACE SABER LOADING & DISPLAY CODE

=======================================================================
*/

// leave this at the top of all UI_xxxx files for PCH reasons...
//
#include "../server/exe_headers.h"
#include "ui_local.h"
#include "ui_shared.h"
#include "../ghoul2/G2.h"

#define MAX_SABER_DATA_SIZE 0x80000
char	SaberParms[MAX_SABER_DATA_SIZE];
qboolean	ui_saber_parms_parsed = qfalse;

extern vmCvar_t	ui_rgb_saber_red;
extern vmCvar_t	ui_rgb_saber_green;
extern vmCvar_t	ui_rgb_saber_blue;
extern vmCvar_t	ui_rgb_saber2_red;
extern vmCvar_t	ui_rgb_saber2_green;
extern vmCvar_t	ui_rgb_saber2_blue;

extern vmCvar_t ui_SerenityJediEngineMode;

extern vmCvar_t	ui_SFXSabers;
extern vmCvar_t	ui_SFXSabersGlowSize;
extern vmCvar_t	ui_SFXSabersCoreSize;
//
extern vmCvar_t	ui_SFXSabersGlowSizeEP1;
extern vmCvar_t	ui_SFXSabersCoreSizeEP1;
extern vmCvar_t	ui_SFXSabersGlowSizeEP2;
extern vmCvar_t	ui_SFXSabersCoreSizeEP2;
extern vmCvar_t	ui_SFXSabersGlowSizeEP3;
extern vmCvar_t	ui_SFXSabersCoreSizeEP3;
extern vmCvar_t	ui_SFXSabersGlowSizeSFX;
extern vmCvar_t	ui_SFXSabersCoreSizeSFX;
extern vmCvar_t	ui_SFXSabersGlowSizeOT;
extern vmCvar_t	ui_SFXSabersCoreSizeOT;
extern vmCvar_t	ui_SFXSabersGlowSizeROTJ;
extern vmCvar_t	ui_SFXSabersCoreSizeROTJ;
extern vmCvar_t	ui_SFXSabersGlowSizeTFA;
extern vmCvar_t	ui_SFXSabersCoreSizeTFA;
extern vmCvar_t	ui_SFXSabersGlowSizeUSB;
extern vmCvar_t	ui_SFXSabersCoreSizeUSB;

static qhandle_t redSaberGlowShader;
static qhandle_t redSaberCoreShader;
static qhandle_t orangeSaberGlowShader;
static qhandle_t orangeSaberCoreShader;
static qhandle_t yellowSaberGlowShader;
static qhandle_t yellowSaberCoreShader;
static qhandle_t greenSaberGlowShader;
static qhandle_t greenSaberCoreShader;
static qhandle_t blueSaberGlowShader;
static qhandle_t blueSaberCoreShader;
static qhandle_t purpleSaberGlowShader;
static qhandle_t purpleSaberCoreShader;
static qhandle_t unstableRedSaberGlowShader;
static qhandle_t unstableRedSaberCoreShader;

static qhandle_t blackSaberGlowShader;
static qhandle_t blackSaberCoreShader;
static qhandle_t rgbSaberGlowShader;
static qhandle_t rgbSaberCoreShader;
static qhandle_t limeSaberGlowShader;
static qhandle_t limeSaberCoreShader;

//Episode I Sabers
static qhandle_t ep1SaberCoreShader;
static qhandle_t ep1blackSaberCoreShader;
static qhandle_t redEp1GlowShader;
static qhandle_t orangeEp1GlowShader;
static qhandle_t yellowEp1GlowShader;
static qhandle_t greenEp1GlowShader;
static qhandle_t blueEp1GlowShader;
static qhandle_t purpleEp1GlowShader;

//Episode II Sabers
static qhandle_t ep2SaberCoreShader;
static qhandle_t ep2blackSaberCoreShader;
static qhandle_t whiteIgniteFlare;
static qhandle_t blackIgniteFlare;
static qhandle_t redEp2GlowShader;
static qhandle_t orangeEp2GlowShader;
static qhandle_t yellowEp2GlowShader;
static qhandle_t greenEp2GlowShader;
static qhandle_t blueEp2GlowShader;
static qhandle_t purpleEp2GlowShader;

//Episode III Sabers
static qhandle_t ep3SaberCoreShader;
static qhandle_t ep3redSaberCoreShader;
static qhandle_t ep3orangeSaberCoreShader;
static qhandle_t ep3yellowSaberCoreShader;
static qhandle_t ep3greenSaberCoreShader;
static qhandle_t ep3blueSaberCoreShader;
static qhandle_t ep3purpleSaberCoreShader;
static qhandle_t ep3blackSaberCoreShader;
static qhandle_t redEp3GlowShader;
static qhandle_t orangeEp3GlowShader;
static qhandle_t yellowEp3GlowShader;
static qhandle_t greenEp3GlowShader;
static qhandle_t blueEp3GlowShader;
static qhandle_t purpleEp3GlowShader;

//SFX Sabers
static qhandle_t sfxSaberBladeShader;
static qhandle_t sfxredSaberBladeShader;
static qhandle_t sfxorangeSaberBladeShader;
static qhandle_t sfxyellowSaberBladeShader;
static qhandle_t sfxgreenSaberBladeShader;
static qhandle_t sfxblueSaberBladeShader;
static qhandle_t sfxpurpleSaberBladeShader;
static qhandle_t sfxblackSaberBladeShader;

//Original Trilogy Sabers
static qhandle_t otSaberCoreShader;
static qhandle_t redOTGlowShader;
static qhandle_t orangeOTGlowShader;
static qhandle_t yellowOTGlowShader;
static qhandle_t greenOTGlowShader;
static qhandle_t blueOTGlowShader;
static qhandle_t purpleOTGlowShader;
static qhandle_t rgbTFASaberCoreShader;

//Episode VI Sabers
static qhandle_t ep6SaberCoreShader;
static qhandle_t ep6redSaberCoreShader;
static qhandle_t ep6orangeSaberCoreShader;
static qhandle_t ep6yellowSaberCoreShader;
static qhandle_t ep6greenSaberCoreShader;
static qhandle_t ep6blueSaberCoreShader;
static qhandle_t ep6purpleSaberCoreShader;
static qhandle_t ep6blackSaberCoreShader;
static qhandle_t redEp6GlowShader;
static qhandle_t orangeEp6GlowShader;
static qhandle_t yellowEp6GlowShader;
static qhandle_t greenEp6GlowShader;
static qhandle_t blueEp6GlowShader;
static qhandle_t purpleEp6GlowShader;

//Episode VII Sabers
static qhandle_t ep7SaberCoreShader;
static qhandle_t ep7redSaberCoreShader;
static qhandle_t ep7orangeSaberCoreShader;
static qhandle_t ep7yellowSaberCoreShader;
static qhandle_t ep7greenSaberCoreShader;
static qhandle_t ep7blueSaberCoreShader;
static qhandle_t ep7purpleSaberCoreShader;
static qhandle_t ep7blackSaberCoreShader;
static qhandle_t redEp7GlowShader;
static qhandle_t orangeEp7GlowShader;
static qhandle_t yellowEp7GlowShader;
static qhandle_t greenEp7GlowShader;
static qhandle_t blueEp7GlowShader;
static qhandle_t purpleEp7GlowShader;

void UI_CacheSaberGlowGraphics(void)
{//FIXME: these get fucked by vid_restarts
	redSaberGlowShader = re.RegisterShader("gfx/effects/sabers/red_glow");
	redSaberCoreShader = re.RegisterShader("gfx/effects/sabers/red_line");
	orangeSaberGlowShader = re.RegisterShader("gfx/effects/sabers/orange_glow");
	orangeSaberCoreShader = re.RegisterShader("gfx/effects/sabers/orange_line");
	yellowSaberGlowShader = re.RegisterShader("gfx/effects/sabers/yellow_glow");
	yellowSaberCoreShader = re.RegisterShader("gfx/effects/sabers/yellow_line");
	greenSaberGlowShader = re.RegisterShader("gfx/effects/sabers/green_glow");
	greenSaberCoreShader = re.RegisterShader("gfx/effects/sabers/green_line");
	blueSaberGlowShader = re.RegisterShader("gfx/effects/sabers/blue_glow");
	blueSaberCoreShader = re.RegisterShader("gfx/effects/sabers/blue_line");
	purpleSaberGlowShader = re.RegisterShader("gfx/effects/sabers/purple_glow");
	purpleSaberCoreShader = re.RegisterShader("gfx/effects/sabers/purple_line");
	unstableRedSaberCoreShader = re.RegisterShader("gfx/effects/sabers_tfa/red_unstable_line");

	blackSaberGlowShader = re.RegisterShader("gfx/effects/sabers/black_glow");
	blackSaberCoreShader = re.RegisterShader("gfx/effects/sabers/black_line");
	rgbSaberGlowShader = re.RegisterShader("gfx/effects/sabers/rgb_glow");
	rgbSaberCoreShader = re.RegisterShader("gfx/effects/sabers/rgb_line");
	limeSaberGlowShader = re.RegisterShader("gfx/effects/sabers/lime_glow");
	limeSaberCoreShader = re.RegisterShader("gfx/effects/sabers/lime_line");


	//Episode I Sabers
	ep1SaberCoreShader = re.RegisterShader("gfx/effects/sabers_ep1/rgb_line");
	ep1blackSaberCoreShader = re.RegisterShader("gfx/effects/sabers_ep1/black_line");
	redEp1GlowShader = re.RegisterShader("gfx/effects/sabers_ep1/red_glow");
	orangeEp1GlowShader = re.RegisterShader("gfx/effects/sabers_ep1/orange_glow");
	yellowEp1GlowShader = re.RegisterShader("gfx/effects/sabers_ep1/yellow_glow");
	greenEp1GlowShader = re.RegisterShader("gfx/effects/sabers_ep1/green_glow");
	blueEp1GlowShader = re.RegisterShader("gfx/effects/sabers_ep1/blue_glow");
	purpleEp1GlowShader = re.RegisterShader("gfx/effects/sabers_ep1/purple_glow");
	//Episode II Sabers
	ep2SaberCoreShader = re.RegisterShader("gfx/effects/sabers_ep2/rgb_line");
	ep2blackSaberCoreShader = re.RegisterShader("gfx/effects/sabers_ep2/black_line");
	redEp2GlowShader = re.RegisterShader("gfx/effects/sabers_ep2/red_glow");
	orangeEp2GlowShader = re.RegisterShader("gfx/effects/sabers_ep2/orange_glow");
	yellowEp2GlowShader = re.RegisterShader("gfx/effects/sabers_ep2/yellow_glow");
	greenEp2GlowShader = re.RegisterShader("gfx/effects/sabers_ep2/green_glow");
	blueEp2GlowShader = re.RegisterShader("gfx/effects/sabers_ep2/blue_glow");
	purpleEp2GlowShader = re.RegisterShader("gfx/effects/sabers_ep2/purple_glow");
	//Episode III Sabers
	ep3SaberCoreShader = re.RegisterShader("gfx/effects/sabers_ep3/rgb_line");
	ep3redSaberCoreShader = re.RegisterShader("gfx/effects/sabers_ep3/red_line");
	ep3orangeSaberCoreShader = re.RegisterShader("gfx/effects/sabers_ep3/orange_line");
	ep3yellowSaberCoreShader = re.RegisterShader("gfx/effects/sabers_ep3/yellow_line");
	ep3greenSaberCoreShader = re.RegisterShader("gfx/effects/sabers_ep3/green_line");
	ep3blueSaberCoreShader = re.RegisterShader("gfx/effects/sabers_ep3/blue_line");
	ep3purpleSaberCoreShader = re.RegisterShader("gfx/effects/sabers_ep3/purple_line");
	ep3blackSaberCoreShader = re.RegisterShader("gfx/effects/sabers_ep3/black_line");
	redEp3GlowShader = re.RegisterShader("gfx/effects/sabers_ep3/red_glow");
	orangeEp3GlowShader = re.RegisterShader("gfx/effects/sabers_ep3/orange_glow");
	yellowEp3GlowShader = re.RegisterShader("gfx/effects/sabers_ep3/yellow_glow");
	greenEp3GlowShader = re.RegisterShader("gfx/effects/sabers_ep3/green_glow");
	blueEp3GlowShader = re.RegisterShader("gfx/effects/sabers_ep3/blue_glow");
	purpleEp3GlowShader = re.RegisterShader("gfx/effects/sabers_ep3/purple_glow");
	//SFX Sabers
	sfxSaberBladeShader = re.RegisterShader("SFX_Sabers/saber_blade");
	sfxblackSaberBladeShader = re.RegisterShader("SFX_Sabers/saber_blade_black");
	//Original Trilogy Sabers
	otSaberCoreShader = re.RegisterShader("gfx/effects/sabers_ot/ot_saberCore");
	redOTGlowShader = re.RegisterShader("gfx/effects/sabers_ot/ot_redGlow");
	orangeOTGlowShader = re.RegisterShader("gfx/effects/sabers_ot/ot_orangeGlow");
	yellowOTGlowShader = re.RegisterShader("gfx/effects/sabers_ot/ot_yellowGlow");
	greenOTGlowShader = re.RegisterShader("gfx/effects/sabers_ot/ot_greenGlow");
	blueOTGlowShader = re.RegisterShader("gfx/effects/sabers_ot/ot_blueGlow");
	purpleOTGlowShader = re.RegisterShader("gfx/effects/sabers_ot/ot_purpleGlow");
	//Episode VI Sabers
	ep6SaberCoreShader = re.RegisterShader("gfx/effects/sabers_rotj/rgb_line");
	ep6blackSaberCoreShader = re.RegisterShader("gfx/effects/sabers_rotj/black_line");
	redEp6GlowShader = re.RegisterShader("gfx/effects/sabers_rotj/red_glow");
	orangeEp6GlowShader = re.RegisterShader("gfx/effects/sabers_rotj/orange_glow");
	yellowEp6GlowShader = re.RegisterShader("gfx/effects/sabers_rotj/yellow_glow");
	greenEp6GlowShader = re.RegisterShader("gfx/effects/sabers_rotj/green_glow");
	blueEp6GlowShader = re.RegisterShader("gfx/effects/sabers_rotj/blue_glow");
	purpleEp6GlowShader = re.RegisterShader("gfx/effects/sabers_rotj/purple_glow");
	//Episode VII Sabers
	ep7SaberCoreShader = re.RegisterShader("gfx/effects/sabers_tfa/rgb_line");
	ep7redSaberCoreShader = re.RegisterShader("gfx/effects/sabers_tfa/red_line");
	ep7orangeSaberCoreShader = re.RegisterShader("gfx/effects/sabers_tfa/orange_line");
	ep7yellowSaberCoreShader = re.RegisterShader("gfx/effects/sabers_tfa/yellow_line");
	ep7greenSaberCoreShader = re.RegisterShader("gfx/effects/sabers_tfa/green_line");
	ep7blueSaberCoreShader = re.RegisterShader("gfx/effects/sabers_tfa/blue_line");
	ep7purpleSaberCoreShader = re.RegisterShader("gfx/effects/sabers_tfa/purple_line");
	ep7blackSaberCoreShader = re.RegisterShader("gfx/effects/sabers_tfa/black_line");
	rgbTFASaberCoreShader = re.RegisterShader("gfx/effects/TFASabers/blade_TFA");
	redEp7GlowShader = re.RegisterShader("gfx/effects/sabers_tfa/red_glow");
	orangeEp7GlowShader = re.RegisterShader("gfx/effects/sabers_tfa/orange_glow");
	yellowEp7GlowShader = re.RegisterShader("gfx/effects/sabers_tfa/yellow_glow");
	greenEp7GlowShader = re.RegisterShader("gfx/effects/sabers_tfa/green_glow");
	blueEp7GlowShader = re.RegisterShader("gfx/effects/sabers_tfa/blue_glow");
	purpleEp7GlowShader = re.RegisterShader("gfx/effects/sabers_tfa/purple_glow");
}

qboolean UI_ParseLiteral(const char** data, const char* string)
{
	const char* token;

	token = COM_ParseExt(data, qtrue);
	if (token[0] == 0)
	{
		ui.Printf("unexpected EOF\n");
		return qtrue;
	}

	if (Q_stricmp(token, string))
	{
		ui.Printf("required string '%s' missing\n", string);
		return qtrue;
	}

	return qfalse;
}

qboolean UI_SaberParseParm(const char* saberName, const char* parmname, char* saberData)
{
	const char* token;
	const char* value;
	const char* p;

	if (!saberName || !saberName[0])
	{
		return qfalse;
	}

	//try to parse it out
	p = SaberParms;
	COM_BeginParseSession();

	// look for the right saber
	while (p)
	{
		token = COM_ParseExt(&p, qtrue);
		if (token[0] == 0)
		{
			COM_EndParseSession();
			return qfalse;
		}

		if (!Q_stricmp(token, saberName))
		{
			break;
		}

		SkipBracedSection(&p);
	}
	if (!p)
	{
		COM_EndParseSession();
		return qfalse;
	}

	if (UI_ParseLiteral(&p, "{"))
	{
		COM_EndParseSession();
		return qfalse;
	}

	// parse the saber info block
	while (1)
	{
		token = COM_ParseExt(&p, qtrue);
		if (!token[0])
		{
			ui.Printf(S_COLOR_RED"ERROR: unexpected EOF while parsing '%s'\n", saberName);
			COM_EndParseSession();
			return qfalse;
		}

		if (!Q_stricmp(token, "}"))
		{
			break;
		}

		if (!Q_stricmp(token, parmname))
		{
			if (COM_ParseString(&p, &value))
			{
				continue;
			}
			strcpy(saberData, value);
			COM_EndParseSession();
			return qtrue;
		}

		SkipRestOfLine(&p);
		continue;
	}

	COM_EndParseSession();
	return qfalse;
}

qboolean UI_SaberProperNameForSaber(const char* saberName, char* saberProperName)
{
	return UI_SaberParseParm(saberName, "name", saberProperName);
}

qboolean UI_SaberModelForSaber(const char* saberName, char* saberModel)
{
	return UI_SaberParseParm(saberName, "saberModel", saberModel);
}

qboolean UI_SaberSkinForSaber(const char* saberName, char* saberSkin)
{
	return UI_SaberParseParm(saberName, "customSkin", saberSkin);
}

qboolean UI_SaberTypeForSaber(const char* saberName, char* saberType)
{
	return UI_SaberParseParm(saberName, "saberType", saberType);
}

int UI_SaberNumBladesForSaber(const char* saberName)
{
	char	numBladesString[8] = { 0 };
	UI_SaberParseParm(saberName, "numBlades", numBladesString);
	int numBlades = atoi(numBladesString);
	if (numBlades < 1)
	{
		numBlades = 1;
	}
	else if (numBlades > 8)
	{
		numBlades = 8;
	}
	return numBlades;
}

qboolean UI_SaberShouldDrawBlade(const char* saberName, int bladeNum)
{
	int bladeStyle2Start = 0, noBlade = 0;
	char	bladeStyle2StartString[8] = { 0 };
	char	noBladeString[8] = { 0 };
	UI_SaberParseParm(saberName, "bladeStyle2Start", bladeStyle2StartString);
	if (bladeStyle2StartString[0])
	{
		bladeStyle2Start = atoi(bladeStyle2StartString);
	}
	if (bladeStyle2Start
		&& bladeNum >= bladeStyle2Start)
	{//use second blade style
		UI_SaberParseParm(saberName, "noBlade2", noBladeString);
		if (noBladeString[0])
		{
			noBlade = atoi(noBladeString);
		}
	}
	else
	{//use first blade style
		UI_SaberParseParm(saberName, "noBlade", noBladeString);
		if (noBladeString[0])
		{
			noBlade = atoi(noBladeString);
		}
	}
	return ((qboolean)(noBlade == 0));
}

float UI_SaberBladeLengthForSaber(const char* saberName, int bladeNum)
{
	char	lengthString[8] = { 0 };
	float	length = 40.0f;
	UI_SaberParseParm(saberName, "saberLength", lengthString);
	if (lengthString[0])
	{
		length = atof(lengthString);
		if (length < 0.0f)
		{
			length = 0.0f;
		}
	}

	UI_SaberParseParm(saberName, va("saberLength%d", bladeNum + 1), lengthString);
	if (lengthString[0])
	{
		length = atof(lengthString);
		if (length < 0.0f)
		{
			length = 0.0f;
		}
	}

	return length;
}

float UI_SaberBladeRadiusForSaber(const char* saberName, int bladeNum)
{
	char	radiusString[8] = { 0 };
	float	radius = 3.0f;
	UI_SaberParseParm(saberName, "saberRadius", radiusString);
	if (radiusString[0])
	{
		radius = atof(radiusString);
		if (radius < 0.0f)
		{
			radius = 0.0f;
		}
	}

	UI_SaberParseParm(saberName, va("saberRadius%d", bladeNum + 1), radiusString);
	if (radiusString[0])
	{
		radius = atof(radiusString);
		if (radius < 0.0f)
		{
			radius = 0.0f;
		}
	}

	return radius;
}

void UI_SaberLoadParms(void)
{
	int			len, totallen, saberExtFNLen, fileCnt, i;
	char* buffer, * holdChar, * marker;
	char		saberExtensionListBuf[2048];			//	The list of file names read in

	//ui.Printf( "UI Parsing *.sab saber definitions\n" );

	ui_saber_parms_parsed = qtrue;
	UI_CacheSaberGlowGraphics();

	//set where to store the first one
	totallen = 0;
	marker = SaberParms;
	marker[0] = '\0';

	//now load in the sabers
	fileCnt = ui.FS_GetFileList("ext_data/sabers", ".sab", saberExtensionListBuf, sizeof(saberExtensionListBuf));

	holdChar = saberExtensionListBuf;
	for (i = 0; i < fileCnt; i++, holdChar += saberExtFNLen + 1)
	{
		saberExtFNLen = strlen(holdChar);

		len = ui.FS_ReadFile(va("ext_data/sabers/%s", holdChar), (void**)&buffer);

		if (len == -1)
		{
			ui.Printf("UI_SaberLoadParms: error reading %s\n", holdChar);
		}
		else
		{
			if (totallen && *(marker - 1) == '}')
			{//don't let it end on a } because that should be a stand-alone token
				strcat(marker, " ");
				totallen++;
				marker++;
			}
			len = COM_Compress(buffer);

			if (totallen + len >= MAX_SABER_DATA_SIZE) {
				Com_Error(ERR_FATAL, "UI_SaberLoadParms: ran out of space before reading %s\n(you must make the .npc files smaller)", holdChar);
			}
			strcat(marker, buffer);
			ui.FS_FreeFile(buffer);

			totallen += len;
			marker += len;
		}
	}
}

void RGB_LerpColor(vec3_t from, vec3_t to, float frac, vec3_t out)
{
	vec3_t diff;
	int i;

	VectorSubtract(to, from, diff);

	VectorCopy(from, out);

	for (i = 0; i < 3; i++)
		out[i] += diff[i] * frac;

}

int getint(char** buf)
{
	double temp;
	temp = strtod(*buf, buf);
	return (int)temp;
}

void ParseRGBSaber(char* str, vec3_t c)
{
	char* p = str;
	int i;

	for (i = 0; i < 3; i++)
	{
		c[i] = getint(&p);
		p++;
	}
}

vec3_t  ScriptedColors[10][2] = { 0 };
int		ScriptedTimes[10][2] = { 0 };
int		ScriptedNum[2] = { 0 };
int		ScriptedActualNum[2] = { 0 };
int		ScriptedStartTime[2] = { 0 };
int		ScriptedEndTime[2] = { 0 };

void UI_ParseScriptedSaber(char* script, int snum)
{
	int n = 0, l;
	char* p = script;

	l = strlen(p);
	p++;

	while (p[0] && p - script < l && n < 10)
	{
		ParseRGBSaber(p, ScriptedColors[n][snum]);
		while (p[0] != ':')
			p++;
		p++;

		ScriptedTimes[n][snum] = getint(&p);

		p++;
		n++;
	}
	ScriptedNum[snum] = n;
}


void RGB_AdjustSciptedSaberColor(vec3_t color, int n)
{
	int actual;
	float frac;
	int time = uiInfo.uiDC.realTime, i;


	if (!ScriptedStartTime[n])
	{
		ScriptedActualNum[n] = 0;
		ScriptedStartTime[n] = time;
		ScriptedEndTime[n] = time + ScriptedTimes[0][n];
	}
	else if (ScriptedEndTime[n] < time)
	{
		ScriptedActualNum[n] = (ScriptedActualNum[n] + 1) % ScriptedNum[n];
		actual = ScriptedActualNum[n];
		ScriptedStartTime[n] = time;
		ScriptedEndTime[n] = time + ScriptedTimes[actual][n];
	}

	actual = ScriptedActualNum[n];

	frac = (float)(time - ScriptedStartTime[n]) / (float)(ScriptedEndTime[n] - ScriptedStartTime[n]);


	if (actual + 1 != ScriptedNum[n])
		RGB_LerpColor(ScriptedColors[actual][n], ScriptedColors[actual + 1][n], frac, color);
	else
		RGB_LerpColor(ScriptedColors[actual][n], ScriptedColors[0][n], frac, color);

	for (i = 0; i < 3; i++)
		color[i] /= 255;

}

#define PIMP_MIN_INTESITY 120

void RGB_RandomRGB(vec3_t c)
{
	int i;
	for (i = 0; i < 3; i++)
		c[i] = 0;

	while (c[0] + c[1] + c[2] < PIMP_MIN_INTESITY)
		for (i = 0; i < 3; i++)
			c[i] = rand() % 255;
}

int PimpStartTime[2];
int PimpEndTime[2];
vec3_t PimpColorFrom[2];
vec3_t PimpColorTo[2];

void RGB_AdjustPimpSaberColor(vec3_t color, int n)
{
	int time, i;
	float frac;

	if (!PimpStartTime[n])
	{
		PimpStartTime[n] = uiInfo.uiDC.realTime;
		RGB_RandomRGB(PimpColorFrom[n]);
		RGB_RandomRGB(PimpColorTo[n]);
		time = 250 + rand() % 250;
		PimpEndTime[n] = uiInfo.uiDC.realTime + time;
	}
	else if (PimpEndTime[n] < uiInfo.uiDC.realTime)
	{
		VectorCopy(PimpColorTo[n], PimpColorFrom[n]);
		RGB_RandomRGB(PimpColorTo[n]);
		time = 250 + rand() % 250;
		PimpStartTime[n] = uiInfo.uiDC.realTime;
		PimpEndTime[n] = uiInfo.uiDC.realTime + time;
	}

	frac = (float)(uiInfo.uiDC.realTime - PimpStartTime[n]) / (float)(PimpEndTime[n] - PimpStartTime[n]);

	RGB_LerpColor(PimpColorFrom[n], PimpColorTo[n], frac, color);

	for (i = 0; i < 3; i++)
		color[i] /= 255;
}

void UI_DoSFXSaber(vec3_t blade_muz, vec3_t blade_dir, float lengthMax, float radius, saber_colors_t color, saber_crystals_t crystals, int whichSaber)
{
	vec3_t	mid;
	float	radiusmult, effectradius, coreradius;
	float	blade_len;
	float	effectalpha = 0.8f;
	float	AngleScale = 1.0f;

	qhandle_t	blade = 0, glow = 0;
	refEntity_t saber;

	blade_len = lengthMax;

	if (blade_len < 0.5f)
	{
		return;
	}

	switch (color)
	{
	case SABER_RED:
		glow = redSaberGlowShader;
		blade = sfxSaberBladeShader;
		break;
	case SABER_ORANGE:
		glow = orangeSaberGlowShader;
		blade = sfxSaberBladeShader;
		break;
	case SABER_YELLOW:
		glow = yellowSaberGlowShader;
		blade = sfxSaberBladeShader;
		break;
	case SABER_GREEN:
		glow = greenSaberGlowShader;
		blade = sfxSaberBladeShader;
		break;
	case SABER_BLUE:
		glow = blueSaberGlowShader;
		blade = sfxSaberBladeShader;
		break;
	case SABER_PURPLE:
		glow = purpleSaberGlowShader;
		blade = sfxSaberBladeShader;
		break;
	case SABER_RGB:
		glow = rgbSaberGlowShader;
		blade = sfxSaberBladeShader;
		break;
	case SABER_WHITE:
		glow = rgbSaberGlowShader;
		blade = sfxSaberBladeShader;
		break;
	case SABER_BLACK:
		glow = blackSaberGlowShader;
		blade = sfxblackSaberBladeShader;
		break;
	case SABER_UNSTABLE_RED:
		glow = redSaberGlowShader;
		blade = unstableRedSaberCoreShader;
		break;
	default:
		glow = rgbSaberGlowShader;
		blade = sfxSaberBladeShader;
		break;
	}

	VectorMA(blade_muz, blade_len * 0.5f, blade_dir, mid);

	memset(&saber, 0, sizeof(refEntity_t));

	if (blade_len < lengthMax)
	{
		radiusmult = 0.5 + ((blade_len / lengthMax) / 2);
	}
	else
	{
		radiusmult = 1.0;
	}

	effectradius = ((radius * 1.6) + crandoms() * 0.1f) * radiusmult * ui_SFXSabersGlowSizeSFX.value;
	coreradius = ((radius * 0.4) + crandoms() * 0.1f) * radiusmult * ui_SFXSabersCoreSizeSFX.value;
	
	effectradius *= 0.75;
	coreradius *= 1.25;

	{
		if (blade_len - ((effectradius * AngleScale) / 2) > 0)
		{
			saber.radius = effectradius * AngleScale;
			saber.saberLength = (blade_len - (saber.radius / 2));
			VectorCopy(blade_muz, saber.origin);
			VectorCopy(blade_dir, saber.axis[0]);
			saber.reType = RT_SABER_GLOW;
			saber.customShader = glow;
			saber.shaderRGBA[0] = 0xff * effectalpha;
			saber.shaderRGBA[1] = 0xff * effectalpha;
			saber.shaderRGBA[2] = 0xff * effectalpha;
			saber.shaderRGBA[3] = 0xff * effectalpha;

			if (color >= SABER_RGB)
			{
				if (whichSaber == 0)
				{
					saber.shaderRGBA[0] = ui_rgb_saber_red.integer * effectalpha;
					saber.shaderRGBA[1] = ui_rgb_saber_green.integer * effectalpha;
					saber.shaderRGBA[2] = ui_rgb_saber_blue.integer * effectalpha;
				}
				else
				{
					saber.shaderRGBA[0] = ui_rgb_saber2_red.integer * effectalpha;
					saber.shaderRGBA[1] = ui_rgb_saber2_green.integer * effectalpha;
					saber.shaderRGBA[2] = ui_rgb_saber2_blue.integer * effectalpha;
				}
			}

			DC->addRefEntityToScene(&saber);
		}

		// Do the hot core
		VectorMA(blade_muz, blade_len, blade_dir, saber.origin);
		VectorMA(blade_muz, -1, blade_dir, saber.oldorigin);

		saber.customShader = blade;

		saber.reType = RT_LINE;

		saber.radius = coreradius;

		saber.shaderTexCoord[0] = saber.shaderTexCoord[1] = 1.0f;
		saber.shaderRGBA[0] = saber.shaderRGBA[1] = saber.shaderRGBA[2] = saber.shaderRGBA[3] = 0xff;

		DC->addRefEntityToScene(&saber);
		DC->addRefEntityToScene(&saber);
	}
}

void UI_DoEp1Saber(vec3_t blade_muz, vec3_t blade_dir, float lengthMax, float radius, saber_colors_t color, saber_crystals_t crystals, int whichSaber)
{
	vec3_t	mid, rgb = { 1,1,1 };
	float	radiusmult, effectradius, coreradius;
	float	blade_len;
	float	effectalpha = 0.8f;
	float	AngleScale = 1.0f;

	qhandle_t	glow = 0, blade = 0;
	refEntity_t saber;

	blade_len = lengthMax;

	if (blade_len < 0.5f)
	{
		return;
	}

	switch (color)
	{
	case SABER_RED:
		glow = redEp1GlowShader;
		blade = ep1SaberCoreShader;
		VectorSet(rgb, 1.0f, 0.2f, 0.2f);
		break;
	case SABER_ORANGE:
		glow = orangeEp1GlowShader;
		blade = ep1SaberCoreShader;
		VectorSet(rgb, 1.0f, 0.5f, 0.1f);
		break;
	case SABER_YELLOW:
		glow = yellowEp1GlowShader;
		blade = ep1SaberCoreShader;
		VectorSet(rgb, 1.0f, 1.0f, 0.2f);
		break;
	case SABER_GREEN:
		glow = greenEp1GlowShader;
		blade = ep1SaberCoreShader;
		VectorSet(rgb, 0.2f, 1.0f, 0.2f);
		break;
	case SABER_BLUE:
		glow = blueEp1GlowShader;
		blade = ep1SaberCoreShader;
		VectorSet(rgb, 0.2f, 0.4f, 1.0f);
		break;
	case SABER_PURPLE:
		glow = purpleEp1GlowShader;
		blade = ep1SaberCoreShader;
		VectorSet(rgb, 0.9f, 0.2f, 1.0f);
		break;
	case SABER_RGB:
		glow = rgbSaberGlowShader;
		blade = ep1SaberCoreShader;
		break;
	case SABER_WHITE:
		glow = rgbSaberGlowShader;
		blade = ep1SaberCoreShader;
		VectorSet(rgb, 1.0f, 1.0f, 1.0f);
		break;
	case SABER_BLACK:
		glow = blackSaberGlowShader;
		blade = ep1blackSaberCoreShader;
		VectorSet(rgb, .0f, .0f, .0f);
		break;
	case SABER_UNSTABLE_RED:
		glow = redEp1GlowShader;
		blade = unstableRedSaberCoreShader;
		VectorSet(rgb, 1.0f, 0.2f, 0.2f);
		break;
	default:
		glow = rgbSaberGlowShader;
		blade = ep1SaberCoreShader;
		break;
	}

	VectorMA(blade_muz, blade_len * 0.5f, blade_dir, mid);

	memset(&saber, 0, sizeof(refEntity_t));

	if (blade_len < lengthMax)
	{
		radiusmult = 0.5 + ((blade_len / lengthMax) / 2);
	}
	else
	{
		radiusmult = 1.0;
	}


	effectradius = ((radius * 1.6) + crandoms() * 0.1f) * radiusmult * ui_SFXSabersGlowSizeEP1.value;
	coreradius = ((radius * 0.4) + crandoms() * 0.1f) * radiusmult * ui_SFXSabersCoreSizeEP1.value;

	effectradius *= 0.75;
	coreradius *= 1.5;

	{
		if (blade_len - ((effectradius * AngleScale) / 2) > 0)
		{
			saber.radius = effectradius * AngleScale;
			saber.saberLength = (blade_len - (saber.radius / 2));
			VectorCopy(blade_muz, saber.origin);
			VectorCopy(blade_dir, saber.axis[0]);
			saber.reType = RT_SABER_GLOW;
			saber.customShader = glow;
			saber.shaderRGBA[0] = 0xff * effectalpha;
			saber.shaderRGBA[1] = 0xff * effectalpha;
			saber.shaderRGBA[2] = 0xff * effectalpha;
			saber.shaderRGBA[3] = 0xff * effectalpha;

			if (color >= SABER_RGB)
			{
				if (whichSaber == 0)
				{
					saber.shaderRGBA[0] = ui_rgb_saber_red.integer * effectalpha;
					saber.shaderRGBA[1] = ui_rgb_saber_green.integer * effectalpha;
					saber.shaderRGBA[2] = ui_rgb_saber_blue.integer * effectalpha;
				}
				else
				{
					saber.shaderRGBA[0] = ui_rgb_saber2_red.integer * effectalpha;
					saber.shaderRGBA[1] = ui_rgb_saber2_green.integer * effectalpha;
					saber.shaderRGBA[2] = ui_rgb_saber2_blue.integer * effectalpha;
				}
			}

			DC->addRefEntityToScene(&saber);
		}

		// Do the hot core
		VectorMA(blade_muz, blade_len, blade_dir, saber.origin);
		VectorMA(blade_muz, -1, blade_dir, saber.oldorigin);

		saber.customShader = ep1SaberCoreShader;

		saber.reType = RT_LINE;

		saber.radius = coreradius;

		saber.shaderTexCoord[0] = saber.shaderTexCoord[1] = 1.0f;
		saber.shaderRGBA[0] = saber.shaderRGBA[1] = saber.shaderRGBA[2] = saber.shaderRGBA[3] = 0xff;

		DC->addRefEntityToScene(&saber);
		DC->addRefEntityToScene(&saber);
	}
	
}

void UI_DoEp2Saber(vec3_t blade_muz, vec3_t blade_dir, float lengthMax, float radius, saber_colors_t color, saber_crystals_t crystals, int whichSaber)
{
	vec3_t	mid, rgb = { 1,1,1 };
	float	radiusmult, effectradius, coreradius;
	float	blade_len;
	float	effectalpha = 0.8f;
	float	AngleScale = 1.0f;

	qhandle_t	glow = 0, blade = 0;
	refEntity_t saber;

	blade_len = lengthMax;

	if (blade_len < 0.5f)
	{
		return;
	}

	switch (color)
	{
	case SABER_RED:
		glow = redEp2GlowShader;
		blade = ep2SaberCoreShader;
		VectorSet(rgb, 1.0f, 0.2f, 0.2f);
		break;
	case SABER_ORANGE:
		glow = orangeEp2GlowShader;
		blade = ep2SaberCoreShader;
		VectorSet(rgb, 1.0f, 0.5f, 0.1f);
		break;
	case SABER_YELLOW:
		glow = yellowEp2GlowShader;
		blade = ep2SaberCoreShader;
		VectorSet(rgb, 1.0f, 1.0f, 0.2f);
		break;
	case SABER_GREEN:
		glow = greenEp2GlowShader;
		blade = ep2SaberCoreShader;
		VectorSet(rgb, 0.2f, 1.0f, 0.2f);
		break;
	case SABER_BLUE:
		glow = blueEp2GlowShader;
		blade = ep2SaberCoreShader;
		VectorSet(rgb, 0.2f, 0.4f, 1.0f);
		break;
	case SABER_PURPLE:
		glow = purpleEp2GlowShader;
		blade = ep2SaberCoreShader;
		VectorSet(rgb, 0.9f, 0.2f, 1.0f);
		break;
	case SABER_RGB:
		glow = rgbSaberGlowShader;
		blade = ep2SaberCoreShader;
		break;
	case SABER_WHITE:
		glow = rgbSaberGlowShader;
		blade = ep2SaberCoreShader;
		VectorSet(rgb, 1.0f, 1.0f, 1.0f);
		break;
	case SABER_BLACK:
		glow = blackSaberGlowShader;
		blade = ep2blackSaberCoreShader;
		VectorSet(rgb, .0f, .0f, .0f);
		break;
	case SABER_UNSTABLE_RED:
		glow = redEp2GlowShader;
		blade = unstableRedSaberCoreShader;
		VectorSet(rgb, 1.0f, 0.2f, 0.2f);
		break;
	default:
		glow = rgbSaberGlowShader;
		blade = ep2SaberCoreShader;
		break;
	}

	VectorMA(blade_muz, blade_len * 0.5f, blade_dir, mid);

	memset(&saber, 0, sizeof(refEntity_t));

	if (blade_len < lengthMax)
	{
		radiusmult = 0.5 + ((blade_len / lengthMax) / 2);
	}
	else
	{
		radiusmult = 1.0;
	}


	effectradius = ((radius * 1.6) + Q_flrand(-1.0f, 1.0f) * 0.1f) * radiusmult * ui_SFXSabersGlowSizeEP2.value;
	coreradius = ((radius * 0.4) + Q_flrand(-1.0f, 1.0f) * 0.1f) * radiusmult * ui_SFXSabersCoreSizeEP2.value;

	effectradius *= 0.75;
	coreradius *= 1.5;

	{
		if (blade_len - ((effectradius * AngleScale) / 2) > 0)
		{
			saber.radius = effectradius * AngleScale;
			saber.saberLength = (blade_len - (saber.radius / 2));
			VectorCopy(blade_muz, saber.origin);
			VectorCopy(blade_dir, saber.axis[0]);
			saber.reType = RT_SABER_GLOW;
			saber.customShader = glow;
			saber.shaderRGBA[0] = 0xff * effectalpha;
			saber.shaderRGBA[1] = 0xff * effectalpha;
			saber.shaderRGBA[2] = 0xff * effectalpha;
			saber.shaderRGBA[3] = 0xff * effectalpha;

			if (color >= SABER_RGB)
			{
				if (whichSaber == 0)
				{
					saber.shaderRGBA[0] = ui_rgb_saber_red.integer * effectalpha;
					saber.shaderRGBA[1] = ui_rgb_saber_green.integer * effectalpha;
					saber.shaderRGBA[2] = ui_rgb_saber_blue.integer * effectalpha;
				}
				else
				{
					saber.shaderRGBA[0] = ui_rgb_saber2_red.integer * effectalpha;
					saber.shaderRGBA[1] = ui_rgb_saber2_green.integer * effectalpha;
					saber.shaderRGBA[2] = ui_rgb_saber2_blue.integer * effectalpha;
				}
			}

			DC->addRefEntityToScene(&saber);
		}

		// Do the hot core
		VectorMA(blade_muz, blade_len, blade_dir, saber.origin);
		VectorMA(blade_muz, -1, blade_dir, saber.oldorigin);


		saber.customShader = blade;

		saber.reType = RT_LINE;

		saber.radius = coreradius;

		saber.shaderTexCoord[0] = saber.shaderTexCoord[1] = 1.0f;
		saber.shaderRGBA[0] = saber.shaderRGBA[1] = saber.shaderRGBA[2] = saber.shaderRGBA[3] = 0xff;

		DC->addRefEntityToScene(&saber);
		DC->addRefEntityToScene(&saber);
	}
}

void UI_DoEp3Saber(vec3_t blade_muz, vec3_t blade_dir, float lengthMax, float radius, saber_colors_t color, saber_crystals_t crystals, int whichSaber)
{
	vec3_t	mid, rgb = { 1,1,1 };
	float	radiusmult, effectradius, coreradius;
	float	blade_len;
	float	effectalpha = 0.8f;
	float	AngleScale = 1.0f;

	qhandle_t	glow = 0, blade = 0;
	refEntity_t saber;

	blade_len = lengthMax;

	if (blade_len < 0.5f)
	{
		return;
	}

	switch (color)
	{
	case SABER_RED:
		glow = redEp3GlowShader;
		blade = ep3redSaberCoreShader;
		VectorSet(rgb, 1.0f, 0.2f, 0.2f);
		break;
	case SABER_ORANGE:
		glow = orangeEp3GlowShader;
		blade = ep3orangeSaberCoreShader;
		VectorSet(rgb, 1.0f, 0.5f, 0.1f);
		break;
	case SABER_YELLOW:
		glow = yellowEp3GlowShader;
		blade = ep3yellowSaberCoreShader;
		VectorSet(rgb, 1.0f, 1.0f, 0.2f);
		break;
	case SABER_GREEN:
		glow = greenEp3GlowShader;
		blade = ep3greenSaberCoreShader;
		VectorSet(rgb, 0.2f, 1.0f, 0.2f);
		break;
	case SABER_BLUE:
		glow = blueEp3GlowShader;
		blade = ep3blueSaberCoreShader;
		VectorSet(rgb, 0.2f, 0.4f, 1.0f);
		break;
	case SABER_PURPLE:
		glow = purpleEp3GlowShader;
		blade = ep3purpleSaberCoreShader;
		VectorSet(rgb, 0.9f, 0.2f, 1.0f);
		break;
	case SABER_RGB:
		glow = rgbSaberGlowShader;
		blade = ep3SaberCoreShader;
		break;
	case SABER_WHITE:
		glow = rgbSaberGlowShader;
		blade = ep3SaberCoreShader;
		VectorSet(rgb, 1.0f, 1.0f, 1.0f);
		break;
	case SABER_BLACK:
		glow = blackSaberGlowShader;
		blade = ep3blackSaberCoreShader;
		VectorSet(rgb, .0f, .0f, .0f);
		break;
	case SABER_UNSTABLE_RED:
		glow = redEp3GlowShader;
		blade = unstableRedSaberCoreShader;
		VectorSet(rgb, 1.0f, 0.2f, 0.2f);
		break;
	default:
		glow = rgbSaberGlowShader;
		blade = ep3SaberCoreShader;
		break;
	}

	VectorMA(blade_muz, blade_len * 0.5f, blade_dir, mid);

	memset(&saber, 0, sizeof(refEntity_t));

	if (blade_len < lengthMax)
	{
		radiusmult = 0.5 + ((blade_len / lengthMax) / 2);
	}
	else
	{
		radiusmult = 1.0;
	}

	effectradius = ((radius * 1.6) + Q_flrand(-1.0f, 1.0f) * 0.1f) * radiusmult * ui_SFXSabersGlowSizeEP3.value;
	coreradius = ((radius * 0.4) + Q_flrand(-1.0f, 1.0f) * 0.1f) * radiusmult * ui_SFXSabersCoreSizeEP3.value;

	effectradius *= 0.75;

	{
		if (blade_len - ((effectradius * AngleScale) / 2) > 0)
		{
			saber.radius = effectradius * AngleScale;
			saber.saberLength = (blade_len - (saber.radius / 2));
			VectorCopy(blade_muz, saber.origin);
			VectorCopy(blade_dir, saber.axis[0]);
			saber.reType = RT_SABER_GLOW;
			saber.customShader = glow;
			saber.shaderRGBA[0] = 0xff * effectalpha;
			saber.shaderRGBA[1] = 0xff * effectalpha;
			saber.shaderRGBA[2] = 0xff * effectalpha;
			saber.shaderRGBA[3] = 0xff * effectalpha;

			if (color >= SABER_RGB)
			{
				if (whichSaber == 0)
				{
					saber.shaderRGBA[0] = ui_rgb_saber_red.integer * effectalpha;
					saber.shaderRGBA[1] = ui_rgb_saber_green.integer * effectalpha;
					saber.shaderRGBA[2] = ui_rgb_saber_blue.integer * effectalpha;
				}
				else
				{
					saber.shaderRGBA[0] = ui_rgb_saber2_red.integer * effectalpha;
					saber.shaderRGBA[1] = ui_rgb_saber2_green.integer * effectalpha;
					saber.shaderRGBA[2] = ui_rgb_saber2_blue.integer * effectalpha;
				}
			}

			DC->addRefEntityToScene(&saber);
		}

		// Do the hot core
		VectorMA(blade_muz, blade_len, blade_dir, saber.origin);
		VectorMA(blade_muz, -1, blade_dir, saber.oldorigin);

		saber.customShader = blade;

		saber.reType = RT_LINE;

		saber.radius = coreradius;

		saber.shaderTexCoord[0] = saber.shaderTexCoord[1] = 1.0f;
		saber.shaderRGBA[0] = saber.shaderRGBA[1] = saber.shaderRGBA[2] = saber.shaderRGBA[3] = 0xff;

		DC->addRefEntityToScene(&saber);
		DC->addRefEntityToScene(&saber);
	}
}

void UI_DoOTSaber(vec3_t blade_muz, vec3_t blade_dir, float lengthMax, float radius, saber_colors_t color, saber_crystals_t crystals, int whichSaber)
{
	vec3_t	mid, rgb = { 1,1,1 };
	float	radiusmult, effectradius, coreradius;
	float	blade_len;
	float	glowscale = 0.5;
	float	effectalpha = 0.8f;
	float	AngleScale = 1.0f;

	qhandle_t	glow = 0, blade = 0;
	refEntity_t saber;

	blade_len = lengthMax;

	if (blade_len < 0.5f)
	{
		return;
	}

	switch (color)
	{
	case SABER_RED:
		glow = redOTGlowShader;
		blade = otSaberCoreShader;
		VectorSet(rgb, 1.0f, 0.2f, 0.2f);
		break;
	case SABER_ORANGE:
		glow = orangeOTGlowShader;
		blade = otSaberCoreShader;
		VectorSet(rgb, 1.0f, 0.5f, 0.1f);
		break;
	case SABER_YELLOW:
		glow = yellowOTGlowShader;
		blade = otSaberCoreShader;
		VectorSet(rgb, 1.0f, 1.0f, 0.2f);
		break;
	case SABER_GREEN:
		glow = greenOTGlowShader;
		blade = otSaberCoreShader;
		VectorSet(rgb, 0.2f, 1.0f, 0.2f);
		break;
	case SABER_BLUE:
		glow = blueOTGlowShader;
		blade = otSaberCoreShader;
		VectorSet(rgb, 0.2f, 0.4f, 1.0f);
		break;
	case SABER_PURPLE:
		glow = purpleOTGlowShader;
		blade = otSaberCoreShader;
		VectorSet(rgb, 0.9f, 0.2f, 1.0f);
		break;
	case SABER_RGB:
		glow = rgbSaberGlowShader;
		blade = otSaberCoreShader;
	case SABER_WHITE:
		glow = rgbSaberGlowShader;
		blade = otSaberCoreShader;
		VectorSet(rgb, 1.0f, 1.0f, 1.0f);
		break;
	case SABER_BLACK:
		glow = blackSaberGlowShader;
		blade = sfxblackSaberBladeShader;
		VectorSet(rgb, .0f, .0f, .0f);
		break;
	case SABER_UNSTABLE_RED:
		glow = redOTGlowShader;
		blade = unstableRedSaberCoreShader;
		break;
	default:
		glow = rgbSaberGlowShader;
		blade = otSaberCoreShader;
		break;
	}

	VectorMA(blade_muz, blade_len * 0.5f, blade_dir, mid);

	memset(&saber, 0, sizeof(refEntity_t));

	if (blade_len < lengthMax)
	{
		radiusmult = 0.5 + ((blade_len / lengthMax) / 2);
	}
	else
	{
		radiusmult = 1.0;
	}

	effectradius = ((radius * 1.6) + flrand(0.3f, 1.8f)) * radiusmult * ui_SFXSabersGlowSizeOT.value;
	coreradius = ((radius * 0.4) + flrand(0.1f, 1.0f)) * radiusmult * ui_SFXSabersCoreSizeOT.value;

	coreradius *= 0.5f;

	{
		if (blade_len - ((effectradius * AngleScale) / 2) > 0)
		{
			saber.radius = effectradius * AngleScale;
			saber.saberLength = (blade_len - (saber.radius / 2));
			VectorCopy(blade_muz, saber.origin);
			VectorCopy(blade_dir, saber.axis[0]);
			saber.reType = RT_SABER_GLOW;
			saber.customShader = glow;
			saber.shaderRGBA[0] = 0xff * effectalpha;
			saber.shaderRGBA[1] = 0xff * effectalpha;
			saber.shaderRGBA[2] = 0xff * effectalpha;
			saber.shaderRGBA[3] = 0xff * effectalpha;

			if (color >= SABER_RGB)
			{
				if (whichSaber == 0)
				{
					saber.shaderRGBA[0] = ui_rgb_saber_red.integer * effectalpha;
					saber.shaderRGBA[1] = ui_rgb_saber_green.integer * effectalpha;
					saber.shaderRGBA[2] = ui_rgb_saber_blue.integer * effectalpha;
				}
				else
				{
					saber.shaderRGBA[0] = ui_rgb_saber2_red.integer * effectalpha;
					saber.shaderRGBA[1] = ui_rgb_saber2_green.integer * effectalpha;
					saber.shaderRGBA[2] = ui_rgb_saber2_blue.integer * effectalpha;
				}
			}

			DC->addRefEntityToScene(&saber);
		}

		// Do the hot core
		VectorMA(blade_muz, blade_len, blade_dir, saber.origin);
		VectorMA(blade_muz, -1, blade_dir, saber.oldorigin);

		saber.customShader = blade;

		saber.reType = RT_LINE;

		saber.radius = coreradius;

		saber.shaderTexCoord[0] = saber.shaderTexCoord[1] = 1.0f;
		saber.shaderRGBA[0] = saber.shaderRGBA[1] = saber.shaderRGBA[2] = saber.shaderRGBA[3] = 0xff;

		DC->addRefEntityToScene(&saber);
		DC->addRefEntityToScene(&saber);
	}
}

void UI_DoRotJSaber(vec3_t blade_muz, vec3_t blade_dir, float lengthMax, float radius, saber_colors_t color, saber_crystals_t crystals, int whichSaber)
{
	vec3_t	mid;
	float	radiusmult, effectradius, coreradius;
	float	blade_len;
	float	effectalpha = 0.8f;
	float	AngleScale = 1.0f;

	qhandle_t	blade = 0, glow = 0;
	refEntity_t saber;

	blade_len = lengthMax;

	if (blade_len < 0.5f)
	{
		return;
	}

	switch (color)
	{
	case SABER_RED:
		glow = redEp6GlowShader;
		blade = ep6SaberCoreShader;
		break;
	case SABER_ORANGE:
		glow = orangeEp6GlowShader;
		blade = ep6SaberCoreShader;
		break;
	case SABER_YELLOW:
		glow = yellowEp6GlowShader;
		blade = ep6SaberCoreShader;
		break;
	case SABER_GREEN:
		glow = greenEp6GlowShader;
		blade = ep6SaberCoreShader;
		break;
	case SABER_BLUE:
		glow = blueEp6GlowShader;
		blade = ep6SaberCoreShader;
		break;
	case SABER_PURPLE:
		glow = purpleEp6GlowShader;
		blade = ep6SaberCoreShader;
		break;
	case SABER_RGB:
		glow = rgbSaberGlowShader;
		blade = ep6SaberCoreShader;
		break;
	case SABER_WHITE:
		glow = rgbSaberGlowShader;
		blade = ep6SaberCoreShader;
		break;
	case SABER_BLACK:
		glow = blackSaberGlowShader;
		blade = sfxblackSaberBladeShader;
		break;
	case SABER_UNSTABLE_RED:
		glow = redEp6GlowShader;
		blade = unstableRedSaberCoreShader;
		break;
	default:
		glow = rgbSaberGlowShader;
		blade = ep6SaberCoreShader;
		break;
	}

	VectorMA(blade_muz, blade_len * 0.5f, blade_dir, mid);

	memset(&saber, 0, sizeof(refEntity_t));

	if (blade_len < lengthMax)
	{
		radiusmult = 0.5 + ((blade_len / lengthMax) / 2);
	}
	else
	{
		radiusmult = 1.0;
	}

	effectradius = ((radius * 1.6) + crandoms() * 0.1f) * radiusmult * ui_SFXSabersGlowSizeROTJ.value;
	coreradius = ((radius * 0.4) + crandoms() * 0.1f) * radiusmult * ui_SFXSabersCoreSizeROTJ.value;

	effectradius *= 0.75;
	coreradius *= 0.75;

	{
		if (blade_len - ((effectradius * AngleScale) / 2) > 0)
		{
			saber.radius = effectradius * AngleScale;
			saber.saberLength = (blade_len - (saber.radius / 2));
			VectorCopy(blade_muz, saber.origin);
			VectorCopy(blade_dir, saber.axis[0]);
			saber.reType = RT_SABER_GLOW;
			saber.customShader = glow;
			saber.shaderRGBA[0] = 0xff * effectalpha;
			saber.shaderRGBA[1] = 0xff * effectalpha;
			saber.shaderRGBA[2] = 0xff * effectalpha;
			saber.shaderRGBA[3] = 0xff * effectalpha;

			if (color >= SABER_RGB)
			{
				if (whichSaber == 0)
				{
					saber.shaderRGBA[0] = ui_rgb_saber_red.integer * effectalpha;
					saber.shaderRGBA[1] = ui_rgb_saber_green.integer * effectalpha;
					saber.shaderRGBA[2] = ui_rgb_saber_blue.integer * effectalpha;
				}
				else
				{
					saber.shaderRGBA[0] = ui_rgb_saber2_red.integer * effectalpha;
					saber.shaderRGBA[1] = ui_rgb_saber2_green.integer * effectalpha;
					saber.shaderRGBA[2] = ui_rgb_saber2_blue.integer * effectalpha;
				}
			}

			DC->addRefEntityToScene(&saber);
		}

		// Do the hot core
		VectorMA(blade_muz, blade_len, blade_dir, saber.origin);
		VectorMA(blade_muz, -1, blade_dir, saber.oldorigin);

		saber.customShader = blade;

		saber.reType = RT_LINE;

		saber.radius = coreradius;

		saber.shaderTexCoord[0] = saber.shaderTexCoord[1] = 1.0f;
		saber.shaderRGBA[0] = saber.shaderRGBA[1] = saber.shaderRGBA[2] = saber.shaderRGBA[3] = 0xff;

		DC->addRefEntityToScene(&saber);
		DC->addRefEntityToScene(&saber);
	}
}

void UI_DoTFASaber(vec3_t blade_muz, vec3_t blade_dir, float lengthMax, float radius, saber_colors_t color, saber_crystals_t crystals, int whichSaber)
{
	vec3_t	mid;
	float	radiusmult, effectradius, coreradius;
	float	blade_len;
	float	effectalpha = 0.8f;
	float	AngleScale = 1.0f;

	qhandle_t	blade = 0, glow = 0;
	refEntity_t saber;

	blade_len = lengthMax;

	if (blade_len < 0.5f)
	{
		return;
	}

	switch (color)
	{
	case SABER_RED:
		glow = redEp7GlowShader;
		blade = ep7redSaberCoreShader;
		break;
	case SABER_ORANGE:
		glow = orangeEp7GlowShader;
		blade = ep7orangeSaberCoreShader;
		break;
	case SABER_YELLOW:
		glow = yellowEp7GlowShader;
		blade = ep7yellowSaberCoreShader;
		break;
	case SABER_GREEN:
		glow = greenEp7GlowShader;
		blade = ep7greenSaberCoreShader;
		break;
	case SABER_BLUE:
		glow = blueEp7GlowShader;
		blade = ep7blueSaberCoreShader;
		break;
	case SABER_PURPLE:
		glow = purpleEp7GlowShader;
		blade = ep7purpleSaberCoreShader;
		break;
	case SABER_RGB:
		glow = rgbSaberGlowShader;
		blade = ep7SaberCoreShader;
		break;
	case SABER_WHITE:
		glow = rgbSaberGlowShader;
		blade = ep7SaberCoreShader;
		break;
	case SABER_BLACK:
		glow = blackSaberGlowShader;
		blade = ep7blackSaberCoreShader;
		break;
	case SABER_UNSTABLE_RED:
		glow = redEp7GlowShader;
		blade = unstableRedSaberCoreShader;
		break;
	default:
		glow = rgbSaberGlowShader;
		blade = ep7SaberCoreShader;
		break;
	}

	VectorMA(blade_muz, blade_len * 0.5f, blade_dir, mid);

	memset(&saber, 0, sizeof(refEntity_t));

	if (blade_len < lengthMax)
	{
		radiusmult = 0.5 + ((blade_len / lengthMax) / 2);
	}
	else
	{
		radiusmult = 1.0;
	}

	effectradius = ((radius * 1.6) + crandoms() * 0.1f) * radiusmult * ui_SFXSabersGlowSizeTFA.value;
	coreradius = ((radius * 0.4) + crandoms() * 0.1f) * radiusmult * ui_SFXSabersCoreSizeTFA.value;

	effectradius *= 0.75;

	{
		if (blade_len - ((effectradius * AngleScale) / 2) > 0)
		{
			saber.radius = effectradius * AngleScale;
			saber.saberLength = (blade_len - (saber.radius / 2));
			VectorCopy(blade_muz, saber.origin);
			VectorCopy(blade_dir, saber.axis[0]);
			saber.reType = RT_SABER_GLOW;
			saber.customShader = glow;
			saber.shaderRGBA[0] = 0xff * effectalpha;
			saber.shaderRGBA[1] = 0xff * effectalpha;
			saber.shaderRGBA[2] = 0xff * effectalpha;
			saber.shaderRGBA[3] = 0xff * effectalpha;

			if (color >= SABER_RGB)
			{
				if (whichSaber == 0)
				{
					saber.shaderRGBA[0] = ui_rgb_saber_red.integer * effectalpha;
					saber.shaderRGBA[1] = ui_rgb_saber_green.integer * effectalpha;
					saber.shaderRGBA[2] = ui_rgb_saber_blue.integer * effectalpha;
				}
				else
				{
					saber.shaderRGBA[0] = ui_rgb_saber2_red.integer * effectalpha;
					saber.shaderRGBA[1] = ui_rgb_saber2_green.integer * effectalpha;
					saber.shaderRGBA[2] = ui_rgb_saber2_blue.integer * effectalpha;
				}
			}

			DC->addRefEntityToScene(&saber);
		}

		// Do the hot core
		VectorMA(blade_muz, blade_len, blade_dir, saber.origin);
		VectorMA(blade_muz, -1, blade_dir, saber.oldorigin);

		saber.customShader = blade;

		saber.reType = RT_LINE;

		saber.radius = coreradius;

		saber.shaderTexCoord[0] = saber.shaderTexCoord[1] = 1.0f;
		saber.shaderRGBA[0] = saber.shaderRGBA[1] = saber.shaderRGBA[2] = saber.shaderRGBA[3] = 0xff;

		DC->addRefEntityToScene(&saber);
		DC->addRefEntityToScene(&saber);
	}
}

void UI_DoUnstableSaber(vec3_t blade_muz, vec3_t blade_dir, float lengthMax, float radius, saber_colors_t color, int whichSaber)
{
	vec3_t	mid, rgb = { 1,1,1 };
	float	radiusmult, effectradius, coreradius;
	float	blade_len;
	float	effectalpha = 0.8f;
	float	AngleScale = 1.0f;

	qhandle_t	glow = 0, blade = 0;
	refEntity_t saber;

	blade_len = lengthMax;

	if (blade_len < 0.5f)
	{
		return;
	}

	switch (color)
	{
	case SABER_RED:
		glow = redSaberGlowShader;
		if (ui_SerenityJediEngineMode.integer)
		{
			blade = rgbTFASaberCoreShader;
		}
		else
		{
			blade = unstableRedSaberCoreShader;
		}
		VectorSet(rgb, 1.0f, 0.2f, 0.2f);
		break;
	case SABER_ORANGE:
		glow = orangeSaberGlowShader;
		if (ui_SerenityJediEngineMode.integer)
		{
			blade = rgbTFASaberCoreShader;
		}
		else
		{
			blade = unstableRedSaberCoreShader;
		}
		VectorSet(rgb, 1.0f, 0.5f, 0.1f);
		break;
	case SABER_YELLOW:
		glow = yellowSaberGlowShader;
		if (ui_SerenityJediEngineMode.integer)
		{
			blade = rgbTFASaberCoreShader;
		}
		else
		{
			blade = unstableRedSaberCoreShader;
		}
		VectorSet(rgb, 1.0f, 1.0f, 0.2f);
		break;
	case SABER_GREEN:
		glow = greenSaberGlowShader;
		if (ui_SerenityJediEngineMode.integer)
		{
			blade = rgbTFASaberCoreShader;
		}
		else
		{
			blade = unstableRedSaberCoreShader;
		}
		VectorSet(rgb, 0.2f, 1.0f, 0.2f);
		break;
	case SABER_BLUE:
		glow = blueSaberGlowShader;
		if (ui_SerenityJediEngineMode.integer)
		{
			blade = rgbTFASaberCoreShader;
		}
		else
		{
			blade = unstableRedSaberCoreShader;
		}
		VectorSet(rgb, 0.2f, 0.4f, 1.0f);
		break;
	case SABER_PURPLE:
		glow = purpleSaberGlowShader;
		if (ui_SerenityJediEngineMode.integer)
		{
			blade = rgbTFASaberCoreShader;
		}
		else
		{
			blade = unstableRedSaberCoreShader;
		}
		VectorSet(rgb, 0.9f, 0.2f, 1.0f);
		break;
	case SABER_RGB:
	case SABER_WHITE:
		glow = rgbSaberGlowShader;
		if (ui_SerenityJediEngineMode.integer)
		{
			blade = rgbTFASaberCoreShader;
		}
		else
		{
			blade = unstableRedSaberCoreShader;
		}
		VectorSet(rgb, 1.0f, 1.0f, 1.0f);
		break;
	case SABER_BLACK:
		glow = blackSaberGlowShader;
		blade = blackSaberCoreShader;
		VectorSet(rgb, .0f, .0f, .0f);
		break;
	case SABER_CUSTOM:
		glow = rgbSaberGlowShader;
		if (ui_SerenityJediEngineMode.integer)
		{
			blade = rgbTFASaberCoreShader;
		}
		else
		{
			blade = unstableRedSaberCoreShader;
		}
		break;
	case SABER_UNSTABLE_RED:
		glow = redSaberGlowShader;
		blade = unstableRedSaberCoreShader;
		VectorSet(rgb, 1.0f, 0.2f, 0.2f);
		break;
	default:
		glow = rgbSaberGlowShader;
		if (ui_SerenityJediEngineMode.integer)
		{
			blade = rgbTFASaberCoreShader;
		}
		else
		{
			blade = unstableRedSaberCoreShader;
		}
		break;
	}

	VectorMA(blade_muz, blade_len * 0.5f, blade_dir, mid);

	memset(&saber, 0, sizeof(refEntity_t));

	if (blade_len < lengthMax)
	{
		radiusmult = 0.5 + ((blade_len / lengthMax) / 2);
	}
	else
	{
		radiusmult = 1.0;
	}

	effectradius = ((radius * 1.6f) + Q_flrand(-1.0f, 1.0f) * 0.1f) * radiusmult * ui_SFXSabersGlowSizeUSB.value;
	coreradius = ((radius * 0.4f) + Q_flrand(-1.0f, 1.0f) * 0.1f) * radiusmult * ui_SFXSabersCoreSizeUSB.value;

	{
		if (blade_len - ((effectradius * AngleScale) / 2) > 0)
		{
			saber.radius = effectradius * AngleScale;
			saber.saberLength = (blade_len - (saber.radius / 2));
			VectorCopy(blade_muz, saber.origin);
			VectorCopy(blade_dir, saber.axis[0]);
			saber.reType = RT_SABER_GLOW;
			saber.customShader = glow;
			saber.shaderRGBA[0] = 0xff * effectalpha;
			saber.shaderRGBA[1] = 0xff * effectalpha;
			saber.shaderRGBA[2] = 0xff * effectalpha;
			saber.shaderRGBA[3] = 0xff * effectalpha;

			if (color >= SABER_RGB)
			{
				if (whichSaber == 0)
				{
					saber.shaderRGBA[0] = ui_rgb_saber_red.integer * effectalpha;
					saber.shaderRGBA[1] = ui_rgb_saber_green.integer * effectalpha;
					saber.shaderRGBA[2] = ui_rgb_saber_blue.integer * effectalpha;
				}
				else
				{
					saber.shaderRGBA[0] = ui_rgb_saber2_red.integer * effectalpha;
					saber.shaderRGBA[1] = ui_rgb_saber2_green.integer * effectalpha;
					saber.shaderRGBA[2] = ui_rgb_saber2_blue.integer * effectalpha;
				}
			}

			DC->addRefEntityToScene(&saber);
		}

		// Do the hot core
		VectorMA(blade_muz, blade_len, blade_dir, saber.origin);
		VectorMA(blade_muz, -1, blade_dir, saber.oldorigin);

		saber.customShader = blade;

		saber.reType = RT_LINE;

		saber.radius = coreradius;

		saber.shaderTexCoord[0] = saber.shaderTexCoord[1] = 1.0f;
		saber.shaderRGBA[0] = saber.shaderRGBA[1] = saber.shaderRGBA[2] = saber.shaderRGBA[3] = 0xff;

		DC->addRefEntityToScene(&saber);
		DC->addRefEntityToScene(&saber);
	}
}

void UI_DoSaberUnstable(vec3_t blade_muz, vec3_t blade_dir, float lengthMax, float radius, saber_colors_t color, int whichSaber)
{
	vec3_t	mid, rgb = { 1,1,1 };
	float	radiusmult, effectradius, coreradius;
	float	blade_len;
	float	effectalpha = 0.8f;
	float	AngleScale = 1.0f;

	qhandle_t	glow = 0, blade = 0;
	refEntity_t saber;

	blade_len = lengthMax;

	if (blade_len < 0.5f)
	{
		return;
	}

	switch (color)
	{
	case SABER_RED:
		glow = redSaberGlowShader;
		blade = unstableRedSaberCoreShader;
		VectorSet(rgb, 1.0f, 0.2f, 0.2f);
		break;
	case SABER_ORANGE:
		glow = orangeSaberGlowShader;
		blade = unstableRedSaberCoreShader;
		VectorSet(rgb, 1.0f, 0.5f, 0.1f);
		break;
	case SABER_YELLOW:
		glow = yellowSaberGlowShader;
		blade = unstableRedSaberCoreShader;
		VectorSet(rgb, 1.0f, 1.0f, 0.2f);
		break;
	case SABER_GREEN:
		glow = greenSaberGlowShader;
		blade = unstableRedSaberCoreShader;
		VectorSet(rgb, 0.2f, 1.0f, 0.2f);
		break;
	case SABER_BLUE:
		glow = blueSaberGlowShader;
		blade = unstableRedSaberCoreShader;
		VectorSet(rgb, 0.2f, 0.4f, 1.0f);
		break;
	case SABER_PURPLE:
		glow = purpleSaberGlowShader;
		blade = unstableRedSaberCoreShader;
		VectorSet(rgb, 0.9f, 0.2f, 1.0f);
		break;
	case SABER_WHITE:
	case SABER_RGB:
		glow = rgbSaberGlowShader;
		blade = unstableRedSaberCoreShader;
		break;
	case SABER_BLACK:
		glow = blackSaberGlowShader;
		blade = blackSaberCoreShader;
		VectorSet(rgb, .0f, .0f, .0f);
		break;
	case SABER_UNSTABLE_RED:
		glow = redSaberGlowShader;
		blade = unstableRedSaberCoreShader;
		VectorSet(rgb, 1.0f, 0.2f, 0.2f);
		break;
	default:
		glow = rgbSaberGlowShader;
		blade = unstableRedSaberCoreShader;
		break;
	}

	VectorMA(blade_muz, blade_len * 0.5f, blade_dir, mid);

	memset(&saber, 0, sizeof(refEntity_t));

	if (blade_len < lengthMax)
	{
		radiusmult = 0.5 + ((blade_len / lengthMax) / 2);
	}
	else
	{
		radiusmult = 1.0;
	}

	effectradius = ((radius * 1.6f) + Q_flrand(-1.0f, 1.0f) * 0.1f) * radiusmult;
	coreradius = ((radius * 0.4f) + Q_flrand(-1.0f, 1.0f) * 0.1f) * radiusmult;

	{
		if (blade_len - ((effectradius * AngleScale) / 2) > 0)
		{
			saber.radius = effectradius * AngleScale;
			saber.saberLength = (blade_len - (saber.radius / 2));
			VectorCopy(blade_muz, saber.origin);
			VectorCopy(blade_dir, saber.axis[0]);
			saber.reType = RT_SABER_GLOW;
			saber.customShader = glow;
			saber.shaderRGBA[0] = 0xff * effectalpha;
			saber.shaderRGBA[1] = 0xff * effectalpha;
			saber.shaderRGBA[2] = 0xff * effectalpha;
			saber.shaderRGBA[3] = 0xff * effectalpha;

			if (color >= SABER_RGB)
			{
				if (whichSaber == 0)
				{
					saber.shaderRGBA[0] = ui_rgb_saber_red.integer * effectalpha;
					saber.shaderRGBA[1] = ui_rgb_saber_green.integer * effectalpha;
					saber.shaderRGBA[2] = ui_rgb_saber_blue.integer * effectalpha;
				}
				else
				{
					saber.shaderRGBA[0] = ui_rgb_saber2_red.integer * effectalpha;
					saber.shaderRGBA[1] = ui_rgb_saber2_green.integer * effectalpha;
					saber.shaderRGBA[2] = ui_rgb_saber2_blue.integer * effectalpha;
				}
			}

			DC->addRefEntityToScene(&saber);
		}

		// Do the hot core
		VectorMA(blade_muz, blade_len, blade_dir, saber.origin);
		VectorMA(blade_muz, -1, blade_dir, saber.oldorigin);

		saber.customShader = blade;

		saber.reType = RT_LINE;

		saber.radius = coreradius;

		saber.shaderTexCoord[0] = saber.shaderTexCoord[1] = 1.0f;
		saber.shaderRGBA[0] = saber.shaderRGBA[1] = saber.shaderRGBA[2] = saber.shaderRGBA[3] = 0xff;

		DC->addRefEntityToScene(&saber);
		DC->addRefEntityToScene(&saber);
	}
}

void UI_DoSaber(vec3_t origin, vec3_t dir, float length, float lengthMax, float radius, saber_colors_t color, int whichSaber)
{
	vec3_t		mid, rgb = { 1,1,1 };
	qhandle_t	blade = 0, glow = 0;
	refEntity_t saber;
	float radiusmult;

	if (length < 0.5f)
	{
		// if the thing is so short, just forget even adding me.
		return;
	}

	// Find the midpoint of the saber for lighting purposes
	VectorMA(origin, length * 0.5f, dir, mid);

	switch (color)
	{
	case SABER_RED:
		glow = redSaberGlowShader;
		blade = redSaberCoreShader;
		VectorSet(rgb, 1.0f, 0.2f, 0.2f);
		break;
	case SABER_ORANGE:
		glow = orangeSaberGlowShader;
		blade = orangeSaberCoreShader;
		VectorSet(rgb, 1.0f, 0.5f, 0.1f);
		break;
	case SABER_YELLOW:
		glow = yellowSaberGlowShader;
		blade = yellowSaberCoreShader;
		VectorSet(rgb, 1.0f, 1.0f, 0.2f);
		break;
	case SABER_GREEN:
		glow = greenSaberGlowShader;
		blade = greenSaberCoreShader;
		VectorSet(rgb, 0.2f, 1.0f, 0.2f);
		break;
	case SABER_BLUE:
		glow = blueSaberGlowShader;
		blade = blueSaberCoreShader;
		VectorSet(rgb, 0.2f, 0.4f, 1.0f);
		break;
	case SABER_PURPLE:
		glow = purpleSaberGlowShader;
		blade = purpleSaberCoreShader;
		VectorSet(rgb, 0.9f, 0.2f, 1.0f);
		break;
	case SABER_RGB:
		glow = rgbSaberGlowShader;
		blade = rgbSaberCoreShader;
		break;
	case SABER_WHITE:
		glow = rgbSaberGlowShader;
		blade = rgbSaberCoreShader;
		VectorSet(rgb, 1.0f, 1.0f, 1.0f);
		break;
	case SABER_BLACK:
		glow = blackSaberGlowShader;
		blade = blackSaberCoreShader;
		VectorSet(rgb, .0f, .0f, .0f);
		break;
	case SABER_UNSTABLE_RED:
		glow = redSaberGlowShader;
		blade = unstableRedSaberCoreShader;
		break;
	default:
		glow = rgbSaberGlowShader;
		blade = rgbSaberCoreShader;
		break;
	}

	memset(&saber, 0, sizeof(refEntity_t));

	// Saber glow is it's own ref type because it uses a ton of sprites, otherwise it would eat up too many
	//	refEnts to do each glow blob individually
	saber.saberLength = length;

	// Jeff, I did this because I foolishly wished to have a bright halo as the saber is unleashed.
	// It's not quite what I'd hoped tho.  If you have any ideas, go for it!  --Pat
	if (length < lengthMax)
	{
		radiusmult = 1.0 + (2.0 / length);		// Note this creates a curve, and length cannot be < 0.5.
	}
	else
	{
		radiusmult = 1.0;
	}

	float radiusRange = radius * 0.075f;
	float radiusStart = radius - radiusRange;

	saber.radius = (radiusStart + Q_flrand(-1.0f, 1.0f) * radiusRange) * radiusmult;


	VectorCopy(origin, saber.origin);
	VectorCopy(dir, saber.axis[0]);
	saber.reType = RT_SABER_GLOW;
	saber.customShader = glow;
	saber.shaderRGBA[0] = saber.shaderRGBA[1] = saber.shaderRGBA[2] = saber.shaderRGBA[3] = 0xff;

	if (color >= SABER_RGB)
	{
		if (whichSaber == 0)
		{
			saber.shaderRGBA[0] = ui_rgb_saber_red.integer;
			saber.shaderRGBA[1] = ui_rgb_saber_green.integer;
			saber.shaderRGBA[2] = ui_rgb_saber_blue.integer;
		}
		else
		{
			saber.shaderRGBA[0] = ui_rgb_saber2_red.integer;
			saber.shaderRGBA[1] = ui_rgb_saber2_green.integer;
			saber.shaderRGBA[2] = ui_rgb_saber2_blue.integer;
		}
	}

	DC->addRefEntityToScene(&saber);

	// Do the hot core
	VectorMA(origin, length, dir, saber.origin);
	VectorMA(origin, -1, dir, saber.oldorigin);
	saber.customShader = blade;
	saber.reType = RT_LINE;
	radiusStart = radius / 3.0f;
	saber.radius = (radiusStart + Q_flrand(-1.0f, 1.0f) * radiusRange) * radiusmult;

	DC->addRefEntityToScene(&saber);

	if (color != SABER_RGB)
		return;

	saber.customShader = rgbSaberCoreShader;
	saber.reType = RT_LINE;
	saber.shaderTexCoord[0] = saber.shaderTexCoord[1] = 1.0f;
	saber.shaderRGBA[0] = saber.shaderRGBA[1] = saber.shaderRGBA[2] = saber.shaderRGBA[3] = 0xff;
	saber.radius = (radiusStart + Q_flrand(-1.0f, 1.0f) * radiusRange) * radiusmult;
	DC->addRefEntityToScene(&saber);
}

saber_colors_t TranslateSaberColor(const char* name)
{
	if (!Q_stricmp(name, "red"))
	{
		return SABER_RED;
	}
	if (!Q_stricmp(name, "orange"))
	{
		return SABER_ORANGE;
	}
	if (!Q_stricmp(name, "yellow"))
	{
		return SABER_YELLOW;
	}
	if (!Q_stricmp(name, "green"))
	{
		return SABER_GREEN;
	}
	if (!Q_stricmp(name, "blue"))
	{
		return SABER_BLUE;
	}
	if (!Q_stricmp(name, "purple"))
	{
		return SABER_PURPLE;
	}
	if (!Q_stricmp(name, "rgb"))
	{
		return SABER_RGB;
	}
	if (!Q_stricmp(name, "white"))
	{
		return SABER_WHITE;
	}
	if (!Q_stricmp(name, "black"))
	{
		return SABER_BLACK;
	}
	if (!Q_stricmp(name, "custom"))
	{
		return SABER_CUSTOM;
	}
	if (!Q_stricmp(name, "unstable_red"))
	{
		return SABER_UNSTABLE_RED;
	}
	if (!Q_stricmp(name, "random"))
	{
		return ((saber_colors_t)(Q_irand(SABER_ORANGE, SABER_PURPLE)));
	}
	float colors[3];
	Q_parseSaberColor(name, colors);
	int colourArray[3];
	for (int i = 0; i < 3; i++)
	{
		colourArray[i] = (int)(colors[i] * 255);
	}
	return (saber_colors_t)((colourArray[0]) + (colourArray[1] << 8) + (colourArray[2] << 16) + (1 << 24));
}

saberType_t TranslateSaberType(const char* name)
{
	if (!Q_stricmp(name, "SABER_SINGLE"))
	{
		return SABER_SINGLE;
	}
	if (!Q_stricmp(name, "SABER_STAFF"))
	{
		return SABER_STAFF;
	}
	if (!Q_stricmp(name, "SABER_BROAD"))
	{
		return SABER_BROAD;
	}
	if (!Q_stricmp(name, "SABER_PRONG"))
	{
		return SABER_PRONG;
	}
	if (!Q_stricmp(name, "SABER_DAGGER"))
	{
		return SABER_DAGGER;
	}
	if (!Q_stricmp(name, "SABER_ARC"))
	{
		return SABER_ARC;
	}
	if (!Q_stricmp(name, "SABER_SAI"))
	{
		return SABER_SAI;
	}
	if (!Q_stricmp(name, "SABER_CLAW"))
	{
		return SABER_CLAW;
	}
	if (!Q_stricmp(name, "SABER_LANCE"))
	{
		return SABER_LANCE;
	}
	if (!Q_stricmp(name, "SABER_STAR"))
	{
		return SABER_STAR;
	}
	if (!Q_stricmp(name, "SABER_TRIDENT"))
	{
		return SABER_TRIDENT;
	}
	if (!Q_stricmp(name, "SABER_SITH_SWORD"))
	{
		return SABER_SITH_SWORD;
	}
	if (!Q_stricmp(name, "SABER_UNSTABLE"))
	{
		return SABER_UNSTABLE;
	}
	if (!Q_stricmp(name, "SABER_STAFF_UNSTABLE"))
	{
		return SABER_STAFF_UNSTABLE;
	}
	if (!Q_stricmp(name, "SABER_THIN"))
	{
		return SABER_THIN;
	}
	if (!Q_stricmp(name, "SABER_STAFF_THIN"))
	{
		return SABER_STAFF_THIN;
	}
	if (!Q_stricmp(name, "SABER_SFX"))
	{
		return SABER_SFX;
	}
	if (!Q_stricmp(name, "SABER_STAFF_SFX"))
	{
		return SABER_STAFF_SFX;
	}
	if (!Q_stricmp(name, "SABER_CUSTOMSFX"))
	{
		return SABER_CUSTOMSFX;
	}
	if (!Q_stricmp(name, "SABER_GRIE"))
	{
		return SABER_GRIE;
	}
	return SABER_SINGLE;
}

void UI_SaberDrawBlade(itemDef_t* item, char* saberName, int saberModel, saberType_t saberType, vec3_t origin, float curYaw, int bladeNum)
{
	char bladeColorString[MAX_QPATH];
	vec3_t	angles = { 0 };
	int whichSaber = 0;

	if (item->flags & (ITF_ISANYSABER) && item->flags & (ITF_ISCHARACTER))
	{	//it's bolted to a dude!
		angles[YAW] = curYaw;
	}
	else
	{
		angles[PITCH] = curYaw;
		angles[ROLL] = 90;
	}

	if (saberModel >= item->ghoul2.size())
	{//uhh... invalid index!
		return;
	}

	if ((item->flags & ITF_ISSABER) && saberModel < 2)
	{
		whichSaber = 0;
	}
	else//if ( item->flags&ITF_ISSABER2 ) - presumed
	{
		whichSaber = 1;
	}

	if (whichSaber == 0)
	{
		DC->getCVarString("ui_saber_color", bladeColorString, sizeof(bladeColorString));
	}
	else//if ( whichSaber == 1 ) - presumed
	{
		DC->getCVarString("ui_saber2_color", bladeColorString, sizeof(bladeColorString));
	}
	saber_colors_t bladeColor = TranslateSaberColor(bladeColorString);

	float bladeLength = UI_SaberBladeLengthForSaber(saberName, bladeNum);
	float bladeRadius = UI_SaberBladeRadiusForSaber(saberName, bladeNum);
	vec3_t	bladeOrigin = { 0 };
	vec3_t	axis[3] = {};
	mdxaBone_t	boltMatrix;
	qboolean tagHack = qfalse;

	char* tagName = va("*blade%d", bladeNum + 1);
	int bolt = DC->g2_AddBolt(&item->ghoul2[saberModel], tagName);

	if (bolt == -1)
	{
		tagHack = qtrue;
		//hmm, just fall back to the most basic tag (this will also make it work with pre-JKA saber models
		bolt = DC->g2_AddBolt(&item->ghoul2[saberModel], "*flash");
		if (bolt == -1)
		{//no tag_flash either?!!
			bolt = 0;
		}
	}

	DC->g2_GetBoltMatrix(item->ghoul2, saberModel, bolt, &boltMatrix, angles, origin, uiInfo.uiDC.realTime, NULL, vec3_origin);//NULL was cgs.model_draw

	// work the matrix axis stuff into the original axis and origins used.
	DC->g2_GiveMeVectorFromMatrix(boltMatrix, ORIGIN, bladeOrigin);
	DC->g2_GiveMeVectorFromMatrix(boltMatrix, NEGATIVE_X, axis[0]);//front (was NEGATIVE_Y, but the md3->glm exporter screws up this tag somethin' awful)
	DC->g2_GiveMeVectorFromMatrix(boltMatrix, NEGATIVE_Y, axis[1]);//right
	DC->g2_GiveMeVectorFromMatrix(boltMatrix, POSITIVE_Z, axis[2]);//up

	float scale = DC->xscale;

	if (tagHack)
	{
		switch (saberType)
		{
		case SABER_SINGLE:
		case SABER_DAGGER:
		case SABER_LANCE:
		case SABER_GRIE:
		case SABER_UNSTABLE:
		case SABER_THIN:
		case SABER_SFX:
		case SABER_CUSTOMSFX:
			break;
		case SABER_STAFF:
		case SABER_STAFF_UNSTABLE:
		case SABER_STAFF_THIN:
		case SABER_STAFF_SFX:
			if (bladeNum == 1)
			{
				VectorScale(axis[0], -1, axis[0]);
				VectorMA(bladeOrigin, 16 * scale, axis[0], bladeOrigin);
			}
			break;
		case SABER_BROAD:
			if (bladeNum == 0)
			{
				VectorMA(bladeOrigin, -1 * scale, axis[1], bladeOrigin);
			}
			else if (bladeNum == 1)
			{
				VectorMA(bladeOrigin, 1 * scale, axis[1], bladeOrigin);
			}
			break;
		case SABER_PRONG:
			if (bladeNum == 0)
			{
				VectorMA(bladeOrigin, -3 * scale, axis[1], bladeOrigin);
			}
			else if (bladeNum == 1)
			{
				VectorMA(bladeOrigin, 3 * scale, axis[1], bladeOrigin);
			}
			break;
		case SABER_ARC:
			VectorSubtract(axis[1], axis[2], axis[1]);
			VectorNormalize(axis[1]);
			switch (bladeNum)
			{
			case 0:
				VectorMA(bladeOrigin, 8 * scale, axis[0], bladeOrigin);
				VectorScale(axis[0], 0.75f, axis[0]);
				VectorScale(axis[1], 0.25f, axis[1]);
				VectorAdd(axis[0], axis[1], axis[0]);
				break;
			case 1:
				VectorScale(axis[0], 0.25f, axis[0]);
				VectorScale(axis[1], 0.75f, axis[1]);
				VectorAdd(axis[0], axis[1], axis[0]);
				break;
			case 2:
				VectorMA(bladeOrigin, -8 * scale, axis[0], bladeOrigin);
				VectorScale(axis[0], -0.25f, axis[0]);
				VectorScale(axis[1], 0.75f, axis[1]);
				VectorAdd(axis[0], axis[1], axis[0]);
				break;
			case 3:
				VectorMA(bladeOrigin, -16 * scale, axis[0], bladeOrigin);
				VectorScale(axis[0], -0.75f, axis[0]);
				VectorScale(axis[1], 0.25f, axis[1]);
				VectorAdd(axis[0], axis[1], axis[0]);
				break;
			}
			break;
		case SABER_SAI:
			if (bladeNum == 1)
			{
				VectorMA(bladeOrigin, -3 * scale, axis[1], bladeOrigin);
			}
			else if (bladeNum == 2)
			{
				VectorMA(bladeOrigin, 3 * scale, axis[1], bladeOrigin);
			}
			break;
		case SABER_CLAW:
			switch (bladeNum)
			{
			case 0:
				VectorMA(bladeOrigin, 2 * scale, axis[0], bladeOrigin);
				VectorMA(bladeOrigin, 2 * scale, axis[2], bladeOrigin);
				break;
			case 1:
				VectorMA(bladeOrigin, 2 * scale, axis[0], bladeOrigin);
				VectorMA(bladeOrigin, 2 * scale, axis[2], bladeOrigin);
				VectorMA(bladeOrigin, 2 * scale, axis[1], bladeOrigin);
				break;
			case 2:
				VectorMA(bladeOrigin, 2 * scale, axis[0], bladeOrigin);
				VectorMA(bladeOrigin, 2 * scale, axis[2], bladeOrigin);
				VectorMA(bladeOrigin, -2 * scale, axis[1], bladeOrigin);
				break;
			}
			break;
		case SABER_STAR:
			switch (bladeNum)
			{
			case 0:
				VectorMA(bladeOrigin, 8 * scale, axis[0], bladeOrigin);
				break;
			case 1:
				VectorScale(axis[0], 0.33f, axis[0]);
				VectorScale(axis[2], 0.67f, axis[2]);
				VectorAdd(axis[0], axis[2], axis[0]);
				VectorMA(bladeOrigin, 8 * scale, axis[0], bladeOrigin);
				break;
			case 2:
				VectorScale(axis[0], -0.33f, axis[0]);
				VectorScale(axis[2], 0.67f, axis[2]);
				VectorAdd(axis[0], axis[2], axis[0]);
				VectorMA(bladeOrigin, 8 * scale, axis[0], bladeOrigin);
				break;
			case 3:
				VectorScale(axis[0], -1, axis[0]);
				VectorMA(bladeOrigin, 8 * scale, axis[0], bladeOrigin);
				break;
			case 4:
				VectorScale(axis[0], -0.33f, axis[0]);
				VectorScale(axis[2], -0.67f, axis[2]);
				VectorAdd(axis[0], axis[2], axis[0]);
				VectorMA(bladeOrigin, 8 * scale, axis[0], bladeOrigin);
				break;
			case 5:
				VectorScale(axis[0], 0.33f, axis[0]);
				VectorScale(axis[2], -0.67f, axis[2]);
				VectorAdd(axis[0], axis[2], axis[0]);
				VectorMA(bladeOrigin, 8 * scale, axis[0], bladeOrigin);
				break;
			}
			break;
		case SABER_TRIDENT:
			switch (bladeNum)
			{
			case 0:
				VectorMA(bladeOrigin, 24 * scale, axis[0], bladeOrigin);
				break;
			case 1:
				VectorMA(bladeOrigin, -6 * scale, axis[1], bladeOrigin);
				VectorMA(bladeOrigin, 24 * scale, axis[0], bladeOrigin);
				break;
			case 2:
				VectorMA(bladeOrigin, 6 * scale, axis[1], bladeOrigin);
				VectorMA(bladeOrigin, 24 * scale, axis[0], bladeOrigin);
				break;
			case 3:
				VectorMA(bladeOrigin, -32 * scale, axis[0], bladeOrigin);
				VectorScale(axis[0], -1, axis[0]);
				break;
			}
			break;
		case SABER_SITH_SWORD:
			//no blade
			break;
		default:
			break;
		}
	}
	if (saberType == SABER_SITH_SWORD)
	{//draw no blade
		return;
	}

	if (ui_SFXSabers.integer < 1)
	{// Draw the Raven blade.
		if (saberType == SABER_UNSTABLE || saberType == SABER_STAFF_UNSTABLE)
		{
			UI_DoSaberUnstable(bladeOrigin, axis[0], bladeLength, bladeRadius, bladeColor, whichSaber);
		}
		else
		{
			UI_DoSaber(bladeOrigin, axis[0], bladeLength, bladeLength, bladeRadius, bladeColor, whichSaber);
		}
	}
	else if (!(saberType == SABER_SITH_SWORD))
	{
		switch (ui_SFXSabers.integer)
		{
		case 1:
			if (saberType == SABER_UNSTABLE || saberType == SABER_STAFF_UNSTABLE)
			{
				UI_DoUnstableSaber(bladeOrigin, axis[0], bladeLength, bladeRadius, bladeColor, whichSaber);
			}
			else
			{
				UI_DoEp1Saber(bladeOrigin, axis[0], bladeLength, bladeRadius, bladeColor, SABER_CRYSTAL_DEFAULT, whichSaber);
			}
			break;
		case 2:
			if (saberType == SABER_UNSTABLE || saberType == SABER_STAFF_UNSTABLE)
			{
				UI_DoUnstableSaber(bladeOrigin, axis[0], bladeLength, bladeRadius, bladeColor, whichSaber);
			}
			else
			{
				UI_DoEp2Saber(bladeOrigin, axis[0], bladeLength, bladeRadius, bladeColor, SABER_CRYSTAL_DEFAULT, whichSaber);
			}
			break;
		case 3:
			if (saberType == SABER_UNSTABLE || saberType == SABER_STAFF_UNSTABLE)
			{
				UI_DoUnstableSaber(bladeOrigin, axis[0], bladeLength, bladeRadius, bladeColor, whichSaber);
			}
			else
			{
				UI_DoEp3Saber(bladeOrigin, axis[0], bladeLength, bladeRadius, bladeColor, SABER_CRYSTAL_DEFAULT, whichSaber);
			}
			break;
		case 4:
			if (saberType == SABER_UNSTABLE || saberType == SABER_STAFF_UNSTABLE)
			{
				UI_DoUnstableSaber(bladeOrigin, axis[0], bladeLength, bladeRadius, bladeColor, whichSaber);
			}
			else
			{
				UI_DoSFXSaber(bladeOrigin, axis[0], bladeLength, bladeRadius, bladeColor, SABER_CRYSTAL_DEFAULT, whichSaber);
			}
			break;
		case 5:
			if (saberType == SABER_UNSTABLE || saberType == SABER_STAFF_UNSTABLE)
			{
				UI_DoUnstableSaber(bladeOrigin, axis[0], bladeLength, bladeRadius, bladeColor, whichSaber);
			}
			else
			{
				UI_DoOTSaber(bladeOrigin, axis[0], bladeLength, bladeRadius, bladeColor, SABER_CRYSTAL_DEFAULT, whichSaber);
			}
			break;
		case 6:
			if (saberType == SABER_UNSTABLE || saberType == SABER_STAFF_UNSTABLE)
			{
				UI_DoUnstableSaber(bladeOrigin, axis[0], bladeLength, bladeRadius, bladeColor, whichSaber);
			}
			else
			{
				UI_DoRotJSaber(bladeOrigin, axis[0], bladeLength, bladeRadius, bladeColor, SABER_CRYSTAL_DEFAULT, whichSaber);
			}
			break;
		case 7:
			if (saberType == SABER_UNSTABLE || saberType == SABER_STAFF_UNSTABLE)
			{
				UI_DoUnstableSaber(bladeOrigin, axis[0], bladeLength, bladeRadius, bladeColor, whichSaber);
			}
			else
			{
				UI_DoTFASaber(bladeOrigin, axis[0], bladeLength, bladeRadius, bladeColor, SABER_CRYSTAL_DEFAULT, whichSaber);
			}
			break;
		case 8:
			if (saberType == SABER_UNSTABLE || saberType == SABER_STAFF_UNSTABLE)
			{
				UI_DoUnstableSaber(bladeOrigin, axis[0], bladeLength, bladeRadius, bladeColor, whichSaber);
			}
			else
			{
				UI_DoUnstableSaber(bladeOrigin, axis[0], bladeLength, bladeRadius, bladeColor, whichSaber);
			}
			break;
		}
	}
}

extern qboolean ItemParse_asset_model_go(itemDef_t* item, const char* name);
extern qboolean ItemParse_model_g2skin_go(itemDef_t* item, const char* skinName);
void UI_GetSaberForMenu(char* saber, int saberNum)
{
	char saberTypeString[MAX_QPATH] = { 0 };
	saberType_t saberType = SABER_NONE;

	if (saberNum == 0)
	{
		DC->getCVarString("g_saber", saber, MAX_QPATH);
	}
	else
	{
		DC->getCVarString("g_saber2", saber, MAX_QPATH);
	}
	//read this from the sabers.cfg
	UI_SaberTypeForSaber(saber, saberTypeString);
	if (saberTypeString[0])
	{
		saberType = TranslateSaberType(saberTypeString);
	}

	switch (uiInfo.movesTitleIndex)
	{
	case 0://MD_ACROBATICS:
		break;
	case 1://MD_SINGLE_FAST:
	case 2://MD_SINGLE_MEDIUM:
	case 3://MD_SINGLE_STRONG:
		if (saberType != SABER_SINGLE)
		{
			Q_strncpyz(saber, "single_1", MAX_QPATH);
		}
		break;
	case 4://MD_DUAL_SABERS:
		if (saberType != SABER_SINGLE)
		{
			Q_strncpyz(saber, "single_1", MAX_QPATH);
		}
		break;
	case 5://MD_SABER_STAFF:
		if (saberType == SABER_SINGLE || saberType == SABER_NONE)
		{
			Q_strncpyz(saber, "dual_1", MAX_QPATH);
		}
		break;
	}

}

void UI_SaberDrawBlades(itemDef_t* item, vec3_t origin, float curYaw)
{
	//NOTE: only allows one saber type in view at a time
	char saber[MAX_QPATH];
	int saberNum = 0;
	int saberModel = 0;
	int	numSabers = 1;

	if ((item->flags & ITF_ISCHARACTER)//hacked sabermoves sabers in character's hand
		&& uiInfo.movesTitleIndex == 4 /*MD_DUAL_SABERS*/)
	{
		numSabers = 2;
	}

	for (saberNum = 0; saberNum < numSabers; saberNum++)
	{
		if ((item->flags & ITF_ISCHARACTER))//hacked sabermoves sabers in character's hand
		{
			UI_GetSaberForMenu(saber, saberNum);
			saberModel = saberNum + 1;
		}
		else if ((item->flags & ITF_ISSABER))
		{
			DC->getCVarString("ui_saber", saber, sizeof(saber));
			saberModel = 0;
		}
		else if ((item->flags & ITF_ISSABER2))
		{
			DC->getCVarString("ui_saber2", saber, sizeof(saber));
			saberModel = 0;
		}
		else
		{
			return;
		}
		if (saber[0])
		{
			int numBlades = UI_SaberNumBladesForSaber(saber);
			if (numBlades)
			{//okay, here we go, time to draw each blade...
				char	saberTypeString[MAX_QPATH] = { 0 };
				UI_SaberTypeForSaber(saber, saberTypeString);
				saberType_t saberType = TranslateSaberType(saberTypeString);
				for (int curBlade = 0; curBlade < numBlades; curBlade++)
				{
					if (UI_SaberShouldDrawBlade(saber, curBlade))
					{
						UI_SaberDrawBlade(item, saber, saberModel, saberType, origin, curYaw, curBlade);
					}
				}
			}
		}
	}
}

void UI_SaberAttachToChar(itemDef_t* item)
{
	int	numSabers = 1;
	int	saberNum = 0;

	if (item->ghoul2.size() > 2 && item->ghoul2[2].mModelindex >= 0)
	{//remove any extra models
		DC->g2_RemoveGhoul2Model(item->ghoul2, 2);
	}
	if (item->ghoul2.size() > 1 && item->ghoul2[1].mModelindex >= 0)
	{//remove any extra models
		DC->g2_RemoveGhoul2Model(item->ghoul2, 1);
	}

	if (uiInfo.movesTitleIndex == 4 /*MD_DUAL_SABERS*/)
	{
		numSabers = 2;
	}

	for (saberNum = 0; saberNum < numSabers; saberNum++)
	{
		//bolt sabers
		char modelPath[MAX_QPATH];
		char skinPath[MAX_QPATH];
		char saber[MAX_QPATH];

		UI_GetSaberForMenu(saber, saberNum);

		if (UI_SaberModelForSaber(saber, modelPath))
		{//successfully found a model
			int g2Saber = DC->g2_InitGhoul2Model(item->ghoul2, modelPath, 0, 0, 0, 0, 0); //add the model
			if (g2Saber)
			{
				//get the customSkin, if any
				if (UI_SaberSkinForSaber(saber, skinPath))
				{
					int g2skin = DC->registerSkin(skinPath);
					DC->g2_SetSkin(&item->ghoul2[g2Saber], 0, g2skin);//this is going to set the surfs on/off matching the skin file
				}
				else
				{
					DC->g2_SetSkin(&item->ghoul2[g2Saber], -1, 0);//turn off custom skin
				}
				int boltNum;
				if (saberNum == 0)
				{
					boltNum = DC->g2_AddBolt(&item->ghoul2[0], "*r_hand");
				}
				else
				{
					boltNum = DC->g2_AddBolt(&item->ghoul2[0], "*l_hand");
				}
				re.G2API_AttachG2Model(&item->ghoul2[g2Saber], &item->ghoul2[0], boltNum, 0);
			}
		}
	}
}
