cc_library(
    name = 'structures',
    srcs = ['structures.cpp'],
    hdrs = ['structures.hpp'],
    includes = ['.'],
    copts = ["-std=c++11"],
)

cc_binary(
    name = 'unique_ptr',
    srcs = ['unique_ptr.cpp'],
    copts = ["-std=c++17"],
    deps = [
        'structures'
    ]
)
