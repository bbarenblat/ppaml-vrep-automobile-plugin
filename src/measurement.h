/* measurement.h -- data sets we gather
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

#ifndef PPAML_VREP_AUTOMOBILE_PLUGIN_MEASUREMENT_H
#define PPAML_VREP_AUTOMOBILE_PLUGIN_MEASUREMENT_H

#ifdef HAVE_CONFIG_H
#   include <config.h>
#endif

#include <string>
#include <vector>

#include "csv.h"

// Base class for time-series data
struct Datum {
    inline Datum(float time)
        : time(time) {
    }
    float time;
};


// Datum definitions //

class Pose : public Datum, public csv::Datum {
public:
    inline Pose(float time, float x, float y, float theta)
        : ::Datum(time), x(x), y(y), theta(theta) {
    }
    float x;
    float y;
    float theta;
    inline virtual std::string csvHeader() const;
    inline virtual std::string csv() const;
    inline virtual unsigned int nCols() const;

protected:
    static const std::string csvHeader_;
};


class ControlSignals : public Datum, public csv::Datum {
public:
    inline ControlSignals(float time, float speed, float steeringAngle)
        : ::Datum(time), speed(speed), steeringAngle(steeringAngle) {
    }
    float speed;
    float steeringAngle;
    inline virtual std::string csvHeader() const;
    inline virtual std::string csv() const;
    inline virtual unsigned int nCols() const;

protected:
    static const std::string csvHeader_;
};


class LidarDatum : public Datum, public csv::Datum {
public:
    inline LidarDatum(float time, std::vector<float> distance,
                      std::vector<float> intensity)
        : ::Datum(time), distance(distance), intensity(intensity) {
    }
    std::vector<float> distance;
    std::vector<float> intensity;
    virtual std::string csvHeader() const;
    inline virtual std::string csv() const;
    inline virtual unsigned int nCols() const;
};


#include "measurement-inl.h"

#endif
