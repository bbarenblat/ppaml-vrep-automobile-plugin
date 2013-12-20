/* measurement.cpp -- data sets we gather
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

#include <array>
#include <string>
#include <vector>

#include "csv.h"
#include "measurement.h"

const std::string Pose::csvHeader_ = "TimeGPS,GPSLat,GPSLon,Orientation";

const std::string ControlSignals::csvHeader_ = "Time_VS,Velocity,Steering";

std::string LidarDatum::csvHeader() const {
    std::string result;
    const std::string timeLaserHeader = "TimeLaser";
    const std::string distanceHeader = "Laser";
    const std::string intensityHeader = "Intensity";
    result.reserve(timeLaserHeader.size()
                   + distance.size() * (1 + distanceHeader.size())
                   + intensity.size() * (1 + intensityHeader.size()));
    result.append(timeLaserHeader);
    for (std::vector<float>::size_type i = 0; i < distance.size(); i++) {
        result.append(",");
        result.append(distanceHeader);
    }
    for (std::vector<float>::size_type i = 0; i < intensity.size(); i++) {
        result.append(",");
        result.append(intensityHeader);
    }
    return result;
}

std::string LidarDatum::csv() const {
    std::string result = std::to_string(time);
    /* Yes, there are double braces in the initializer on the next line.  See
     * <http://stackoverflow.com/questions/11400090>. */
    const std::array<std::vector<float>, 2> dataSets = {{distance, intensity}};
    for (const std::vector<float> &data : dataSets) {
        if (! data.empty()) {
            result.append(",");
            result.append(csv::fromContainer(data));
        }
    }
    return result;
}
