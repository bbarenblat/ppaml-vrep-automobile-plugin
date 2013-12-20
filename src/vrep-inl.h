/* vrep-inl.h -- convenience code to interface with V-REP
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

#ifndef PPAML_VREP_AUTOMOBILE_PLUGIN_VREP_INL_H_
#define PPAML_VREP_AUTOMOBILE_PLUGIN_VREP_INL_H_

namespace vrep {

    // class InterfaceLockGuard

    InterfaceLockGuard::InterfaceLockGuard() {
        lock();
    }

    InterfaceLockGuard::InterfaceLockGuard(const InterfaceLockGuard &other) {
        copyState(other);
    }

    InterfaceLockGuard &InterfaceLockGuard::operator=(
        const InterfaceLockGuard &other) {
        copyState(other);
        return *this;
    }

    InterfaceLockGuard::InterfaceLockGuard(InterfaceLockGuard &&other) {
        moveState(other);
    }

    InterfaceLockGuard &InterfaceLockGuard::operator=(
        InterfaceLockGuard &&other) {
        moveState(other);
        return *this;
    }

    void InterfaceLockGuard::lock() {
        VREP(simLockInterface(true));
        isLocked = true;
    }

    void InterfaceLockGuard::unlock() {
        VREP(simLockInterface(false));
        isLocked = false;
    }


    // class Error

    Error::Error(const std::string &whatArg)
        : std::runtime_error(whatArg) {
    }

    Error::Error(const char *whatArg)
        : Error(std::string(whatArg)) {
    }


    // template<typename T> class FunctionError<T>

    template<typename T>
    FunctionError<T>::FunctionError(const std::string &invocation,
                                        const T result)
        : Error("V-REP function invocation "
                + invocation
                + " failed, returning "
                + std::to_string(result)) {
    }

    template<typename T>
    FunctionError<T>::FunctionError(const char *invocation, const T result)
        : FunctionError(std::string(invocation), result) {
    }


    // template<typename T> T checkFunction(T, const char[])

    template<typename T>
    T checkFunction(T result, const char invocation[]) {
        if (isSuccess(result)) {
            return result;
        } else {
            throw vrep::FunctionError<T>(invocation, result);
        }
    }


    // template<typename T> bool isSuccess(const T)

    template<>
    bool isSuccess(const simInt val) {
        return val != -1;
    }

    template<>
    bool isSuccess(const simVoid *const val) {
        return val != nullptr;
    }

}

#endif

