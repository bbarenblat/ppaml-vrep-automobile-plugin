/* automobile.cpp -- recording data about the car
 * Copyright (C) 2013, 2014  Galois, Inc.
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

/* This module assumes only one car is recording data at once.  Thus, it follows
 * a singleton object pattern.  To keep everything a bit simpler, the state
 * variables live in an anonymous namespace, and the functions simply mutate
 * them as appropriate. */

#ifdef HAVE_CONFIG_H
#   include <config.h>
#endif

#include <cassert>
#include <cstdarg>

#include <array>
#include <fstream>
#include <functional>
#include <string>
#include <vector>

#include <boost/filesystem.hpp>
#include <boost/optional.hpp>
#include <v_repLib.h>

#include "automobile.h"
#include "noise.h"
#include "vrepFfi.h"

#ifndef HAVE_CXX11_INITIALIZER_LISTS
#   include <cassert>
#   include <list>
#endif


namespace {

    // Paths to the output files
    namespace path {

        std::string dataDir = "/tmp";

        const std::string groundDir = "/ground";
        const std::string noisyDir = "/noisy";

        const std::string properties = "/properties.csv";
        const std::string sensor = "/slam_sensor.csv";
        const std::string pose = "/slam_gps.csv";
        const std::string control = "/slam_control.csv";
        const std::string laser = "/slam_laser.csv";

    }

    // Run specifications
    struct Properties : public csv::Datum {
        inline Properties(float L, float h, float a, float b, float theta0)
            : L(L), h(h), a(a), b(b), theta0(theta0) {
        }
        inline virtual std::string csvHeader() const;
        virtual std::string csv() const;
        inline virtual unsigned int nCols() const;
        // Distance between front and rear axles
        float L;
        // Distance between center of rear axle and encoder on left rear wheel
        float h;
        // Lengthwise distance between the rear axle and the lidar
        float a;
        // Widthwise distance between the rear axle and the lidar
        float b;
        // Initial angle of the car
        float theta0;
    };

    // Individual sample records
    struct Sample : public csv::Datum {
        inline explicit Sample(const Pose &);
        inline explicit Sample(const ControlSignals &);
        inline explicit Sample(const LidarDatum &);
        inline virtual std::string csvHeader() const;
        inline virtual std::string csv() const;
        inline virtual unsigned int nCols() const;
        float time;
        unsigned short sensorId;
    };


    // Lidar specifications //
    namespace laser {

        float MAX_DISTANCE = 10.;
        float MAX_INTENSITY = 32768.;

    }


    // Noise //
    namespace noise {

        bool requested = false;

        // Dummy noise source that provides no noise
        GaussianNoiseSource<float> none(0., 0.);

        // Noise sources for the various measurements (can be set via Lua)
        boost::optional<GaussianNoiseSource<float>> position;
        boost::optional<GaussianNoiseSource<float>> angle;
        boost::optional<GaussianNoiseSource<float>> speed;
        boost::optional<GaussianNoiseSource<float>> steeringAngle;
        boost::optional<GaussianNoiseSource<float>> distance;
        boost::optional<GaussianNoiseSource<float>> intensity;

    }


    // Prototypes //

    // Lua callbacks
    typedef simVoid LuaFunc(SLuaCallBack *);
    LuaFunc init;
    LuaFunc setNoiseParameters;
    LuaFunc savePose;
    LuaFunc saveControls;
    LuaFunc saveLaser;

    inline void savePropertiesFile(const Properties &);

    template<typename CsvDatum>
    inline void saveDatum(const std::string &subdir,
                          const std::string &filename, const CsvDatum &);

    void outputDatum(const std::string &filePath, const csv::Datum &);

}


// Registering //

