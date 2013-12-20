/* vrepFfi.cpp -- exposing functions to V-REP
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

#ifdef HAVE_CONFIG_H
#   include <config.h>
#endif

#include <cstring>

#include <string>

#include "vrepFfi.h"

namespace vrep {

    // Extracting Lua arguments from the callback structure //

    LuaCall::LuaCall(SLuaCallBack *simCall)
        : simCall(simCall), argIdx(0), cursorBool(simCall->inputBool),
          cursorInt(simCall->inputInt), cursorFloat(simCall->inputFloat),
          cursorChar(simCall->inputChar) {
    }

    void LuaCall::ensureNextArgType(const int expected) {
        const int nextType = simCall->inputArgTypeAndSize[2 * argIdx];
        if (nextType != expected) {
            throw MarshalingError("unexpected Lua argument (expected type "
                                  + std::to_string(expected)
                                  + ", got type "
                                  + std::to_string(nextType)
                                  + ")");
        }
    }

    template<>
    std::string LuaCall::unsafeGetAtom() {
        // Determine how long the string is.  Lua uses '@' as its sentinel.
        size_t len;
        simChar *sentinel = std::strchr(cursorChar, '@');
        if (sentinel) {
            len = sentinel - cursorChar;
        } else {
            len = std::strlen(cursorChar);
        }
        // Build the 'std::string'.
        const std::string result = std::string(cursorChar, len);
        cursorChar += len + (sentinel ? 1 : 0);
        return result;
    }

}
