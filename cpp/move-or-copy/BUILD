cc_library(
    name = 'structures',
    srcs = ['structures.cpp'],
    hdrs = ['structures.hpp'],
    includes = ['.'],
    copts = ["-std=c++11"],
)

cc_binary(
    name = 'copy',
    srcs = ['copy.cpp'],
    copts = ["-std=c++17"],
    deps = [
        'structures'
    ]
)