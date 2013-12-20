/* csv-inl.h -- CSV utilities
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

#ifndef PPAML_VREP_AUTOMOBILE_PLUGIN_CSV_INL_H
#define PPAML_VREP_AUTOMOBILE_PLUGIN_CSV_INL_H

namespace csv {

    template<typename SequenceContainer>
    std::string fromContainer(const SequenceContainer &columns) {
        std::ostringstream line;
        for (typename SequenceContainer::const_iterator col = columns.begin();
             col != columns.end();
             col++) {
            if (col != columns.begin()) {
                line << ",";
            }
            line << *col;
        }
        return line.str();
    }

}

#ifndef HAVE_CXX11_INITIALIZER_LISTS

    template<typename T>
    std::list<T> makeList(typename std::list<T>::size_type size, ...) {
        std::list<T> result;
        va_list args;
        va_start(args, size);
        for (typename std::list<T>::size_type i = 0; i < size; i++) {
            result.push_back(va_arg(args, T));
        }
        va_end(args);
        return result;
    }

#endif

#endif
