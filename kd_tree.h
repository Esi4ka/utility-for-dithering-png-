#pragma once

#include <nanoflann.hpp>
#include <array>
#include <vector>
#include <cassert>
#include <iostream>

template <size_t Dim>
using Point = std::array<double, Dim>;

template <size_t Dim>
using Points = std::vector<Point<Dim>>;

template <size_t Dim, class Distance = nanoflann::metric_L2_Simple>
class KdTree {
private:
    using MetricType = typename Distance::template traits<double, KdTree<Dim>>::distance_t;
    using SelfType = KdTree<Dim>;
    using IndexType = nanoflann::KDTreeSingleIndexAdaptor<MetricType, SelfType, Dim, size_t>;

    Points<Dim> tree_data_ = {};
    IndexType* root_ = nullptr;

public:
    explicit KdTree(Points<Dim> points) : tree_data_(points) {
        const size_t dimension = tree_data_[0].size();
        root_ = new IndexType(static_cast<int>(dimension), *this,
                              nanoflann::KDTreeSingleIndexAdaptorParams(1));
    }

    size_t GetNearest(const Point<Dim>& point) const {
        // std::cout << "call get nearest! \n";

        nanoflann::KNNResultSet<double /*, size_t*/> result_set(1);
        std::vector<size_t> ret_indexes(1);
        std::vector<double> out_dists_sqr(1);
        //        size_t ret_indexes;
        //        double out_dists_sqr;

        result_set.init(&ret_indexes[0], &out_dists_sqr[0]);
        nanoflann::SearchParams my_param;
        root_->findNeighbors(result_set, &point[0], my_param);
        return ret_indexes[0];
    }

    const KdTree<Dim>& Derived() const {
        return *this;
    }

    KdTree<Dim>& Derived() {
        return *this;
    }

    inline size_t kdtree_get_point_count() const {
        return tree_data_.size();
    }

    inline double kdtree_get_pt(const size_t idx, const size_t dim) const {
        return tree_data_[idx][dim];
    }

    template <class BBOX>
    bool kdtree_get_bbox(BBOX& /*bb*/) const {
        return false;
    }

    ~KdTree() {
        if (root_ != nullptr) {
            delete root_;
        }
    }
};
