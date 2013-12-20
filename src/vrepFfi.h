/* vrepFfi.h -- exposing functions to V-REP
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

#ifndef PPAML_VREP_AUTOMOBILE_PLUGIN_VREPFFI_H_
#define PPAML_VREP_AUTOMOBILE_PLUGIN_VREPFFI_H_

#ifdef HAVE_CONFIG_H
#   include <config.h>
#endif

#include <cstddef>
#include <cstring>

#include <forward_list>
#include <string>
#include <vector>

#include <v_repLib.h>

#include "vrep.h"

namespace vrep {

    namespace {

        // C-Lua type equivalences //

        template<typename T, int idVal>
        struct LuaTypeWrapper {
            static const int id = idVal;
            static inline T isNextArg(const SLuaCallBack *);
        };

        template<typename T>
        struct LuaType;

        template<>
        struct LuaType<std::nullptr_t>
            : public LuaTypeWrapper<std::nullptr_t, sim_lua_arg_nil> {
        };

        template<>
        struct LuaType<bool> : public LuaTypeWrapper<bool, sim_lua_arg_bool> {
            typedef simBool LuaT;
        };

        template<>
        struct LuaType<int> : public LuaTypeWrapper<int, sim_lua_arg_int> {
            typedef simInt LuaT;
        };

        template<>
        struct LuaType<float>
            : public LuaTypeWrapper<float, sim_lua_arg_float> {
            typedef simFloat LuaT;
        };

        template<>
        struct LuaType<std::string>
            : public LuaTypeWrapper<std::string, sim_lua_arg_string> {
            typedef simChar LuaT;
       };

        template<typename U>
        struct LuaType<std::vector<U>>
            : public LuaTypeWrapper<std::vector<U>,
                                    sim_lua_arg_table | LuaType<U>::id> {
            typedef typename LuaType<U>::LuaT LuaT;
        };


        // Converting a variadic template type to a Lua type list //

        std::forward_list<int> toLuaTypes();

        template<typename Car>
        std::forward_list<int> toLuaTypes();

        template<typename Car, typename Cadr, typename ...Cddr>
        std::forward_list<int> toLuaTypes();
    }


    // Exposing C++ functions to Lua //

    template<typename ...Args>
    void exposeFunction(const std::string name, const std::string callTips,
                        simVoid (*const f)(SLuaCallBack *));


    // Extracting Lua arguments from the callback structure //

    /* A class layered on 'SLuaCallBack' for handling calls from Lua into C++.
     * Watch out--interacting with this class at all outside of a C++ function
     * exposed to Lua will almost certainly cause undefined behavior! */
    class LuaCall {
    public:
        LuaCall(SLuaCallBack *);

        template<typename T>
        T expectAtom();

        template<typename T>
        std::vector<T> expectTable();

    private:
        void ensureNextArgType(const int expected);

        template<typename T>
        T unsafeGetAtom();

        template<typename T>
        std::vector<T> unsafeGetTable();

        SLuaCallBack *const simCall;
        size_t argIdx;
        simBool *cursorBool;
        simInt *cursorInt;
        simFloat *cursorFloat;
        simChar *cursorChar;
    };
    template<> inline bool LuaCall::unsafeGetAtom();
    template<> inline int LuaCall::unsafeGetAtom();
    template<> inline float LuaCall::unsafeGetAtom();
    template<> std::string LuaCall::unsafeGetAtom();


    // Error handling //

    // An error signaling a failed marshal from Lua to C++.
    class MarshalingError : public Error {
    public:
        explicit inline MarshalingError(const std::string &whatArg);
        explicit inline MarshalingError(const char *whatArg);
    };

}

#include "vrepFfi-inl.h"

#endif
