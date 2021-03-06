/*
 * Copyright (C) 1990-2018 Andreas Kromke, andreas.kromke@gmail.com
 *
 * This program is free software; you can redistribute it or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 3
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

/*
*
* Clipboard-Umsetzung für MagicMacX
*
*/

#include "config.h"
// System-Header
#include <fcntl.h>
#include <Carbon/Carbon.h>
#include <machine/endian.h>
#include <string.h>
// Programm-Header
#include "Globals.h"
#include "Debug.h"
#include "TextConversion.h"
#include "Clipboard.h"
#include "PascalStrings.h"

#define CHSET_ATARI_delta						0x7F
#define CHSET_ATARI_cedille_uppercase			0x80		// Ç
#define CHSET_ATARI_u_umlaut_lowercase			0x81		// ü
#define CHSET_ATARI_e_aigu_lowercase			0x82		// é
#define CHSET_ATARI_a_circonflex_lowercase		0x83		// â
#define CHSET_ATARI_a_umlaut_lowercase			0x84		// ä
#define CHSET_ATARI_a_grave_lowercase			0x85		// à
#define CHSET_ATARI_a_circle_lowercase			0x86		// å
#define CHSET_ATARI_cedille_lowercase			0x87		// ç
#define CHSET_ATARI_e_circonflex_lowercase		0x88		// ê
#define CHSET_ATARI_e_trema_lowercase			0x89		// ë
#define CHSET_ATARI_e_grave_lowercase			0x8A		// è
#define CHSET_ATARI_i_trema_lowercase			0x8B		// ï
#define CHSET_ATARI_i_circonflex_lowercase		0x8C		// î
#define CHSET_ATARI_i_grave_lowercase			0x8D		// ì
#define CHSET_ATARI_a_umlaut_uppercase			0x8E		// Ä
#define CHSET_ATARI_a_hatschek_uppercase		0x8F		// Ǎ
#define CHSET_ATARI_e_aigu_uppercase			0x90		// É
#define CHSET_ATARI_ae_lowercase				0x91		// æ
#define CHSET_ATARI_ae_uppercase				0x92		// Æ
#define CHSET_ATARI_o_circonflex_lowercase		0x93		// ô
#define CHSET_ATARI_o_umlaut_lowercase			0x94		// ö
#define CHSET_ATARI_o_grave_lowercase			0x95		// ò
#define CHSET_ATARI_u_circonflex_lowercase		0x96		// û
#define CHSET_ATARI_u_grave_lowercase			0x97		// ù
#define CHSET_ATARI_y_trema_lowercase			0x98		// ÿ
#define CHSET_ATARI_o_umlaut_uppercase			0x99		// Ö
#define CHSET_ATARI_u_umlaut_uppercase			0x9A		// Ü
#define CHSET_ATARI_cent						0x9B		// ¢
#define CHSET_ATARI_pound						0x9C		// £
#define CHSET_ATARI_yen							0x9D		// ¥
#define CHSET_ATARI_eszett						0x9E		// ß
#define CHSET_ATARI_integral					0x9F		// ⨍
#define CHSET_ATARI_a_aigu_lowercase			0xA0		// á
#define CHSET_ATARI_i_aigu_lowercase			0xA1		// í
#define CHSET_ATARI_o_aigu_lowercase			0xA2		// ó
#define CHSET_ATARI_u_aigu_lowercase			0xA3		// ú
#define CHSET_ATARI_n_tilde_lowercase			0xA4		// ñ
#define CHSET_ATARI_n_tilde_uppercase			0xA5		// Ñ
#define CHSET_ATARI_a_under_lowercase			0xA6		// _a
#define CHSET_ATARI_o_under_lowercase			0xA7		// _o
#define CHSET_ATARI_question_inverted			0xA8		// ¿
#define CHSET_ATARI_angle_upper_left			0xA9		//
#define CHSET_ATARI_angle_upper_right			0xAA		//
#define CHSET_ATARI_1_div_2						0xAB		// ½
#define CHSET_ATARI_1_div_4						0xAC		// ¼
#define CHSET_ATARI_exclamation_inverted		0xAD		// ¡
#define CHSET_ATARI_double_angle_left			0xAE		// «
#define CHSET_ATARI_double_angle_right			0xAF		// »
#define CHSET_ATARI_a_tilde_lowercase			0xB0		// ã
#define CHSET_ATARI_o_tilde_lowercase			0xB1		// õ
#define CHSET_ATARI_o_schraegstrich_uppercase	0xB2		// Ø
#define CHSET_ATARI_o_schraegstrich_lowercase	0xB3		// ø
#define CHSET_ATARI_oe_lowercase				0xB4		// œ
#define CHSET_ATARI_oe_uppercase				0xB5		// Œ
#define CHSET_ATARI_a_grave_uppercase			0xB6		// À
#define CHSET_ATARI_a_tilde_uppercase			0xB7		// Ã
#define CHSET_ATARI_o_tilde_uppercase			0xB8		// Õ
#define CHSET_ATARI_diaeresis					0xB9		// ¨
#define CHSET_ATARI_acute_accent				0xBA		// ´
#define CHSET_ATARI_dagger						0xBB		// †
#define CHSET_ATARI_absatz						0xBC		// ¶
#define CHSET_ATARI_copyright					0xBD		// ©
#define CHSET_ATARI_registered					0xBE		// ®
#define CHSET_ATARI_trademark					0xBF		// ™
#define CHSET_ATARI_ij_lowercase				0xC0		// ĳ
#define CHSET_ATARI_ij_uppercase				0xC1		// Ĳ
// 0xC2 .. 0xDC sind hebräische Buchstaben
#define CHSET_ATARI_paragraph					0xDD		// §
#define CHSET_ATARI_circumflex_accent			0xDE		// ^
#define CHSET_ATARI_infinite					0xDF		// ∞
#define CHSET_ATARI_alpha_lowercase				0xE0		// α
#define CHSET_ATARI_beta_lowercase				0xE1		// β
#define CHSET_ATARI_gamma_uppercase				0xE2		// Γ
#define CHSET_ATARI_pi_lowercase				0xE3		// π
#define CHSET_ATARI_sigma_uppercase				0xE4		// Σ
#define CHSET_ATARI_sigma_lowercase				0xE5		// σ
#define CHSET_ATARI_mue_lowercase				0xE6		// μ
#define CHSET_ATARI_tau_lowercase				0xE7		// τ
#define CHSET_ATARI_error_barred_white_circle	0xE8		// ⧲
#define CHSET_ATARI_theta_uppercase				0xE9		// Θ
#define CHSET_ATARI_omega_uppercase				0xEA		// Ω
#define CHSET_ATARI_delta_lowercase				0xEB		// δ
#define CHSET_ATARI_contour_integral			0xEC		// ∮
#define CHSET_ATARI_phi_uppercase				0xED		// Φ
#define CHSET_ATARI_element						0xEE		// ∈
#define CHSET_ATARI_intersection				0xEF		// ∩
#define CHSET_ATARI_identical					0xF0		// ≡
#define CHSET_ATARI_plusminus					0xF1		// ±
#define CHSET_ATARI_greater_than_or_equal		0xF2		// ≥
#define CHSET_ATARI_smaller_than_or_equal		0xF3		// ≤
#define CHSET_ATARI_top_half_integral			0xF4		// ⌠
#define CHSET_ATARI_bottom_half_integral		0xF5		// ⌡
#define CHSET_ATARI_division					0xF6		// ÷
#define CHSET_ATARI_almost_equal_to				0xF7		// ≈
#define CHSET_ATARI_degree						0xF8		// °
#define CHSET_ATARI_dot_above					0xF9		// ˙
#define CHSET_ATARI_bullet						0xFA		// •
#define CHSET_ATARI_square_root					0xFB		// √
#define CHSET_ATARI_superscript_n				0xFC		// ⁿ
#define CHSET_ATARI_superscript_two				0xFD		// ²
#define CHSET_ATARI_superscript_three			0xFE		// ³
#define CHSET_ATARI_overline					0xFF		//  ̅

