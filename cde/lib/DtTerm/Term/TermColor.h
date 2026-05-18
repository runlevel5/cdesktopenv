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
 * $XConsortium: TermColor.h /main/1 1996/04/21 19:15:35 drk $";
 */

/*                                                                      *
 * (c) Copyright 1993, 1994 Hewlett-Packard Company                     *
 * (c) Copyright 1993, 1994 International Business Machines Corp.       *
 * (c) Copyright 1993, 1994 Sun Microsystems, Inc.                      *
 * (c) Copyright 1993, 1994 Novell, Inc.                                *
 */

#ifndef	_Dt_TermColor_h
#define	_Dt_TermColor_h

void _DtTermColorInit(Widget w);
void _DtTermColorDestroy(Widget w);
void _DtTermColorInitializeColorPair(Widget w, VtColorPair colorPair);

/*
** Resolve an xterm 256-colour index (16..255) to a Pixel.  Allocates
** via XAllocColor on first use and caches the result on the widget's
** DtTermData.  Returns the widget's default fg Pixel on allocation
** failure or an out-of-range index.
*/
Pixel _DtTermResolve256Pixel(Widget w, unsigned int xcol);

#endif	/* _Dt_TermColor_h */
/* DON'T ADD ANYTHING AFTER THIS #endif... */
