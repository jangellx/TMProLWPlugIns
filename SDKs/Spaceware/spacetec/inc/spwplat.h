/*----------------------------------------------------------------------
 *  spwplat.h -- Defines target platform names from cpp predefines.
 *
 *  $Id: spwplat.h,v 1.2 1996/09/17 19:05:58 prichard Exp $
 *
 *  This needs more work, I'm not quite sure how it should work now...
 *
 *----------------------------------------------------------------------
 *
 *  (C) 1996 Spacetec IMC Corporation.  All rights reserved.
 *
 *  The computer codes included in this file, including source code and
 *  object code, constitutes the proprietary and confidential information of
 *  Spacetec IMC Corporation, and are provided pursuant to a license
 *  agreement.  These computer codes are protected by international, federal
 *  and state law, including United States Copyright Law and international
 *  treaty provisions.  Except as expressly authorized by the license
 *  agreement, or as expressly permitted under applicable laws of member
 *  states of the European Union and then only to the extent so permitted,
 *  no part of these computer codes may be reproduced or transmitted in any
 *  form or by any means, electronic or mechanical, modified, decompiled,
 *  disassembled, reverse engineered, sold, transferred, rented or utilized
 *  for any unauthorized purpose without the express written permission of
 *  Spacetec IMC Corporation.
 *
 */

#ifndef SPWPLAT_H
#define SPWPLAT_H

static char spwplatCvsId[]="(C) 1996 Spacetec IMC Corporation: $Id: spwplat.h,v 1.2 1996/09/17 19:05:58 prichard Exp $";

/* Check for any BSD/SystemV variant */
#if defined(sun_sunos) || defined(dec_ultrix)
#   define BSD
#else
#   define SystemV
#endif



/* SUN SOLARIS (SunOS version => 5.0) */
#if defined(sun_solaris)

/*  reaffirm the agreed upon predefines */
#   undef sun
#   define sun
#endif



/* SUN SunOS BEFORE SOLARIS (SunOS version < 5.0)*/
#if defined(sun_sunos)

/*  reaffirm the agreed upon predefines */
#   undef sun
#   define sun
#endif



/* SGI Irix 4 */
#if defined(sgi_irix_4)

/*  reaffirm the agreed upon predefines */
#   undef   sgi
#   define  sgi
#endif



/* SGI Irix 5  */
#if defined(sgi_irix_5)

/*  reaffirm the agreed upon predefines */
#   undef   sgi
#   define  sgi
#endif



/* DEC PRE-ALPHA (assume MIPS (may not matter)) ULTRIX WORKSTATIONS */
#if defined(dec_ultrix)
#   define dec
#   define POSIX

/*  reaffirm the agreed upon predefines */
#endif



/* DEC ALPHA OSF WORKSTATIONS */
#if defined(dec_alpha_osf)
#   define dec
#   define POSIX

/*  reaffirm the agreed upon predefines */
#endif


/* DEC VAX VMS WORKSTATIONS */
#if defined(dec_vax_vms)
#   define dec

/*  reaffirm the agreed upon predefines */
#   undef  vms
#   define vms
#   undef  VMS
#   define VMS
#endif


/* DEC ALPHA VMS WORKSTATIONS */
#if defined(dec_alpha_vms)
#   define dec

/*  reaffirm the agreed upon predefines */
#   undef  vms
#   define vms
#   undef  VMS
#   define VMS
#endif




/* IBM RS/6000 (assume AIX versions >= 3.2.1) */
#if defined(ibm_rs6000)

/*  reaffirm the agreed upon predefines */
#   undef POSIX
#   define POSIX
#endif



/* IBM PC (MS_DOS) */
#if defined(ibm_pc_dos)
#   ifndef ibm_pc
#   define ibm_pc
#   endif

#endif



/* IBM PC (MS_DOS) WINDOWS 3.1 */
#if defined(ibm_pc_dos_win_31)
#   ifndef ibm_pc
#   define ibm_pc
#   endif

/*  reaffirm the agreed upon predefines */
#   undef WINDOWS
#   define WINDOWS

#endif



/* IBM PC (NT) WINDOWS 3.1 */
#if defined(ibm_pc_nt_win_31)
#   ifndef ibm_pc
#   define ibm_pc
#   endif

/*  reaffirm the agreed upon predefines */
#   undef WINNT
#   define WINNT

/*  reaffirm the agreed upon predefines */
#   undef WINDOWS
#   define WINDOWS

#endif


/* IBM PC (NT) WINDOWS */
#if defined(ibm_pc_winnt)
#   ifndef ibm_pc
#   define ibm_pc
#   endif

/*  reaffirm the agreed upon predefines */
#   undef WINNT
#   define WINNT

/*  reaffirm the agreed upon predefines */
#   undef WINDOWS
#   define WINDOWS

/*  reaffirm the agreed upon predefines */
#   undef MSDOS
#   define MSDOS

#endif


#if defined(ALLIANT)
/*  reaffirm the agreed upon predefines */
#   undef ALLIANT
#   define ALLIANT
#endif


#endif