const CClipboard::atariCharEntry CClipboard::atariCharConvTable[] =
{
	{ CHSET_ATARI_cedille_uppercase			, u8"Ç" },
	{ CHSET_ATARI_u_umlaut_lowercase		, u8"ü" },
	{ CHSET_ATARI_e_aigu_lowercase			, u8"é" },
	{ CHSET_ATARI_a_circonflex_lowercase	, u8"â" },
	{ CHSET_ATARI_a_umlaut_lowercase		, u8"ä" },
	{ CHSET_ATARI_a_grave_lowercase			, u8"à" },
	{ CHSET_ATARI_a_circle_lowercase		, u8"å" },
	{ CHSET_ATARI_cedille_lowercase			, u8"ç" },
	{ CHSET_ATARI_e_circonflex_lowercase	, u8"ê" },
	{ CHSET_ATARI_e_trema_lowercase			, u8"ë" },
	{ CHSET_ATARI_e_grave_lowercase			, u8"è" },
	{ CHSET_ATARI_i_trema_lowercase			, u8"ï" },
	{ CHSET_ATARI_i_circonflex_lowercase	, u8"î" },
	{ CHSET_ATARI_i_grave_lowercase			, u8"ì" },
	{ CHSET_ATARI_a_umlaut_uppercase		, u8"Ä" },
	{ CHSET_ATARI_a_hatschek_uppercase	  	, u8"Ǎ" },
	{ CHSET_ATARI_e_aigu_uppercase			, u8"É" },
	{ CHSET_ATARI_ae_lowercase				, u8"æ" },
	{ CHSET_ATARI_ae_uppercase				, u8"Æ" },
	{ CHSET_ATARI_o_circonflex_lowercase	, u8"ô" },
	{ CHSET_ATARI_o_umlaut_lowercase		, u8"ö" },
	{ CHSET_ATARI_o_grave_lowercase			, u8"ò" },
	{ CHSET_ATARI_u_circonflex_lowercase	, u8"û" },
	{ CHSET_ATARI_u_grave_lowercase			, u8"ù" },
	{ CHSET_ATARI_y_trema_lowercase			, u8"ÿ" },
	{ CHSET_ATARI_o_umlaut_uppercase		, u8"Ö" },
	{ CHSET_ATARI_u_umlaut_uppercase		, u8"Ü" },
	{ CHSET_ATARI_cent						, u8"¢" },
	{ CHSET_ATARI_pound						, u8"£" },
	{ CHSET_ATARI_yen						, u8"¥" },
	{ CHSET_ATARI_eszett					, u8"ß" },
	{ CHSET_ATARI_integral					, u8"⨍" },
	{ CHSET_ATARI_a_aigu_lowercase			, u8"á" },
	{ CHSET_ATARI_i_aigu_lowercase			, u8"í" },
	{ CHSET_ATARI_o_aigu_lowercase			, u8"ó" },
	{ CHSET_ATARI_u_aigu_lowercase			, u8"ú" },
	{ CHSET_ATARI_n_tilde_lowercase			, u8"ñ" },
	{ CHSET_ATARI_n_tilde_uppercase			, u8"Ñ" },
//	{ CHSET_ATARI_a_under_lowercase			,u8"_a" },
//	{ CHSET_ATARI_o_under_lowercase			,u8"_o" },
	{ CHSET_ATARI_question_inverted			, u8"¿" },
	{ CHSET_ATARI_angle_upper_left			, u8" " },
	{ CHSET_ATARI_angle_upper_right			, u8" " },
	{ CHSET_ATARI_1_div_2					, u8"½" },
	{ CHSET_ATARI_1_div_4					, u8"¼" },
	{ CHSET_ATARI_exclamation_inverted		, u8"¡" },
	{ CHSET_ATARI_double_angle_left			, u8"«" },
	{ CHSET_ATARI_double_angle_right		, u8"»" },
	{ CHSET_ATARI_a_tilde_lowercase			, u8"ã" },
	{ CHSET_ATARI_o_tilde_lowercase			, u8"õ" },
	{ CHSET_ATARI_o_schraegstrich_uppercase	, u8"Ø" },
	{ CHSET_ATARI_o_schraegstrich_lowercase	, u8"ø" },
	{ CHSET_ATARI_oe_lowercase				, u8"œ" },
	{ CHSET_ATARI_oe_uppercase				, u8"Œ" },
	{ CHSET_ATARI_a_grave_uppercase			, u8"À" },
	{ CHSET_ATARI_a_tilde_uppercase			, u8"Ã" },
	{ CHSET_ATARI_o_tilde_uppercase			, u8"Õ" },
	{ CHSET_ATARI_diaeresis					, u8"¨" },
	{ CHSET_ATARI_acute_accent				, u8"´" },
	{ CHSET_ATARI_dagger					, u8"†" },
	{ CHSET_ATARI_absatz					, u8"¶" },
	{ CHSET_ATARI_copyright					, u8"©" },
	{ CHSET_ATARI_registered				, u8"®" },
	{ CHSET_ATARI_trademark					, u8"™" },
	{ CHSET_ATARI_ij_lowercase				, u8"ĳ" },
	{ CHSET_ATARI_ij_uppercase				, u8"Ĳ" },
	{ CHSET_ATARI_paragraph					, u8"§" },
	{ CHSET_ATARI_circumflex_accent			, u8"^" },
	{ CHSET_ATARI_infinite					, u8"∞" },
	{ CHSET_ATARI_alpha_lowercase			, u8"α" },
	{ CHSET_ATARI_beta_lowercase			, u8"β" },
	{ CHSET_ATARI_gamma_uppercase			, u8"Γ" },
	{ CHSET_ATARI_pi_lowercase				, u8"π" },
	{ CHSET_ATARI_sigma_uppercase			, u8"Σ" },
	{ CHSET_ATARI_sigma_lowercase			, u8"σ" },
	{ CHSET_ATARI_mue_lowercase				, u8"μ" },
	{ CHSET_ATARI_tau_lowercase				, u8"τ" },
	{ CHSET_ATARI_error_barred_white_circle	, u8"⧲" },
	{ CHSET_ATARI_theta_uppercase			, u8"Θ" },
	{ CHSET_ATARI_omega_uppercase			, u8"Ω" },
	{ CHSET_ATARI_delta_lowercase			, u8"δ" },
	{ CHSET_ATARI_contour_integral			, u8"∮" },
	{ CHSET_ATARI_phi_uppercase				, u8"Φ" },
	{ CHSET_ATARI_element					, u8"∈" },
	{ CHSET_ATARI_intersection				, u8"∩" },
	{ CHSET_ATARI_identical					, u8"≡" },
	{ CHSET_ATARI_plusminus					, u8"±" },
	{ CHSET_ATARI_greater_than_or_equal		, u8"≥" },
	{ CHSET_ATARI_smaller_than_or_equal		, u8"≤" },
	{ CHSET_ATARI_top_half_integral			, u8"⌠" },
	{ CHSET_ATARI_bottom_half_integral		, u8"⌡" },
	{ CHSET_ATARI_division					, u8"÷" },
	{ CHSET_ATARI_almost_equal_to			, u8"≈" },
	{ CHSET_ATARI_degree					, u8"°" },
	{ CHSET_ATARI_dot_above					, u8"˙" },
	{ CHSET_ATARI_bullet					, u8"•" },
	{ CHSET_ATARI_square_root				, u8"√" },
	{ CHSET_ATARI_superscript_n				, u8"ⁿ" },
	{ CHSET_ATARI_superscript_two			, u8"²" },
	{ CHSET_ATARI_superscript_three			, u8"³" },
	{ CHSET_ATARI_overline					, u8" ̅" }
};

