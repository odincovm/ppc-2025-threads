#include <gtest/gtest.h>

#include <algorithm>
#include <chrono>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <vector>

#include "core/perf/include/perf.hpp"
#include "core/task/include/task.hpp"
#include "stl/malyshev_a_increase_contrast_by_histogram/include/ops_stl.hpp"

namespace {

void FillData(std::vector<int> &data) {
  uint8_t tmp = 20;
  for (size_t i = 0; i < data.size(); i++) {
    data[i] = tmp++;
    if (tmp == 240) {
      tmp = 20;
    }
  }
}

}  // namespace

TEST(malyshev_a_increase_contrast_by_histogram_stl, test_pipeline_run) {
  constexpr int kCount = 150000000;

  std::vector<int> in(kCount);
  std::vector<int> out(kCount);

  FillData(in);

  auto task_data_stl = std::make_shared<ppc::core::TaskData>();
  task_data_stl->inputs.emplace_back(reinterpret_cast<uint8_t *>(in.data()));
  task_data_stl->inputs_count.emplace_back(in.size());
  task_data_stl->outputs.emplace_back(reinterpret_cast<uint8_t *>(out.data()));
  task_data_stl->outputs_count.emplace_back(out.size());

  auto test_task_stl = std::make_shared<malyshev_a_increase_contrast_by_histogram_stl::TestTaskSTL>(task_data_stl);

  auto perf_attr = std::make_shared<ppc::core::PerfAttr>();
  perf_attr->num_running = 10;
  const auto t0 = std::chrono::high_resolution_clock::now();
  perf_attr->current_timer = [&] {
    auto current_time_point = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(current_time_point - t0).count();
    return static_cast<double>(duration) * 1e-9;
  };

  auto perf_results = std::make_shared<ppc::core::PerfResults>();

  auto perf_analyzer = std::make_shared<ppc::core::Perf>(test_task_stl);
  perf_analyzer->PipelineRun(perf_attr, perf_results);
  ppc::core::Perf::PrintPerfStatistic(perf_results);

  auto [input_min, input_max] = std::ranges::minmax_element(in);
  auto [output_min, output_max] = std::ranges::minmax_element(out);
  EXPECT_LE(*output_min, *input_min);
  EXPECT_GE(*output_max, *input_max);
}

TEST(malyshev_a_increase_contrast_by_histogram_stl, test_task_run) {
  constexpr int kCount = 150000000;

  std::vector<int> in(kCount);
  std::vector<int> out(kCount);

  FillData(in);

  auto task_data_stl = std::make_shared<ppc::core::TaskData>();
  task_data_stl->inputs.emplace_back(reinterpret_cast<uint8_t *>(in.data()));
  task_data_stl->inputs_count.emplace_back(in.size());
  task_data_stl->outputs.emplace_back(reinterpret_cast<uint8_t *>(out.data()));
  task_data_stl->outputs_count.emplace_back(out.size());

  auto test_task_stl = std::make_shared<malyshev_a_increase_contrast_by_histogram_stl::TestTaskSTL>(task_data_stl);

  auto perf_attr = std::make_shared<ppc::core::PerfAttr>();
  perf_attr->num_running = 10;
  const auto t0 = std::chrono::high_resolution_clock::now();
  perf_attr->current_timer = [&] {
    auto current_time_point = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(current_time_point - t0).count();
    return static_cast<double>(duration) * 1e-9;
  };

  auto perf_results = std::make_shared<ppc::core::PerfResults>();

  auto perf_analyzer = std::make_shared<ppc::core::Perf>(test_task_stl);
  perf_analyzer->TaskRun(perf_attr, perf_results);
  ppc::core::Perf::PrintPerfStatistic(perf_results);

  auto [input_min, input_max] = std::ranges::minmax_element(in);
  auto [output_min, output_max] = std::ranges::minmax_element(out);
  EXPECT_LE(*output_min, *input_min);
  EXPECT_GE(*output_max, *input_max);
}
