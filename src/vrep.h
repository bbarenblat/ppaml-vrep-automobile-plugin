/* vrep.h -- convenience code to interface with V-REP
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

#ifndef PPAML_VREP_AUTOMOBILE_PLUGIN_VREP_H_
#define PPAML_VREP_AUTOMOBILE_PLUGIN_VREP_H_

#ifdef HAVE_CONFIG_H
#   include <config.h>
#endif

#include <cstddef>

#include <stdexcept>
#include <string>

#include <v_repLib.h>

/* All the convenience code lives in the 'vrep' namespace to distinguish it from
 * the actual V-REP functions, which all are prefixed with 'sim'. */
namespace vrep {

    // A RAII wrapper for 'simLockInterface'.
    class InterfaceLockGuard {
    public:
        inline InterfaceLockGuard();
        ~InterfaceLockGuard() noexcept;
        // Copying
        inline InterfaceLockGuard(const InterfaceLockGuard &);
        inline InterfaceLockGuard &operator=(const InterfaceLockGuard &);
        // Moving
        inline InterfaceLockGuard(InterfaceLockGuard &&);
        inline InterfaceLockGuard &operator=(InterfaceLockGuard &&);

    private:
        inline void lock();
        inline void unlock();
        void copyState(const InterfaceLockGuard &);
        void moveState(InterfaceLockGuard &);

        bool isLocked;
    };


    // Error handling //

    // The class for all errors which occur when interfacing with V-REP.
    class Error : public std::runtime_error {
    public:
        explicit inline Error(const std::string &whatArg);
        explicit inline Error(const char *whatArg);
    };

    // An error signaling a failed V-REP function invocation.
    template<typename T>
    class FunctionError : public Error {
    public:
        FunctionError(const std::string &invocation, const T result);
        inline FunctionError(const char *invocation, const T result);
    };

/* A nice macro to convert failed V-REP function invocations to
 * 'FunctionError's.  Simply wrap your V-REP function interface--e.g.,
 *
 *     simInt x = VREP(simGetIntegerParameter(foo, &bar));
 *
 * --and you'll be off to the races.  'x' will be assigned the result of the
 * invocation if and only if the invocation succeeded; otherwise, a
 * 'FunctionError' describing the failed invocation will be thrown.
 *
 * This needs to be a macro so that it can include the code of the failed
 * invocation itself. */
#define VREP(invocation) vrep::checkFunction((invocation), #invocation)

    template<typename T>
    T checkFunction(T result, const char invocation[]);

    /* Returns 'true' iff the passed value signifies a successful V-REP function
     * invocation. */
    template<typename T>
    inline bool isSuccess(const T);
    template<> inline bool isSuccess(const simInt);
    template<> inline bool isSuccess(const simVoid *);
}

#include "vrep-inl.h"

#endif
