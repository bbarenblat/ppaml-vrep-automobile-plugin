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

#ifndef PPAML_VREP_AUTOMOBILE_PLUGIN_NOISE_H
#define PPAML_VREP_AUTOMOBILE_PLUGIN_NOISE_H

#ifdef HAVE_CONFIG_H
#   include <config.h>
#endif

#include <random>
#include <stdexcept>
#include <string>
#include <vector>

#include "csv.h"
#include "measurement.h"


// Noise sources //

template<typename RealType = double>
class GaussianNoiseSource {
public:
    inline GaussianNoiseSource(RealType mean, RealType stddev);
    inline RealType get();

private:
    std::mt19937 generator;
    std::normal_distribution<RealType> distribution;
};

/* Convenience function to construct a GaussianNoiseSource<float> from a
 * two-element float vector representing mean and standard deviation.
 * Throws a std::invalid_argument if the vector is of the wrong
 * length. */
inline GaussianNoiseSource<float> gaussian(const std::vector<float>);


// Noisy values //

// Adds noise to a value.
Pose addNoise(const Pose &, GaussianNoiseSource<float> &positionNoise,
              GaussianNoiseSource<float> &angleNoise);
ControlSignals addNoise(const ControlSignals &,
                        GaussianNoiseSource<float> &speedNoise,
                        GaussianNoiseSource<float> &steeringAngleNoise);
LidarDatum addNoise(const LidarDatum &,
                    GaussianNoiseSource<float> &distanceNoise,
                    GaussianNoiseSource<float> &intensityNoise);


#include "noise-inl.h"

#endif