// statische Attribute:

//unsigned long CClipboard::s_lastCrDat = 0;
//unsigned long CClipboard::s_lastMdDat = 0;
//long CClipboard::s_lastFILgLen = 0;
//char CClipboard::s_AtariScrapPath[256] = ":MAGIC_C:GEMSYS:GEMSCRAP:";


/**********************************************************************
 *
 * statisch: Clipboard Mac (UTF-8) -> Atari
 *
 **********************************************************************/

const CClipboard::atariCharEntry *CClipboard::FindUtf8(const uint8_t *utf8)
{
	unsigned nElements = sizeof(atariCharConvTable)/sizeof(atariCharConvTable[0]);

	const atariCharEntry *p = atariCharConvTable;
	while (p < atariCharConvTable + nElements)
	{
		if (!memcmp(utf8, p->utf8Char, strlen(p->utf8Char)))
			return p;
		p++;
	}

	return NULL;
}


/**********************************************************************
 *
 * statisch: Clipboard Mac Atari -> (UTF-8)
 *
 **********************************************************************/

const CClipboard::atariCharEntry *CClipboard::FindAtari(uint8_t c)
{
	unsigned nElements = sizeof(atariCharConvTable)/sizeof(atariCharConvTable[0]);

	const atariCharEntry *p = atariCharConvTable;
	while (p < atariCharConvTable + nElements)
	{
		if (p->atariChar == c)
		{
			return p;
		}
		p++;
	}

	return NULL;
}


