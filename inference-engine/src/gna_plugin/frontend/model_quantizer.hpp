// Copyright (C) 2018-2020 Intel Corporation
// SPDX-License-Identifier: Apache-2.0
//

#pragma once

#include <vector>
#include <utility>
#include <string>

#include <legacy/layer_transform.hpp>
#include "gna_graph_tools.hpp"
#include <legacy/details/ie_cnn_network_tools.h>
#include "layer_quantizer.hpp"
#include "scale_factor_calc.hpp"
#include "weights_converter.hpp"

namespace GNAPluginNS {

/**
 * Quantize entire cnn - network
 * @tparam T - type trait for weights and biases
 */
template<class T>
class ModelQuantizer {
 public:
    InferenceEngine::ICNNNetwork::Ptr quantize(InferenceEngine::ICNNNetwork &model, float scaleFactor) const {
        return quantize(model, [](InferenceEngine::CNNNetPtr &, bool runBeforeCopy){}, std::vector<float>({scaleFactor}));
    }

    template <class PreQuantisationCb>
    InferenceEngine::ICNNNetwork::Ptr quantize(InferenceEngine::ICNNNetwork &model, const PreQuantisationCb &cb, float scaleFactor) const {
        return quantize(model, cb, std::vector<float>({scaleFactor}));
    }

    InferenceEngine::ICNNNetwork::Ptr quantize(InferenceEngine::ICNNNetwork &model, std::vector<float> scaleFactor) const {
        return quantize(model, [](InferenceEngine::CNNNetPtr &, bool runBeforeCopy){}, scaleFactor);
    }

