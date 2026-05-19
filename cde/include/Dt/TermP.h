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
 * $XConsortium: TermP.h /main/1 1996/04/21 19:16:07 drk $";
 */

/*                                                                      *
 * (c) Copyright 1993, 1994 Hewlett-Packard Company                     *
 * (c) Copyright 1993, 1994 International Business Machines Corp.       *
 * (c) Copyright 1993, 1994 Sun Microsystems, Inc.                      *
 * (c) Copyright 1993, 1994 Novell, Inc.                                *
 */

#ifndef	_Dt_TermP_h
#define	_Dt_TermP_h

#include "TermPrimP.h"
#include "Term.h"
#include "TermData.h"
#include "TermFunctionKey.h"

#ifdef	__cplusplus
extern "C" {
#endif	/* __cplusplus */

/* Vt class structure... */

typedef struct _DtTermClassPart
{
    int foo;
} DtTermClassPart;

/* full clas record declaration for Vt class... */
typedef struct _DtTermClassRec {
    CoreClassPart		core_class;
    XmPrimitiveClassPart	primitive_class;
    DtTermPrimitiveClassPart		term_primitive_class;
    DtTermClassPart		term_class;
} DtTermClassRec;

externalref DtTermClassRec dtTermClassRec;

/* vt instance record... */
typedef struct _DtTermPart
{
    DtTermData			td;	/* non-widget terminal data	*/
    Boolean                     autoWrap;
    Boolean                     reverseWrap;
    Boolean                     sunFunctionKeys;
    Boolean                     c132;
    Boolean			appKeypadMode;
    Boolean			appCursorMode;
    /*
    ** The 16-entry ANSI colour palette, settable via the *color0..*color15
    ** resources.  Pixels are resolved by Xt's String -> Pixel converter at
    ** widget initialise time from the resource defaults or user overrides.
    ** colour[N] corresponds to xterm palette index N (slots 0..7 dim ANSI,
    ** 8..15 bright); they are copied into td->colorPairs[1..16] in
    ** _DtTermColorInit.
    */
    Pixel			color0;
    Pixel			color1;
    Pixel			color2;
    Pixel			color3;
    Pixel			color4;
    Pixel			color5;
    Pixel			color6;
    Pixel			color7;
    Pixel			color8;
    Pixel			color9;
    Pixel			color10;
    Pixel			color11;
    Pixel			color12;
    Pixel			color13;
    Pixel			color14;
    Pixel			color15;
    /*
    ** xterm convention: SGR 1 (bold) promotes a 30..37 fg to its 90..97
    ** bright twin.  Set False to render bold attributes without the colour
    ** brightening (i.e. only via the bold font and / or overstrike).
    */
    Boolean			boldColors;

    /*
    ** OSC 52 clipboard control.  Defaults to False so a hostile program
    ** can't silently slurp or replace the user's clipboard.  Set True via
    ** Dtterm*allowClipboardOps if you want OSC 52 set / paste handling.
    */
    Boolean			allowClipboardOps;
} DtTermPart;

/* full instance record declaration... */

typedef struct _DtTermRec {
    CorePart		core;
    XmPrimitivePart	primitive;
    DtTermPrimitivePart	term;
    DtTermPart		vt;
} DtTermRec;

/* private function declarations... */
/* end private function declarations... */

#ifdef	__cplusplus
} /* close scope of 'extern "C"'... */
#endif	/* __cplusplus */

#endif	/* _Dt_TermP_h */
/* DON'T ADD ANYTHING AFTER THIS #endif... */