/**********************************************************************
*
* statisch: Clipboard Mac (UTF-8) -> Atari
*
**********************************************************************/

void CClipboard::Mac2Atari(const uint8_t *pData)
{
	unsigned char *ScrapBuffer;
	unsigned char *wrPtr;
	const atariCharEntry *p;


#ifdef _DEBUG
	char dbgoutbuf[128];
	strlcpy(dbgoutbuf, (const char *) pData, sizeof(dbgoutbuf));
	DebugInfo("CClipboard::Mac2Atari(%s)", dbgoutbuf);
#endif

	if	(!pData)
		return;		// keine Daten

	// Puffer anfordern, der die doppelte Länge der Ausgangsdaten hat, denn
	// im Extremfall sind nur CRs im Clipboard.
	ScrapBuffer = (unsigned char *) malloc(2 * strlen((const char *) pData) + 1);
	if	(!ScrapBuffer)
		return;

#ifdef _DEBUG_VERBOSE
	unsigned char uc;
	for	(register int i = 0; i < strlen(pData); i++)
	{
		uc = (unsigned char) pData[i];
		CDebug::DebugInfo("CClipboard::Mac2Atari() --- "
					"Byte[%d] = %u (%c)", i, uc, (uc >= 32) ? uc : '?');
	}
#endif

	// Atari-Clipboard-Datei im Unix-Modus zum Schreiben öffnen, ggf. erstellen und nullen.

	int fd = OpenAtariScrapFile(O_WRONLY | O_CREAT | O_TRUNC);
	if	(fd < 0)
	{
		free(ScrapBuffer);
		return;
	}

	// Text zeichenweise von UTF-8 konvertieren

	unsigned char c;

	wrPtr = ScrapBuffer;
	while(*pData)
	{
		c = *pData++;
		if (c > 0x20 && c <= 0x7F)
		{
			// US-ASCII (7 Bit), ohne Steuerzeichen
			*wrPtr++ = c;
		}
		else
		if (c >= 0x80 && c <= 0xBF)
		{
			// zweites oder drittes Byte einer Sequenz. Hier: ungültig
			// ignorieren
		}
		else
		if (c >= 0xC0 && c <= 0xC1)
		{
			// Start einer 2 Byte langen Sequenz, welche den Codebereich aus 0 bis 127 abbildet, unzulässig
			// ignorieren
		}
		else
		if (c >= 0xC2 && c <= 0xDF)
		{
			// Start einer 2 Byte langen Sequenz
			if (pData[0])
			{
				p = FindUtf8(pData - 1);
				if (p)
				{
					// found
					*wrPtr++ = (uint8_t) p->atariChar;
				}
				else
				{
					// not found
					*wrPtr++ = '?';
				}
				
				pData++;	// 2-byte character
			}
		}
		else
		if (c >= 0xE0 && c <= 0xEF)
		{
			// Start einer 3 Byte langen Sequenz
			if (pData[0] && pData[1])
			{
				p = FindUtf8(pData - 1);
				if (p)
				{
					// found
					*wrPtr++ = (uint8_t) p->atariChar;
				}
				else
				{
					// not found
					*wrPtr++ = '?';
				}

				pData += 2;	// 3-byte character
			}
		}
		else
		if (c >= 0xF0 && c <= 0xF7)
		{
			// Start einer 4 Byte langen Sequenz
			if (pData[0] && pData[1] && pData[2])
			{
				p = FindUtf8(pData - 1);
				if (p)
				{
					// found
					*wrPtr++ = (uint8_t) p->atariChar;
				}
				else
				{
					// not found
					*wrPtr++ = '?';
				}
				
				pData += 3;	// 4-byte character
			}
		}
		else
		if (c >= 0xF8 && c <= 0xFB)
		{
			// Start einer 5 Byte langen Sequenz
			if (pData[0] && pData[1] && pData[2] && pData[3])
			{
				p = FindUtf8(pData - 1);
				if (p)
				{
					// found
					*wrPtr++ = (uint8_t) p->atariChar;
				}
				else
				{
					// not found
					*wrPtr++ = '?';
				}
				
				pData += 4;	// 5-byte character
			}
		}
		else
		if (c >= 0xFC && c <= 0xFD)
		{
			// Start einer 6 Byte langen Sequenz
			if (pData[0] && pData[1] && pData[2] && pData[3] && pData[4])
			{
				p = FindUtf8(pData - 1);
				if (p)
				{
					// found
					*wrPtr++ = (uint8_t) p->atariChar;
				}
				else
				{
					// not found
					*wrPtr++ = '?';
				}
				
				pData += 5;	// 6-byte character
			}
		}
		else
		switch (c)
		{
			case '\r':
				*wrPtr++ = c;
				if (*pData == '\n')
				{
					// \r\n -> \r\n
					*wrPtr++ = *pData++;
				}
				else
				{
					// \r\n -> \r\n
					*wrPtr++ = '\n';
				}
				break;
				
			case '\n':
				// \n -> \r\n
				*wrPtr++ = '\r';
				*wrPtr++ = '\n';
				break;
/*
			case 0xEF:
				// BOM ignorieren
				if (pData[0] == 0xBB && pData[1] == 0xBF)
				{
					// BOM
					pData += 2;
				}
				else
				if (pData[0] && pData[1])
				{
					// andere Sequenz
					pData += 2;
				}
 */
		}
	}
	*wrPtr++ = 0;		// abschließendes EOS

	// letzte Zeile schreiben
	SInt32 wrcount = wrPtr - ScrapBuffer;
	(void) write(fd, ScrapBuffer, wrcount);
	(void) close(fd);

	free(ScrapBuffer);
}


