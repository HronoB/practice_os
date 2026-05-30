#include "sse.h"


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

        __m128 feat = _mm_set1_ps(feature);

        const float* thds = batch.thds.data();
        const float* vals = batch.vals.data();

        size_t sz = batch.thds.size();

        __m128 acc = _mm_setzero_ps();

        for (size_t i = 0; i < sz; i += 4) {

            __m128 thresholds = _mm_loadu_ps(thds + i);
            __m128 values     = _mm_loadu_ps(vals + i);

            __m128 mask = _mm_cmpgt_ps(feat, thresholds);

            __m128 contrib = _mm_and_ps(values, mask);

            acc = _mm_add_ps(acc, contrib);
        }

        alignas(16) float tmp[4];
        _mm_store_ps(tmp, acc);

        result += tmp[0] + tmp[1] + tmp[2] + tmp[3];
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


