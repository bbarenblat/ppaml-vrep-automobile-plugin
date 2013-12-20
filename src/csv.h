/* csv.h -- CSV utilities
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

#ifndef PPAML_VREP_AUTOMOBILE_PLUGIN_CSV_H
#define PPAML_VREP_AUTOMOBILE_PLUGIN_CSV_H

#ifdef HAVE_CONFIG_H
#   include <config.h>
#endif

#include <string>
#include <sstream>

#ifndef HAVE_CXX11_INITIALIZER_LISTS
#   include <cstdarg>
#   include <list>
#endif

namespace csv {

    // Interface: Any datum which can be converted to a CSV string.
    struct Datum {
        virtual std::string csvHeader() const = 0;
        virtual std::string csv() const = 0;
        virtual unsigned int nCols() const = 0;
    };

    /* Converts a sequence of objects to a CSV string.  It uses stringstreams
     * internally, so all data in the sequence will get converted to strings
     * using operator<<; however, no escaping will occur. */
    template<typename SequenceContainer>
    std::string fromContainer(const SequenceContainer &);

}

#ifndef HAVE_CXX11_INITIALIZER_LISTS

    /* Without C++11 initializer list syntax, creating containers to pass to
     * csv::fromContainer is really ugly.  This function makes it substantially
     * nicer, at the cost of forcing you to manually specify the length of the
     * container (an unfortunate varargs limitation).  */
    template<typename T>
    std::list<T> makeList(typename std::list<T>::size_type, ...);

    /* floats get promoted to doubles in a va_list, so they need special
     * handling. */
    template<>
    std::list<float> makeList(typename std::list<float>::size_type, ...);

#endif

#include "csv-inl.h"

#endif