/**********************************************************************
*
* statisch: Clipboard Atari -> Mac
*
**********************************************************************/

void CClipboard::Atari2Mac(uint8_t **pBuffer)
{
	uint8_t *inAtariBuffer;
	uint8_t *outUtf8Buffer;
	int fd;
	ssize_t readCnt;


	DebugInfo("CClipboard::Atari2Mac()");
	*pBuffer = NULL;

	// Größe bestimmen per seek-to-end

	fd = OpenAtariScrapFile(O_RDONLY);
	if	(fd < 0)
		return;
	off_t fileLen = lseek(fd, 0, SEEK_END);
	(void) lseek(fd, 0, SEEK_SET);
	if (fileLen == -1)
	{
		close(fd);
		return;
	}

	inAtariBuffer = (unsigned char *) malloc(fileLen);
	if	(!inAtariBuffer)
	{
		DebugError("CClipboard::Atari2Mac() --- Zuwenig Speicher");
		close(fd);
		return;
	}
	readCnt = read(fd, inAtariBuffer, fileLen);
	close(fd);
	if (readCnt == -1)
	{
		free(inAtariBuffer);
		DebugError("CClipboard::Atari2Mac() --- Fehler %d bei read()", errno);
		return;
	}

	// Da ein UTF8-Zeichen im Extremfall 6 Bytes braucht, reservieren wir entsprechend viel Platz

	outUtf8Buffer = (unsigned char *) malloc(fileLen * 6 + 1);
	if	(!outUtf8Buffer)
	{
		DebugError("CClipboard::Atari2Mac() --- Zuwenig Speicher");
		free(inAtariBuffer);
		return;
	}

	// Wir konvertieren die Daten
	
	const uint8_t *rdPtr, *endPtr;
	uint8_t *wrPtr;
	unsigned char c;

	rdPtr = inAtariBuffer;
	endPtr = inAtariBuffer + fileLen;
	wrPtr = outUtf8Buffer;
	while(rdPtr < endPtr)
	{
		c = *rdPtr++;
		if	(c == '\n')
		{
			// Zeilenenden konvertieren 0d0a->0d
		}
		else
		if (c < 0x7F)
		{
			*wrPtr++ = c;	// 7-Bit-ASCII
		}
		else
		{
			const atariCharEntry *p = FindAtari(c);
			if (p)
			{
				memcpy(wrPtr, p->utf8Char, strlen(p->utf8Char));
				wrPtr += strlen(p->utf8Char);
			}
			else
			{
				*wrPtr++ = '?';		// unconvertible character
			}
		}
	}
	*wrPtr = 0;		// end-of-string
	free(inAtariBuffer);
	*pBuffer = outUtf8Buffer;
}


