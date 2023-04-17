/*
+------------------------------------------------------------------+
| Copyright (C) 2023 Roger P. Johnson, roger0080@netscape.net      |
|                                                                  |
| This file is part of libbst.                                     |
|                                                                  |
| libbst is free software: you can redistribute it and/or modify   |
| it under the terms of the GNU Lessor General Public License as   |
| published by the Free Software Foundation, either version 3 of   |
| the License, or (at your option) any later version.              |
|                                                                  |
| libbst is distributed in the hope that it will be useful, but    |
| WITHOUT ANY WARRANTY; without even the implied warranty of       |
| MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the     |
| GNU Lessor General Public License for more details.              |
|                                                                  |
| You should have received a copy of the GNU Lessor General Public |
| License along with libbst. If not, see www.gnu.org/licenses.     |
+------------------------------------------------------------------+
*/

#include <stdlib.h>

#ifdef SVR3
#include <time.h>
#endif

#ifdef SVR4
#include <sys/time.h>
#endif

/* NOTE: had to include this for FreeBSD */
#ifdef BSD
#include <time.h>
#endif

static char *RCSid[] = { "$Id: tid.c,v 2.1 1999/01/02 17:07:51 roger Exp $" };


/* tid: create a unique tree id to identify it uniquely (from a time before uuid's) */
double tid(void)
{
 /*******************************************************************************
  *  An internal library function that creates the unique tree id.
  *
  *  Input Parameters
  *  =================
  *  None.
  *
  *  Output Parameters
  *  =================
  *  Function name returns double result:
  *
  *  Global Variables
  *  =================
  *  None.
  *******************************************************************************/

    time_t t;			/* time in seconds */
    double msec;		/* microseconds */


    /*return ((double) rand()); *//* use random number */

#ifdef BSD
    /* where is this code ? */
#endif

#ifdef SVR3
    static time_t rseed = 0;	/* random number seed */
    t = time(&t);		/* get time in seconds */
    srand(rseed++);		/* use a new seed */
    msec = (double) rand();	/* use random number for the decimal part */
#endif
#ifdef SVR4
    struct timeval tp;		/* pointer to time structure */
    struct timezone tz;		/* pointer to time zone structure */
    gettimeofday(&tp, &tz);	/* get time structures */
    t = tp.tv_sec;		/* t is time in seconds */
    msec = (double) tp.tv_usec;	/* use microseconds for the decimal part */
#endif

    while ((msec /= 10) > 1.0);	/* convert to a decimal number */
    return ((double) t + msec);
}
