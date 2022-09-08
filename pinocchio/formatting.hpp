#pragma once

#include <fmt/core.h>
#include <fmt/ostream.h>
#include <Eigen/Dense>

template <class T, int N1, int N2>
struct fmt::formatter<Eigen::Matrix<T, N1, N2>> : public fmt::ostream_formatter {};

//template <class D>
//struct fmt::formatter<Eigen::EigenBase<D>> : public fmt::ostream_formatter {};

template <class T, int N1, int N2>
struct fmt::formatter<Eigen::WithFormat<Eigen::Matrix<T, N1, N2>>> : public fmt::ostream_formatter {
};