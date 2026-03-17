#include "Math/DiscreteDistribution.hpp"

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <iterator>
#include <vector>

namespace VI {

std::vector<float> NormalizeWeightsToPDF(const std::vector<float> &weights) {
  (void)weights;
  // TODO(student): convert a vector of non-normalized positive weights into a
  // discrete PDF.
  return {};
}

std::vector<float> BuildCDF(const std::vector<float> &pdf) {
  (void)pdf;
  // TODO(student): build the cumulative distribution function associated with
  // the input PDF.
  return {};
}

int SampleCDFIndex(const std::vector<float> &cdf, float u) {
  (void)cdf;
  (void)u;
  // TODO(student): given u in [0, 1), return the sampled discrete index from
  // the CDF.
  return -1;
}

float GetPDFValue(const std::vector<float> &pdf, int index) {
  (void)pdf;
  (void)index;
  // TODO(student): return the probability associated with the sampled index.
  return 0.f;
}

} // namespace VI
