/* measurement-inl.h -- data sets we gather
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

#ifndef PPAML_VREP_AUTOMOBILE_PLUGIN_MEASUREMENT_INL_H
#define PPAML_VREP_AUTOMOBILE_PLUGIN_MEASUREMENT_INL_H

std::string Pose::csvHeader() const {
    return csvHeader_;
}

std::string Pose::csv() const {
#   ifdef HAVE_CXX11_INITIALIZER_LISTS
        return csv::fromContainer(std::vector<float> {time, y, x, theta});
#   else
        return csv::fromContainer(makeList<float>(nCols(), time, y, x, theta));
#   endif
}

unsigned int Pose::nCols() const {
    return 4;
}

std::string ControlSignals::csvHeader() const {
    return csvHeader_;
}

std::string ControlSignals::csv() const {
#   ifdef HAVE_CXX11_INITIALIZER_LISTS
        return csv::fromContainer(
            std::vector<float> {time, speed, steeringAngle});
#   else
        return csv::fromContainer(
            makeList<float>(nCols(), time, speed, steeringAngle));
#   endif
}

unsigned int ControlSignals::nCols() const {
    return 3;
}

unsigned int LidarDatum::nCols() const {
    return 1 + distance.size() + intensity.size();
}

#endif
