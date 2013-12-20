/* vrepFfi-inl.h -- exposing functions to V-REP
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

#ifndef PPAML_VREP_AUTOMOBILE_PLUGIN_VREPFFI_INL_H_
#define PPAML_VREP_AUTOMOBILE_PLUGIN_VREPFFI_INL_H_

namespace vrep {

    namespace {

        // Converting a variadic template type to a Lua type list //

        std::forward_list<int> toLuaTypes() {
            return std::forward_list<int>();
        }

        template<typename Car>
        std::forward_list<int> toLuaTypes() {
            std::forward_list<int> luaTypes = toLuaTypes();
            luaTypes.push_front(LuaType<Car>::id);
            return luaTypes;
        }

        template<typename Car, typename Cadr, typename ...Cddr>
        std::forward_list<int> toLuaTypes() {
            std::forward_list<int> luaTypes = toLuaTypes<Cadr, Cddr...>();
            luaTypes.push_front(LuaType<Car>::id);
            return luaTypes;
        }

    }


    // Exposing C++ functions to Lua //

    template<typename ...Args>
    void exposeFunction(const std::string name, const std::string callTips,
                        simVoid (*const f)(SLuaCallBack *)) {
        // Compute the Lua signature.
        std::vector<int> luaTypes;
        luaTypes.push_back(0);
        for (int type : toLuaTypes<Args...>()) {
            luaTypes.push_back(type);
            // We just added an argument.
            luaTypes[0]++;
        }
        // Register the function with Lua.
        VREP(simRegisterCustomLuaFunction(name.c_str(), callTips.c_str(),
                                          luaTypes.data(), f));
    }


    // Extracting Lua arguments from the callback structure //

    template<typename T>
    T LuaCall::expectAtom() {
        ensureNextArgType(LuaType<T>::id);
        const T result = unsafeGetAtom<T>();
        argIdx++;
        return result;
    }

    template<typename T>
    std::vector<T> LuaCall::expectTable() {
        ensureNextArgType(LuaType<std::vector<T>>::id);
        const std::vector<T> result = unsafeGetTable<T>();
        argIdx++;
        return result;
    }

    template<typename T>
    std::vector<T> LuaCall::unsafeGetTable() {
        // Determine how long the table is.
        size_t tableLen;
        if (simCall->inputArgTypeAndSize[2 * argIdx] & sim_lua_arg_table) {
            tableLen = simCall->inputArgTypeAndSize[2 * argIdx + 1];
        } else {
            tableLen = 1;
        }
        // Build the result.
        std::vector<T> result(tableLen);
        for (size_t i = 0; i < tableLen; i++) {
            result[i] = unsafeGetAtom<T>();
        }
        return result;
    }

    template<>
    bool LuaCall::unsafeGetAtom() {
        return *cursorBool++;
    }

    template<>
    int LuaCall::unsafeGetAtom() {
        return *cursorInt++;
    }

    template<>
    float LuaCall::unsafeGetAtom() {
        return *cursorFloat++;
    }


    // Error handling //

    MarshalingError::MarshalingError(const std::string &whatArg)
        : Error(whatArg) {
    }

    MarshalingError::MarshalingError(const char *whatArg)
        : MarshalingError(std::string(whatArg)) {
    }

}

#endif
