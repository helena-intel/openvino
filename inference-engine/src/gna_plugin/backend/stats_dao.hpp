// Copyright (C) 2020 Intel Corporation
// SPDX-License-Identifier: Apache-2.0
//

#pragma once

#include <string>

class StatisticsDao {
public:
    enum stats_type_e {
        INPUT,
        OUTPUT,
        BIAS,
        STATS_TYPE_COUNT,
    };

    StatisticsDao(size_t layers_count);

    ~StatisticsDao();

    bool Serialize(const char *filename);

    static StatisticsDao *Deserialize(const char *filename);

    bool UpdateStatistics(size_t layer_id, stats_type_e type, const float *vec, size_t n);

    bool UpdateLayerName(size_t layer_id, const char *name);

    bool GetMinMax(size_t layer_id, stats_type_e type, float &min, float &max);

    bool GetMeanStdDev(size_t layer_id, stats_type_e type, float &mean, float &std_dev);

    size_t GetLayerCount() { return layers_count_; }

    int32_t GetLayerId(const char *name);

    void Print();

protected:
    struct statistics_t {
        double sum_sq;
        double sum;
        float min;
        float max;
        size_t samples_count;
        std::string ir_op_name;
    } *input_statistics_, *output_statistics_, *bias_statistics_;
    statistics_t *statistics_array_[STATS_TYPE_COUNT];
    size_t layers_count_;

private:
    std::string trim(const std::string &str,
                     const std::string &whitespace = " \t\r\n") {
        const auto strBegin = str.find_first_not_of(whitespace);
        if (strBegin == std::string::npos)
            return ""; // no content

        const auto strEnd = str.find_last_not_of(whitespace);
        const auto strRange = strEnd - strBegin + 1;

        return str.substr(strBegin, strRange);
    }
};
