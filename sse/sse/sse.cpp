#include "sse.h"


using namespace std;

#define ls < (int)



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













































std::vector<std::vector<float>> LoadDataset(const std::string& path) {
    std::ifstream in(path);
    if (!in.is_open()) {
        throw std::runtime_error("can't open file '" + path + "'");
    }

    int n, m;
    in >> n >> m;

    std::vector<std::vector<float>> dataset;
    for (int i = 0; i < n; ++i) {
        auto& element = dataset.emplace_back(m);
        for (int j = 0; j < m; ++j) {
            in >> element[j];
        }
    }

    return dataset;
}

Model LoadModel(const std::string& path) {
    std::ifstream in(path);
    if (!in.is_open()) {
        throw std::runtime_error("can't open file '" + path + "'");
    }

    int n;
    in >> n;
    Model model;
    for (int i = 0; i < n; ++i) {
        auto& rule = model.emplace_back();
        in >> rule.index >> rule.threshold >> rule.value;
    }

    return model;
}

double ApplyModel(const Model& model, const std::vector<float>& features) {
    double result = 0;
    for (const auto& rule : model) {
        if (features[rule.index] > rule.threshold) {
            result += rule.value;
        }
    }
    return result;
}


