// Copyright (C) 2020 Intel Corporation
// SPDX-License-Identifier: Apache-2.0
//

#include "stats_dao.hpp"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

const char *OV_GNA_PLUGIN_STATS = "OV_GNA_PLUGIN_STATS";
char STATISTICS_NAME[StatisticsDao::stats_type_e::STATS_TYPE_COUNT] = {'I', 'O', 'B'};

StatisticsDao::StatisticsDao(size_t layers_count)
        : layers_count_(layers_count), input_statistics_(nullptr), output_statistics_(nullptr),
          bias_statistics_(nullptr) {
    input_statistics_ = new statistics_t[layers_count_];
    output_statistics_ = new statistics_t[layers_count_];
    bias_statistics_ = new statistics_t[layers_count_];

    statistics_array_[INPUT] = input_statistics_;
    statistics_array_[OUTPUT] = output_statistics_;
    statistics_array_[BIAS] = bias_statistics_;

    for (size_t t = 0; t < STATS_TYPE_COUNT; t++) {
        statistics_t *statistics = statistics_array_[t];
        for (size_t i = 0; i < layers_count_; i++) {
            statistics[i].max = -1e20f;
            statistics[i].min = 1e20f;
            statistics[i].samples_count = 0;
            statistics[i].sum_sq = 0.0;
            statistics[i].sum = 0.0;
        }
    }
}

StatisticsDao::~StatisticsDao() {
    delete[]input_statistics_;
    delete[]output_statistics_;
    delete[]bias_statistics_;
}

int32_t StatisticsDao::GetLayerId(const char *name) {
    if (name) {
        std::string trimed_name = trim(std::string(name));
        for (size_t i = 0; i < layers_count_; i++) {
            if (0 == output_statistics_[i].ir_op_name.compare(trimed_name)) {
                return i;
            }
        }
    }
    return -1;
}

bool StatisticsDao::Serialize(const char *filename) {
    FILE *file = fopen(filename, "wt");
    if (file) {
        fprintf(file, "%s %zu\n", OV_GNA_PLUGIN_STATS, layers_count_);


        for (size_t i = 0; i < layers_count_; i++) {
            for (int t = 0; t < STATS_TYPE_COUNT; t++) {
                statistics_t &stats = statistics_array_[t][i];
                fprintf(file, "%zu %c: %zu %.5f %.5f %.5lf %.5lf %s\n", i, STATISTICS_NAME[t], stats.samples_count,
                        stats.min, stats.max, stats.sum, stats.sum_sq, stats.ir_op_name.c_str());
            }
        }
        fclose(file);
        return true;
    }
    return false;
}

StatisticsDao *StatisticsDao::Deserialize(const char *filename) {
    bool success = true;
    StatisticsDao *obj = nullptr;
    FILE *file = fopen(filename, "rt");

    if (file) {
        char line[4096];
        char *hdr = fgets(line, sizeof(line) - 1, file);
        char *layer_count_str = nullptr;
        int layer_count = 0;
        size_t i = 0;

        if (nullptr != hdr &&
            0 == memcmp(OV_GNA_PLUGIN_STATS, hdr, strlen(OV_GNA_PLUGIN_STATS)) &&
            (layer_count_str = strchr(hdr, ' '))) {
            layer_count = atoi(layer_count_str);

            if (layer_count > 0) {
                obj = new StatisticsDao(layer_count);
                for (; success && i < layer_count * STATS_TYPE_COUNT; i++) {
                    char *layer_stats_str = fgets(line, sizeof(line) - 1, file);

                    int layer_id = atoi(layer_stats_str);
                    if (layer_id != (i / STATS_TYPE_COUNT)) {
                        success = false;
                        break;
                    }

                    layer_stats_str = strchr(layer_stats_str, ' ');
                    if (!layer_stats_str) {
                        success = false;
                        break;
                    }

                    statistics_t *stats = nullptr;
                    if (layer_stats_str[2] == ':') {
                        switch (layer_stats_str[1]) {
                            case 'I':
                                stats = &obj->input_statistics_[layer_id];
                                break;
                            case 'B':
                                stats = &obj->bias_statistics_[layer_id];
                                break;
                            case 'O':
                                stats = &obj->output_statistics_[layer_id];
                                break;
                        }
                    } else {
                        success = false;
                        break;
                    }
                    if (nullptr == stats) {
                        success = false;
                        break;
                    }
                    layer_stats_str = strchr(layer_stats_str + 1, ' ');
                    if (!layer_stats_str) {
                        success = false;
                        break;
                    }
                    stats->samples_count = atoi(layer_stats_str + 1);

                    layer_stats_str = strchr(layer_stats_str + 1, ' ');
                    if (!layer_stats_str) {
                        success = false;
                        break;
                    }
                    stats->min = atof(layer_stats_str);

                    layer_stats_str = strchr(layer_stats_str + 1, ' ');
                    if (!layer_stats_str) {
                        success = false;
                        break;
                    }
                    stats->max = atof(layer_stats_str);

                    layer_stats_str = strchr(layer_stats_str + 1, ' ');
                    if (!layer_stats_str) {
                        success = false;
                        break;
                    }
                    stats->sum = atof(layer_stats_str);

                    layer_stats_str = strchr(layer_stats_str + 1, ' ');
                    if (!layer_stats_str) {
                        success = false;
                        break;
                    }
                    stats->sum_sq = atof(layer_stats_str);
                    layer_stats_str = strchr(layer_stats_str + 1, ' ');

                    if (layer_stats_str) stats->ir_op_name = obj->trim(layer_stats_str);
                }
            }
        }
        fclose(file);
    }
    if (!success && obj) {
        delete obj;
        obj = nullptr;
    }

    return obj;
}

