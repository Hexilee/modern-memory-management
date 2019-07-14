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

cc_binary(
    name = 'move',
    srcs = ['move.cpp'],
    copts = ["-std=c++17"],
    deps = [
        'structures'
    ]
)

cc_binary(
    name = 'vector',
    srcs = ["vector.cpp"],
    copts = ["-std=c++17"],
    deps = [
        'structures'
    ]
)

cc_binary(
    name = 'object_moved',
    srcs = ["object_moved.cpp"],
    copts = ["-std=c++17"],
    deps = [
        'structures'
    ]
)