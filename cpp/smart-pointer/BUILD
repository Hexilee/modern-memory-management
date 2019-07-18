cc_library(
    name = 'structures',
    srcs = ['structures.cpp'],
    hdrs = ['structures.hpp'],
    includes = ['.'],
    copts = ["-std=c++17"],
)

cc_binary(
    name = 'unique_ptr',
    srcs = ['unique_ptr.cpp'],
    copts = ["-std=c++17"],
    deps = [
        'structures'
    ]
)

cc_binary(
    name = 'shared_ptr',
    srcs = ['shared_ptr.cpp'],
    copts = ["-std=c++17"],
    deps = [
        'structures'
    ]

)

cc_binary(
    name = 'circular_ref',
    srcs = ['circular_ref.cpp'],
    copts = ["-std=c++17"],
)

cc_binary(
    name = 'weak_ptr',
    srcs = ['weak_ptr.cpp'],
    copts = ["-std=c++17"],
)
