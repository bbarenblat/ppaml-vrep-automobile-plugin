/* main.cpp -- plugin entry points
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

#include <cstddef>

#include <exception>
#include <stdexcept>
#include <string>

#include <boost/filesystem.hpp>
#include <v_repLib.h>

#include "automobile.h"
#include "main.h"
#include "vrep.h"
#include "vrepFfi.h"

namespace {

    const unsigned char PLUGIN_VERSION = 1;

    // The V-REP library that will get loaded at runtime
    const std::string VREP_LIBRARY_NAME = "libv_rep.so";
    LIBRARY vrepLibrary;


    // Prototypes //

    /* The library we think we're loading might not actually be the right
     * library. */
    class BadLibraryError : public std::runtime_error {
    public:
        explicit inline BadLibraryError();
    };

    // We might be using a too-old V-REP.
    class TooOldVrepError : public std::runtime_error {
    public:
        explicit inline TooOldVrepError(const simInt version);
    };

    /* Loads the V-REP library and ensures it has the correct set of symbols.
     * Throws 'std::exception' if an error occurs. */
    // This function is declared inline because it is only used once.
    inline void loadAndValidateVrepLibrary();

    /* Throws 'std::exception' if the current version of V-REP is strictly less
       than the 'simInt' passed. */
    // This function is declared inline because it is only used once.
    inline void requireVrepVersion(const simInt);
}

unsigned char v_repStart(void *, int) {
    try {
        loadAndValidateVrepLibrary();
        // If we needed a specific version of V-REP, we'd check it here with
        // 'requireVrepVersion'.
        vrep::InterfaceLockGuard lockInterface;
        // Register functions.
        registerLuaFunctions();
        // Returning zero indicates a failure condition.
        static_assert(PLUGIN_VERSION != 0, "Plugin version must be nonzero");
        return PLUGIN_VERSION;
    } catch (const std::exception& error) {
        // Loading the plugin failed.  Return 0 to indicate failure.
        return 0;
    }
}

void v_repEnd() {
    unloadVrepLibrary(vrepLibrary);
}

void *v_repMessage(int, int *, void *, int *) {
    return nullptr;
}

namespace {

    void loadAndValidateVrepLibrary() {
        try {
            // Load the V-REP library out of the V-REP directory.
            const std::string vrepLibraryPath =
                boost::filesystem::current_path().string()
                + "/" + VREP_LIBRARY_NAME;
            vrepLibrary = loadVrepLibrary(vrepLibraryPath.c_str());
            // Make sure we can actually get code out of the V-REP library.
            if (getVrepProcAddresses(vrepLibrary) == 0) {
                throw BadLibraryError();
            }
        } catch (const std::exception& e) {
            // Problems occurred loading the library; unload it before leaving.
            unloadVrepLibrary(vrepLibrary);
            throw e;
        }
    }

    void requireVrepVersion(const simInt expected) {
        simInt actual;
        VREP(simGetIntegerParameter(sim_intparam_program_version, &actual));
        if (actual < expected) {
            throw TooOldVrepError(expected);
        }
    }

    BadLibraryError::BadLibraryError()
        : std::runtime_error("could not find all required functions in "
                             + VREP_LIBRARY_NAME) {
    }

    TooOldVrepError::TooOldVrepError(const simInt version)
        : std::runtime_error("plugin requires V-REP >= "
                             + std::to_string(version)) {
    }

}
