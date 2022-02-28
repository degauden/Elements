if(NOT GLIBCXXPARALLEL_FOUND)

  set(GLIBCXXPARALLEL_CXX_FLAGS)
  set(GLIBCXXPARALLEL_C_FLAGS)
  set(GLIBCXXPARALLEL_WORKS)
  set(GLIBCXXPARALLEL_DEFINITIONS)


  find_package(OpenMP)
  
  set(GLIBCXXPARALLEL_CXX_FLAGS ${GLIBCXXPARALLEL_CXX_FLAGS} ${OpenMP_CXX_FLAGS}) 
  set(GLIBCXXPARALLEL_C_FLAGS ${GLIBCXXPARALLEL_C_FLAGS} ${OpenMP_C_FLAGS}) 
  
  INCLUDE(CheckCXXSourceRuns)
  set(CMAKE_REQUIRED_FLAGS ${OpenMP_CXX_FLAGS})
  set(CMAKE_REQUIRED_DEFINITIONS -D_GLIBCXX_PARALLEL)
  
  CHECK_CXX_SOURCE_RUNS("
#include <vector>
#include <numeric>

int main(int, char**) {

  int ret_code = 0;

  const std::size_t val_size_t = 1000;

  std::vector<double> vals(val_size_t, 1.0);

  std::iota(vals.begin(), vals.end(), 0.0);

  auto squared = std::accumulate(vals.begin(), vals.end(), 0.0,
                             [](const double& tot, const double& vA) {
                               return tot + vA * vA;
                             });
  double squared2 = 0.0;
  for (const auto& v : vals) {
    squared2 += v * v;
  }


  if (not (std::abs(squared2-squared) <= 1000.0)) {
    ret_code = 1;
  }


  return ret_code;

}"
  GLIBCXXPARALLEL_WORKS)
  mark_as_advanced(GLIBCXXPARALLEL_WORKS)
  if(GLIBCXXPARALLEL_WORKS)
    set(GLIBCXXPARALLEL_DEFINITIONS -D_GLIBCXX_PARALLEL)    
  endif()

# handle the QUIETLY and REQUIRED arguments and set GLIBCXXPARALLEL_FOUND to TRUE if
# all listed variables are TRUE
  INCLUDE(FindPackageHandleStandardArgs)
  FIND_PACKAGE_HANDLE_STANDARD_ARGS(GLIBCXXPARALLEL DEFAULT_MSG GLIBCXXPARALLEL_CXX_FLAGS GLIBCXXPARALLEL_C_FLAGS GLIBCXXPARALLEL_WORKS GLIBCXXPARALLEL_DEFINITIONS)

  mark_as_advanced(GLIBCXXPARALLEL_FOUND GLIBCXXPARALLEL_CXX_FLAGS GLIBCXXPARALLEL_C_FLAGS GLIBCXXPARALLEL_WORKS GLIBCXXPARALLEL_DEFINITIONS)



endif()