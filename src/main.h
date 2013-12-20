/* main.h -- main plugin interface
 * Copyright (C) 2013  Galois, Inc.
 *
 * This library is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option) any later
 * version.
 *
 * This library is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this library.  If not, see <http://www.gnu.org/licenses/>.
 *
 * To contact Galois, complete the Web form at <http://corp.galois.com/contact/>
 * or write to Galois, Inc., 421 Southwest 6th Avenue, Suite 300, Portland,
 * Oregon, 97204-1622. */

#ifndef PPAML_VREP_AUTOMOBILE_PLUGIN_MAIN_H_
#define PPAML_VREP_AUTOMOBILE_PLUGIN_MAIN_H_

#ifdef HAVE_CONFIG_H
#   include <config.h>
#endif

/* This interface is defined by V-REP itself.  See
 * <http://www.coppeliarobotics.com/helpFiles/en/plugins.htm>. */

#ifdef __cplusplus
extern "C" {
#endif

    unsigned char v_repStart(void *pointer, int integer);
    void v_repEnd();
    void *v_repMessage(int message, int *auxiliary_data, void *custom_data,
                       int *reply_data);

#ifdef __cplusplus
}
#endif

#endif
