/*
 * Copyright (C) 2015 Wang Ge
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, in version 3.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef __QFN_PORT_H__
#define __QFN_PORT_H__

#include <stdint.h>      /* Exact-width types. WG14/N843 C99 Standard */
#include <stdbool.h>     /* Boolean type.      WG14/N843 C99 Standard */
#include <xc.h>          /* Device header file */

/* QF-nano interrupt disable/enable... */
#define QF_INT_DISABLE() do { SRbits.IPL = 0x7; } while (0);
#define QF_INT_ENABLE()  do { SRbits.IPL = 0x0; } while (0);

/* QF-nano interrupt disabling policy for interrupt level */
#define QF_ISR_NEST      /* nesting of ISRs allowed */

#include "qepn.h"        /* QEP-nano platform-independent public interface */
#include "qfn.h"         /* QF-nano  platform-independent public interface */
#include "qvn.h"         /* QV-nano  platform-independent public interface */

#endif /* __QFN_PORT_H__ */
