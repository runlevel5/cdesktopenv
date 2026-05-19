/*
 * CDE - Common Desktop Environment
 *
 * Copyright (c) 1993-2012, The Open Group. All rights reserved.
 *
 * These libraries and programs are free software; you can
 * redistribute them and/or modify them under the terms of the GNU
 * Lesser General Public License as published by the Free Software
 * Foundation; either version 2 of the License, or (at your option)
 * any later version.
 *
 * These libraries and programs are distributed in the hope that
 * they will be useful, but WITHOUT ANY WARRANTY; without even the
 * implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 * PURPOSE. See the GNU Lesser General Public License for more
 * details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with these libraries and programs; if not, write
 * to the Free Software Foundation, Inc., 51 Franklin Street, Fifth
 * Floor, Boston, MA 02110-1301 USA
 */
/*
 * $XConsortium: TermFunction.h /main/1 1996/04/21 19:15:50 drk $";
 */

/*                                                                      *
 * (c) Copyright 1993, 1994 Hewlett-Packard Company                     *
 * (c) Copyright 1993, 1994 International Business Machines Corp.       *
 * (c) Copyright 1993, 1994 Sun Microsystems, Inc.                      *
 * (c) Copyright 1993, 1994 Novell, Inc.                                *
 */

#ifndef	_Dt_TermFunction_h
#define	_Dt_TermFunction_h

#include "TermPrimFunction.h"
#include "TermPrimBuffer.h"	/* enhValue */

extern void _DtTermFuncScroll(Widget w, int count,
	FunctionSource functionSource);
extern void _DtTermFuncBeginningOfBuffer(Widget w, int count,
	FunctionSource functionSource);
extern void _DtTermFuncEndOfBuffer(Widget w, int count,
	FunctionSource functionSource);

extern void _DtTermFuncEraseInLine(Widget w, int count,
	FunctionSource functionSource);
extern void _DtTermFuncEraseInDisplay(Widget w, int count,
	FunctionSource functionSource);
extern void _DtTermFuncEraseCharacter(Widget w, int count,
	FunctionSource functionSource);

extern void _DtTermFuncClearToEndOfBuffer(Widget w, int count,
	FunctionSource functionSource);
extern void _DtTermFuncClearBuffer(Widget w, int count,
	FunctionSource functionSource);
extern void _DtTermFuncClearLine(Widget w, int count,
	FunctionSource functionSource);
extern void _DtTermFuncClearToEndOfLine(Widget w, int count,
	FunctionSource functionSource);

extern void _DtTermFuncHardReset(Widget w, int count,
	FunctionSource functionSource);
extern void _DtTermFuncSoftReset(Widget w, int count,
	FunctionSource functionSource);

extern void _DtTermFuncDeleteChar(Widget w, int count,
	FunctionSource functionSource);
extern void _DtTermFuncDeleteCharWrap(Widget w, int count,
	FunctionSource functionSource);

extern void _DtTermFuncInsertLine(Widget w, int count,
	FunctionSource functionSource);

extern void _DtTermFuncDeleteLine(Widget w, int count,
	FunctionSource functionSource);

extern void _DtTermFuncTab(Widget w, int count,
	FunctionSource functionSource);

extern void _DtTermVideoEnhancement(Widget w,int value);

/*
** Write a packed enhValue colour to the current fg or bg slot and
** stamp it onto the cell at the cursor.  Used by the SGR parser to
** apply compound 38;5;N / 48;5;N / 38;2;R;G;B / 48;2;R;G;B sequences.
*/
extern void _DtTermSetColor(Widget w, Boolean isBg, enhValue v);

/*
** Write a packed enhValue underline colour and stamp it on the cell at
** the cursor.  Used by the SGR parser for SGR 58;5;N / 58;2;R;G;B / 59.
*/
extern void _DtTermSetUlColor(Widget w, enhValue v);

extern void _DtTermFontEnhancement(Widget w,int value);

extern void _DtTermSetUserKeyLock(Widget w,Boolean lock_state);
extern Boolean _DtTermGetUserKeyLock(Widget w);
extern void _DtTermSetAutoLineFeed(Widget w,Boolean alf_state);
extern Boolean _DtTermGetAutoLineFeed(Widget w);
extern void _DtTermSetCurrentWorkingDirectory(Widget w, char *cwd);
extern char *_DtTermGetCurrentWorkingDirectory(Widget w);

#endif	/* _Dt_TermFunction_h */
/* DON'T ADD ANYTHING AFTER THIS #endif... */

