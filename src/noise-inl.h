/* noise-inl.h -- noisy data
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

#ifndef PPAML_VREP_AUTOMOBILE_PLUGIN_NOISE_INL_H
#define PPAML_VREP_AUTOMOBILE_PLUGIN_NOISE_INL_H

#ifdef HAVE_CONFIG_H
#   include <config.h>
#endif

template<typename RealType>
GaussianNoiseSource<RealType>::GaussianNoiseSource(RealType mean,
                                                   RealType stddev)
	: generator(), distribution(mean, stddev) {
    generator.seed(std::random_device()());
}

template<typename RealType>
RealType GaussianNoiseSource<RealType>::get() {
    return distribution(generator);
}

GaussianNoiseSource<float> gaussian(const std::vector<float> params) {
    if (params.size() != 2) {
        throw std::invalid_argument(std::string()
            + "expected two-element vector of distribution parameters "
            + "(got " + std::to_string(params.size()) + "-element "
            + "vector instead");
    } else {
        return GaussianNoiseSource<float>(params[0], params[1]);
    }
}

#endif