bool StatisticsDao::UpdateLayerName(size_t layer_id, const char *name) {
    if (layer_id < layers_count_ && name) {
        output_statistics_[layer_id].ir_op_name = trim(std::string(name));
        return true;
    } else {
        return false;
    }
}

bool StatisticsDao::UpdateStatistics(size_t layer_id, stats_type_e type, const float *vec, size_t n) {
    statistics_t *stats_type = nullptr;
    switch (type) {
        case BIAS:
            stats_type = bias_statistics_;
            break;
        case INPUT:
            stats_type = input_statistics_;
            break;
        case OUTPUT:
            stats_type = output_statistics_;
            break;
    }
    if (stats_type && layer_id < layers_count_) {
        double sum = 0.0;
        double sum_sq = 0.0;
        statistics_t &stats = stats_type[layer_id];
        for (size_t i = 0; i < n; i++) {
            float x = vec[i];
            sum += x;
            sum_sq += x * x;
            stats.min = stats.min < x ? stats.min : x;
            stats.max = stats.max > x ? stats.max : x;
        }
        stats.sum += sum;
        stats.sum_sq += sum_sq;
        stats.samples_count += n;
        return true;
    } else {
        return false;
    }
}

bool StatisticsDao::GetMinMax(size_t layer_id, stats_type_e type, float &min, float &max) {
    statistics_t *stats_type = nullptr;
    switch (type) {
        case BIAS:
            stats_type = bias_statistics_;
            break;
        case INPUT:
            stats_type = input_statistics_;
            break;
        case OUTPUT:
            stats_type = output_statistics_;
            break;
    }
    if (stats_type && layer_id < layers_count_) {
        statistics_t &stats = stats_type[layer_id];
        min = stats.min;
        max = stats.max;
        return true;
    } else {
        return false;
    }
}

bool StatisticsDao::GetMeanStdDev(size_t layer_id, stats_type_e type, float &mean, float &std_dev) {
    statistics_t *stats_type = nullptr;
    switch (type) {
        case BIAS:
            stats_type = bias_statistics_;
            break;
        case INPUT:
            stats_type = input_statistics_;
            break;
        case OUTPUT:
            stats_type = output_statistics_;
            break;
        default:
            break;
    }
    if (stats_type && layer_id < layers_count_) {
        statistics_t &stats = stats_type[layer_id];
        if (stats.samples_count > 0) {
            if (stats.samples_count > 1) {
                mean = stats.sum / stats.samples_count;
                double value = stats.sum_sq - 2.0 * mean * stats.sum + mean * mean * stats.samples_count;
                std_dev = (value < 0.0f) ? 0.0f : static_cast<float>(sqrt(value / (stats.samples_count - 1)));
            } else {
                mean = stats.sum;
                std_dev = 0.0f;
            }
        } else {
            mean = 0.0f;
            std_dev = 0.0f;
        }

        return true;
    } else {
        return false;
    }
}

void StatisticsDao::Print() {
    printf("Id\tType\tMin\tMax\tMean\tStd dev\tIR op name\n");

    for (size_t i = 0; i < layers_count_; i++) {
        for (int t = 0; t < STATS_TYPE_COUNT; t++) {
            float min{}, max{}, mean{}, std_dev{};
            GetMeanStdDev(i, (stats_type_e) t, mean, std_dev);
            GetMinMax(i, (stats_type_e) t, min, max);
            printf("%4zu\t%s\t%8.5f\t%8.5f\t%8.5f\t%8.5f\t%s\n", i, &STATISTICS_NAME[t], min, max, mean, std_dev,
                   output_statistics_->ir_op_name.c_str());
        }
    }
}
