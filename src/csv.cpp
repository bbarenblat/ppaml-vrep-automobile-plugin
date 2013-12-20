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

#ifdef HAVE_CONFIG_H
#   include <config.h>
#endif

#ifndef HAVE_CXX11_INITIALIZER_LISTS
#   include <cstdarg>
#   include <list>
#   include <sstream>
#endif

#include "csv.h"

#ifndef HAVE_CXX11_INITIALIZER_LISTS

    std::list<float> makeList(typename std::list<float>::size_type size, ...) {
        std::list<float> result;
        va_list args;
        va_start(args, size);
        for (typename std::list<double>::size_type i = 0;
             i < static_cast<std::list<double>::size_type>(size);
             i++) {
            result.push_back(va_arg(args, double));
        }
        va_end(args);
        return result;
    }

#endif