/**********************************************************************
*
* statisch und privat: Atari-Clipboard-Datei löschen
*
**********************************************************************/
/*
void CClipboard::DeleteAtariScrapFile( const char *fname )
{
	OSErr err;
	Str255 path;
 	FSSpec spec;
 
	strcpy((char *) (path+1), s_AtariScrapPath);
	strcat((char *) (path+1), fname);
	C2P(path);
	err = FSMakeFSSpec(0, 0, path, &spec);

	if	((err != 0) && (err != fnfErr))
	{
		DebugError("CClipboard::DeleteClipboardFile() --- Fehler %d bei FSMakeFSSpec()", err);
	}

	if	(!err)
		FSpDelete(&spec);
}
*/

/**********************************************************************
*
* statisch und privat: Atari-Clipboard öffnen mit Unix-Aufrufen
*
**********************************************************************/

int CClipboard::OpenAtariScrapFile(int unixPerm)
{
	int fd;


	fd = open((const char *) CGlobals::s_atariScrapFileUnixPath, unixPerm);

/*
	if	(perm == fsWrPerm)
	{
		// Andere Scrap-Dateien löschen
		DeleteAtariScrapFile("scrap.asc");
		DeleteAtariScrapFile("scrap.img");
		DeleteAtariScrapFile("scrap.gem");

		// Bestehende Datei löschen
		FSpDelete(&spec);
		// Neue Datei anlegen
		err = FSpCreate(&spec, MyCreator, 'TEXT', smSystemScript);
		if	(err)
		{
			DebugError("CClipboard::OpenAtariScrapFile() --- Fehler %d bei FSpCreate()", err);
			return(err);
		}
	}
*/
	return fd;
}


