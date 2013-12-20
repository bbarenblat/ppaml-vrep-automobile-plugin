/* noise.h -- noisy data
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

#ifdef HAVE_CXX11_CLOSURES
#   include <algorithm>
#endif

#include "measurement.h"
#include "noise.h"

Pose addNoise(const Pose &pose,
              GaussianNoiseSource<float> &positionNoise,
              GaussianNoiseSource<float> &angleNoise) {
    Pose result = pose;
    result.x += positionNoise.get();
    result.y += positionNoise.get();
    result.theta += angleNoise.get();
    return result;
}

ControlSignals addNoise(const ControlSignals &signals,
                        GaussianNoiseSource<float> &speedNoise,
                        GaussianNoiseSource<float> &steeringAngleNoise) {
    ControlSignals result = signals;
    result.speed += speedNoise.get();
    result.steeringAngle += steeringAngleNoise.get();
    return result;
}

LidarDatum addNoise(const LidarDatum &datum,
                    GaussianNoiseSource<float> &distanceNoise,
                    GaussianNoiseSource<float> &intensityNoise) {
    LidarDatum result = datum;
#   ifdef HAVE_CXX11_CLOSURES
        std::for_each(result.distance.begin(), result.distance.end(),
                      [&distanceNoise](float &d) {
                          d += distanceNoise.get();
                      });
        std::for_each(result.intensity.begin(), result.intensity.end(),
                      [&intensityNoise](float &i) {
                          i += intensityNoise.get();
                      });
#   else
        for (float &d : result.distance) {
            d += distanceNoise.get();
        }
        for (float &i : result.intensity) {
            i += intensityNoise.get();
        }
#   endif
    return result;
}