void registerLuaFunctions() {
    vrep::exposeFunction<
        std::string,
        float,
        float,
        float,
        float,
        float,
        float,
        float>(
        "simExtAutomobileInit",
        "simExtAutomobileInit(string directoryName, number L, number h, number a, number b, number theta0, number max_distance, number max_intensity)",
        init);
    vrep::exposeFunction<
        std::vector<float>,     // x and y
        std::vector<float>,     // angle
        std::vector<float>,     // speed
        std::vector<float>,     // steering angle
        std::vector<float>,     // intensity
        std::vector<float>>(    // distance
            "simExtAutomobileRequestNoise",
            "simExtAutomobileRequestNoise(table2 xy, table2 angle, table2 speed, table2 steeringAngle, table2 intensity, table2 distance)",
        setNoiseParameters);
    vrep::exposeFunction<float, float, float, float>(
        "simExtAutomobileSavePose",
        "simExtAutomobileSavePose(number simulationTime, number x, number y, number theta)",
        savePose);
    vrep::exposeFunction<float, float, float>(
        "simExtAutomobileSaveControls",
        "simExtAutomobileSaveControls(number simulationTime, number speed, number steeringAngle)",
        saveControls);
    /* V-REP's depth sensor part has a maximum field of view narrower than 180
     * degrees.  To compensate, we instead use two 90-degree depth sensors and
     * combine the results here. */
    vrep::exposeFunction<
        float,
        std::vector<float>, std::vector<float>,  // depths
        std::vector<float>, std::vector<float>>( // grayscale images
            "simExtAutomobileSaveLaserPair",
            "simExtAutomobileSaveLaserPair(number simulationTime, table leftDepthBuffer, table rightDepthBuffer, table leftImage, table rightImage)",
            saveLaser);
}


// Callbacks //
namespace {

    std::string Properties::csvHeader() const {
#       ifdef HAVE_CXX11_INITIALIZER_LISTS
            return csv::fromContainer(
                std::vector<std::string> {"L", "h", "a", "b", "InitialAngle"});
#       else
            return csv::fromContainer(
                makeList<const char *>(nCols(), "L", "h", "a", "b",
                                       "InitialAngle"));
#       endif
    }

    std::string Properties::csv() const {
#       ifdef HAVE_CXX11_INITIALIZER_LISTS
            return csv::fromContainer(
                std::vector<float> {L, h, a, b, theta0});
#       else
            return csv::fromContainer(
                makeList<float>(nCols(), L, h, a, b, theta0));
#       endif
    }

    unsigned int Properties::nCols() const {
        return 5;
    }

    Sample::Sample(const Pose &pose)
        : time(pose.time), sensorId(1) {
    }

    Sample::Sample(const ControlSignals &signals)
        : time(signals.time), sensorId(2) {
    }

    Sample::Sample(const LidarDatum &datum)
        : time(datum.time), sensorId(3) {
    }

    std::string Sample::csvHeader() const {
        return "Time,Sensor";
    }

    std::string Sample::csv() const {
        return std::to_string(time) + "," + std::to_string(sensorId);
    }

    unsigned int Sample::nCols() const {
        return 2;
    }

    simVoid init(SLuaCallBack *const simCall) {
        // Clean data from previous runs.
        boost::filesystem::remove_all(path::dataDir + path::groundDir);
        boost::filesystem::remove_all(path::dataDir + path::noisyDir);
        boost::filesystem::remove(path::dataDir + path::properties);
        // Save the passed directory as the base directory for the output.
        vrep::LuaCall call(simCall);
        path::dataDir = call.expectAtom<std::string>();
        // Save the passed properties in the properties file.
        const float L = call.expectAtom<float>();
        const float h = call.expectAtom<float>();
        const float a = call.expectAtom<float>();
        const float b = call.expectAtom<float>();
        const float theta0 = call.expectAtom<float>();
        savePropertiesFile(Properties(L, h, a, b, theta0));
        // Save the maximum distance and intensity settings.
        laser::MAX_DISTANCE = call.expectAtom<float>();
        laser::MAX_INTENSITY = call.expectAtom<float>();
        // Reset the noise settings.
        noise::requested = false;
        const std::array<boost::optional<GaussianNoiseSource<float>> *const, 6> sources =
            {{&noise::position, &noise::angle, &noise::speed,
              &noise::steeringAngle, &noise::intensity, &noise::distance}};
        for (boost::optional<GaussianNoiseSource<float>> *const source : sources) {
            *source = boost::none;
        }
    }

    simVoid setNoiseParameters(SLuaCallBack *const simCall) {
        vrep::LuaCall call(simCall);
        const std::array<boost::optional<GaussianNoiseSource<float>> *const, 6> sources =
            {{&noise::position, &noise::angle, &noise::speed,
              &noise::steeringAngle, &noise::intensity, &noise::distance}};
        for (boost::optional<GaussianNoiseSource<float>> *const source : sources) {
            *source = gaussian(call.expectTable<float>());
        }
        noise::requested = true;
    }

