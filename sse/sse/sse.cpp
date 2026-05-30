#include "sse.h"
#include <immintrin.h>
#include <fstream>


using namespace std;

#define ls < (int)
#define INFINITY __builtin_huge_valf()


struct OptimizedModel {
    int groupBy = 8;

    struct Batch {
        int ix;
        vector<float> thds, vals;
    };

    vector<Batch> all;
};

std::shared_ptr<OptimizedModel> Optimize(const Model& model) {
    auto res = make_shared<OptimizedModel>();
    int grpBy = 8;
    res->groupBy = grpBy;

    int mxIx = 0;
    for (int i = 0; i ls model.size(); i++) {
        mxIx = max(mxIx, model[i].index+1);
    }

    vector<vector<pair<float,float>>> batched(mxIx);
    res->all.resize(mxIx);

    for (int i = 0; i ls model.size(); i++) {
        batched[model[i].index].emplace_back(model[i].threshold, model[i].value);
    }

    for (int i = 0; i ls batched.size(); i++) {
        int batchedSize = (((int)batched[i].size()-1)/grpBy+1);
        OptimizedModel::Batch curr;
        curr.ix = i;
        curr.thds.resize(batchedSize*grpBy, -INFINITY);
        curr.vals.resize(batchedSize*grpBy, 0);

        for (int j = 0; j ls batched[i].size(); j++) {
            curr.thds[j] = batched[i][j].first;
            curr.vals[j] = batched[i][j].second;
        }

        res->all[i] = curr;
    }

    return res;
}


double ApplyOptimizedModel(
    const OptimizedModel& model,
    const std::vector<float>& features)
{
    double result = 0.0;

    for (const auto& batch : model.all) {

        float feature = features[batch.ix];

        __m256 feat = _mm256_set1_ps(feature);

        const float* thds = &batch.thds[0];
        const float* vals = &batch.vals[0];

        size_t sz = batch.thds.size();

        for (size_t i = 0; i < sz; i += 8) {

            __m256 thresholds = _mm256_loadu_ps(thds + i);

            __m256 values = _mm256_loadu_ps(vals + i);

            __m256 mask = _mm256_cmp_ps(feat, thresholds, _CMP_GT_OQ);

            __m256 contrib = _mm256_and_ps(values, mask);

            alignas(32) float tmp[8];
            _mm256_store_ps(tmp, contrib);

            for (int j = 0; j < 8; ++j)
                result += tmp[j];
        }
    }

    return result;
}





