    template <class PreQuantisationCb>
    InferenceEngine::ICNNNetwork::Ptr quantize(InferenceEngine::ICNNNetwork &model, const PreQuantisationCb &cb, std::vector<float> scaleFactor) const {
        auto visitor = [&](InferenceEngine::CNNLayerPtr lp) {
            auto newLayer = InferenceEngine::injectData<QuantizedLayerParams>(lp);
            transformLayer(newLayer, WeightsConverter());
            return newLayer;
        };
        auto copiedNet = InferenceEngine::CNNNetCopy(model);
        cb(copiedNet, true);

        IE_ASSERT(copiedNet.get() != nullptr);
        copiedNet = InferenceEngine::CNNNetCopy(*copiedNet, visitor);

        // TODO: probably not the best way of using dynamic cast in order to transform Precision
        // one of solution is to create not copyNet overloads, that accepts 2 functors, one for layer copy
        // and another one for net copy
        auto rawNet = dynamic_cast<InferenceEngine::details::CNNNetworkImpl *>(copiedNet.get());

        // allow client code to access copied topology, to avoid copies if user would like to chain quantisation with
        // another preprocessing
        cb(copiedNet, false);

        if (scaleFactor.empty()) {
            THROW_GNA_EXCEPTION << "Scale factor is empty";
        }

        LayersQuantizer<T> lc(*scaleFactor.begin());
        auto sortedNewNet = InferenceEngine::details::CNNNetSortTopologically(*copiedNet.get());
        gnalog() << "Sorted layers: " << std::endl;
        for (auto &&layer : sortedNewNet) {
            gnalog() << layer->name << std::endl;
        }
        /// filling scale factors for input layers, memory layers will have scaleFactor of 1.0 by default
        InferenceEngine::InputsDataMap dm;
        copiedNet->getInputsInfo(dm);
        int scaleIndex = 0;
        for (auto &&inputData : dm) {
            auto inputLayer = getCreatorLayer(inputData.second->getInputData()).lock();
            auto quantData = InferenceEngine::getInjectedData<QuantizedLayerParams>(inputLayer);
            if (scaleFactor.size() <= scaleIndex) {
                THROW_GNA_EXCEPTION << "Scale factors are not set for some of the inputs";
            }
            IE_ASSERT(quantData != nullptr);
            quantData->_src_quant.scale = scaleFactor[scaleIndex];
            scaleIndex++;
        }

        StatisticsDao* stats = StatisticsDao::Deserialize("layer_statistics.txt");
        const float MIN_DYNAMIC_RANGE = 1e-20;

        int index = 0;
        // set minimum dynamic range
        for (auto& layer : sortedNewNet) {
            auto curr_quant_data = InferenceEngine::getInjectedData<QuantizedLayerParams>(layer);
            auto it = LayerNameToType.find(layer->type);
            if (it != LayerNameToType.end() && it->second != LayerType::Input && curr_quant_data) {
                    curr_quant_data->_src_quant.agg_dynamic_range = 1.75f;
                    curr_quant_data->_dst_quant.agg_dynamic_range = 1.75f;
            }
            index++;
        }

        if (stats) {
            for (auto& layer : sortedNewNet) {
                auto quantData = InferenceEngine::getInjectedData<QuantizedLayerParams>(layer);
                int layer_id = stats->GetLayerId(layer->name.c_str());
                if (quantData && layer_id >= 0) {
                    float min, max;
                    stats->GetMinMax(layer_id, StatisticsDao::stats_type_e::OUTPUT, min, max);
                    // if min > max - scale factor not set
                    if (max > min) {
                        float absmax = max > -min ? max : -min;
                        quantData->_dst_quant.dynamic_range = absmax;
                        quantData->_dst_quant.dynamic_range_set = true;
                    }
                }
            }
            delete stats;
            bool finished = false;
            // overwrite dynamic range of input base on scale factor of input
            // this is done to unify scaling factor for all chunks of inputs
            for (auto& layer : sortedNewNet) {
                auto it = LayerNameToType.find(layer->type);
                if (it != LayerNameToType.end() && it->second == LayerType::Input)
                {
                    auto in_quant_data = InferenceEngine::getInjectedData<QuantizedLayerParams>(layer);
                    in_quant_data->_src_quant.dynamic_range = MAX_VAL_2B_FEAT / in_quant_data->_src_quant.scale;
                    in_quant_data->_src_quant.agg_dynamic_range = MAX_VAL_2B_FEAT / in_quant_data->_src_quant.scale;
                    in_quant_data->_src_quant.dynamic_range_set = true;
                    in_quant_data->_dst_quant.dynamic_range = MAX_VAL_2B_FEAT / in_quant_data->_src_quant.scale;
                    in_quant_data->_dst_quant.agg_dynamic_range = MAX_VAL_2B_FEAT / in_quant_data->_src_quant.scale;
                    in_quant_data->_dst_quant.dynamic_range_set = true;
                }
            }
            while (!finished) {
                finished = true;
                for (auto& layer : sortedNewNet) {
                    auto curr_quant_data = InferenceEngine::getInjectedData<QuantizedLayerParams>(layer);
                    if (curr_quant_data) {
                        if (layer->insData.size()) {
                            float agg_dynamic_range = -1.0f;
                            for (auto in_layer_ptr : layer->insData) {
                                const auto& in_layer = getCreatorLayer(in_layer_ptr.lock()).lock();

                                auto in_quant_data = InferenceEngine::getInjectedData<QuantizedLayerParams>(in_layer);
                                if (in_quant_data && (in_quant_data->_dst_quant.dynamic_range_set)) {
                                    // if this is first visit - we need to take dynamic range, which is not aggregated yet
                                    agg_dynamic_range = agg_dynamic_range > in_quant_data->_dst_quant.dynamic_range ?
                                        agg_dynamic_range : in_quant_data->_dst_quant.dynamic_range;
                                    // and aggregated one too
                                    agg_dynamic_range = agg_dynamic_range > in_quant_data->_dst_quant.agg_dynamic_range ?
                                        agg_dynamic_range : in_quant_data->_dst_quant.agg_dynamic_range;
                                }
                            }

                            if (!LayerInfo(layer).isChangingDynamicRange()) {
                                agg_dynamic_range = agg_dynamic_range > curr_quant_data->_src_quant.agg_dynamic_range ?
                                    agg_dynamic_range : curr_quant_data->_src_quant.agg_dynamic_range;
                                agg_dynamic_range = agg_dynamic_range > curr_quant_data->_dst_quant.agg_dynamic_range ?
                                    agg_dynamic_range : curr_quant_data->_dst_quant.agg_dynamic_range;
                                if (agg_dynamic_range > curr_quant_data->_src_quant.agg_dynamic_range ||
                                    agg_dynamic_range > curr_quant_data->_dst_quant.agg_dynamic_range)
                                {
                                    if (!curr_quant_data->_src_quant.dynamic_range_set)
                                        finished = false;
                                    curr_quant_data->_src_quant.agg_dynamic_range = agg_dynamic_range;
                                    curr_quant_data->_src_quant.dynamic_range_set = true;

                                    if (!curr_quant_data->_dst_quant.dynamic_range_set)
                                        finished = false;
                                    curr_quant_data->_dst_quant.agg_dynamic_range = agg_dynamic_range;
                                    curr_quant_data->_dst_quant.dynamic_range_set = true;
                                }
                            } else {
                                if (curr_quant_data->_src_quant.agg_dynamic_range < agg_dynamic_range)
                                    finished = false;
                                curr_quant_data->_src_quant.agg_dynamic_range = agg_dynamic_range;
                            }
                            if (agg_dynamic_range > 0.0f) {
                                for (auto& in_layer_ptr : layer->insData)
                                {
                                    const auto& in_layer = getCreatorLayer(in_layer_ptr.lock()).lock();
                                    auto in_quant_data = InferenceEngine::getInjectedData<QuantizedLayerParams>(in_layer);
                                    if (in_quant_data)
                                    {
                                        if (in_quant_data->_dst_quant.agg_dynamic_range < agg_dynamic_range)
                                        {
                                            in_quant_data->_dst_quant.agg_dynamic_range = agg_dynamic_range;
                                            in_quant_data->_dst_quant.dynamic_range_set = true;
                                            finished = false;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }

        propagateScaleFactor(sortedNewNet, T::mandatory().getWeightsPrecision().size());
        
        // sorted order gives possibility for propagate quantisation along depended layers
        for (auto &&layer : sortedNewNet) {
            transformLayer(layer, lc);
        }

        return copiedNet;
    }

 private :
    void propagateScaleFactor(std::vector<InferenceEngine::CNNLayerPtr> & net, int weightsBytesSize) const {
        ScaleFactorCalculator sf(net, weightsBytesSize);

        while (!sf.allLayersProcessed()) {
            for (auto &&layer : sf.getStartLayers()) {
                transformLayer(layer, sf);
                // transforming until we reached cases where output scale updated due to situation in downstream layer
                if (sf.needToRestart()) {
                    break;
                }
            }
        }
    }
};
}  // namespace GNAPluginNS