    simVoid savePose(SLuaCallBack *const simCall) {
        // Build the pose.
        vrep::LuaCall call(simCall);
        const float time = call.expectAtom<float>();
        const float x = call.expectAtom<float>();
        const float y = call.expectAtom<float>();
        const float theta = call.expectAtom<float>();
        const Pose pose(time, x, y, theta);
        // Record it.
        saveDatum(path::groundDir, path::pose, pose);
        if (noise::requested) {
            saveDatum(path::noisyDir, path::pose,
                      addNoise(pose, noise::position.get_value_or(noise::none),
                               noise::angle.get_value_or(noise::none)));
        }
    }

    simVoid saveControls(SLuaCallBack *const simCall) {
        // Build the control systems object.
        vrep::LuaCall call(simCall);
        const float time = call.expectAtom<float>();
        const float speed = call.expectAtom<float>();
        const float angle = call.expectAtom<float>();
        const ControlSignals signals(time, speed, angle);
        // Record it.
        saveDatum(path::groundDir, path::control, signals);
        if (noise::requested) {
            saveDatum(path::noisyDir, path::control,
                      addNoise(signals, noise::speed.get_value_or(noise::none),
                               noise::steeringAngle.get_value_or(noise::none)));
        }
    }

    simVoid saveLaser(SLuaCallBack *const simCall) {
        // Get the arguments and reconstruct the full lidar measurements.
        vrep::LuaCall call(simCall);
        const float time = call.expectAtom<float>();
        std::vector<float> distance = call.expectTable<float>();
        const std::vector<float> distanceRight = call.expectTable<float>();
        distance.insert(distance.end(),
                        distanceRight.begin(), distanceRight.end());
        std::vector<float> image = call.expectTable<float>();
        const std::vector<float> imageRight = call.expectTable<float>();
        image.insert(image.end(), imageRight.begin(), imageRight.end());
        // Process the lidar measurements.
#       ifdef HAVE_CXX11_CLOSURES
            std::for_each(distance.begin(), distance.end(),
                          [](float &d) { d *= laser::MAX_DISTANCE; });
            std::for_each(image.begin(), image.end(),
                          [](float &i) { i *= laser::MAX_INTENSITY; });
#       else
            for (float &d : distance) {
                d *= laser::MAX_DISTANCE;
            }
            for (float &i : image) {
                i *= laser::MAX_INTENSITY;
            }
#       endif
        // Build the lidar datum and record it.
        const LidarDatum datum(time, distance, image);
        saveDatum(path::groundDir, path::laser, datum);
        if (noise::requested) {
            saveDatum(path::noisyDir, path::laser,
                      addNoise(datum, noise::distance.get_value_or(noise::none),
                               noise::intensity.get_value_or(noise::none)));
        }
    }

    void savePropertiesFile(const Properties &properties) {
        boost::filesystem::create_directories(path::dataDir);
        outputDatum(path::dataDir + path::properties, properties);
    }

    template<typename CsvDatum>
    void saveDatum(const std::string &subdir, const std::string &filename,
                   const CsvDatum &datum) {
        const std::string logDir = path::dataDir + subdir;
        boost::filesystem::create_directories(logDir);
        outputDatum(logDir + filename, datum);
        outputDatum(logDir + path::sensor, Sample(datum));
    }

    void outputDatum(const std::string &filePath, const csv::Datum &datum) {
        const std::string header = datum.csvHeader();
        std::fstream file(filePath,
                          std::ios::in | std::ios::out | std::ios::app);
        // Ensure we're writing correctly-formatted data.
        std::string firstLine;
        std::getline(file, firstLine);
        if (file.eof() && firstLine.empty()) {
            // The file was empty, so write in the header.
            file.clear();
            file << header << std::endl;
        } else if (firstLine != header) {
            /* The file was nonempty, but the first line did not match the
             * expected header.  This might occur if an external program
             * modifies the file in between our writes, but more likely, we've
             * got a bug somewhere that is causing us to write different data
             * sets to the same file. */
            throw std::logic_error(
                std::string("CSV header mismatch: expected `")
                + header
                + "', but got `"
                + firstLine
                + "'");
        } else {
            /* The file was nonempty, and the headers matched what we expected.
             * We're good to go. */
        }
        // Write the datum.
        file << datum.csv() << std::endl;
    }

}
