cc_library(
    name = 'structures',
    srcs = ['structures.cpp'],
    hdrs = ['structures.hpp'],
    includes = ['.'],
    copts = ["-std=c++11"],
)

cc_binary(
    name = 'lvalue_ref',
    srcs = ['lvalue_ref.cpp'],
    copts = ["-std=c++11"],
    deps = [
        'structures'
    ]
)

cc_binary(
    name = 'lvalue_ref_bind',
    srcs = ['lvalue_ref_bind.cpp'],
    copts = ["-std=c++11"],
    deps = [
        'structures'
    ]
)

cc_binary(
    name = 'lvalue_ref_bind_cpp14',
    srcs = ['lvalue_ref_bind_cpp14.cpp'],
    copts = ["-std=c++14"],
    deps = [
        'structures'
    ]
)