/**********************************************************************
*
* statisch und privat: Atari-Clipboard geändert?
*
**********************************************************************/
/*
OSErr CClipboard::CheckAtariScrapFile( bool *bChanged )
{
	CInfoPBRec pb;
	OSErr err;
	FSSpec spec;


	if	(bChanged)
		*bChanged = false;

	// Wir sind ganz brutal und nehmen an, daß das Atari-Clipboard immer
	// in "MAGIC_C/GEMSYS/GEMSCRAP/SCRAP.TXT" liegt
	err = FSMakeFSSpec(0, 0, "\p:MAGIC_C:GEMSYS:GEMSCRAP:SCRAP.TXT", &spec);
	if	(err)
	{
		DebugError("CClipboard::CheckAtariScrapFile() --- Fehler %d bei FSMakeFSSpec()", err);
		return(err);
	}

	pb.hFileInfo.ioNamePtr = spec.name;
	pb.hFileInfo.ioVRefNum = spec.vRefNum;
	pb.hFileInfo.ioFDirIndex = -1;
	pb.hFileInfo.ioDirID = spec.parID;
	err = PBHGetFInfoSync((HParmBlkPtr)&pb);
	if	(err)
	{
		DebugError("CClipboard::CheckAtariScrapFile() --- Fehler %d bei PBHGetFInfoSync()", err);
		return(err);
	}

	if	((pb.hFileInfo.ioFlCrDat != s_lastCrDat) ||
		 (pb.hFileInfo.ioFlMdDat != s_lastMdDat) ||
		 (pb.hFileInfo.ioFlLgLen != s_lastFILgLen))
	{
		// Hat sich geändert
		s_lastCrDat = pb.hFileInfo.ioFlCrDat;
		s_lastMdDat = pb.hFileInfo.ioFlMdDat;
		s_lastFILgLen = pb.hFileInfo.ioFlLgLen;
		if	(bChanged)
			*bChanged = true;
	}

	return(err);
}
*/