config_setting(
    name = "windows",
    constraint_values = [
        "@platforms//os:windows",
    ],
)

config_setting(
    name = "linux",
    constraint_values = [
        "@platforms//os:linux",
    ],
)

config_setting(
    name = "macos",
    constraint_values = [
        "@platforms//os:macos",
    ],
)

config_setting(
    name = "freebsd",
    constraint_values = [
        "@platforms//os:freebsd",
    ],
)

config_setting(
    name = "openbsd",
    constraint_values = [
        "@platforms//os:openbsd",
    ],
)

cc_library(
    name = "tiny-process-library",
    srcs = [
        "process.hpp",
        "process.cpp"
    ] + select({
        ":windows": ["process_win.cpp"],
        ":linux": ["process_unix.cpp"],
        ":macos": ["process_unix.cpp"],
        ":freebsd": ["process_unix.cpp"],
        ":openbsd": ["process_unix.cpp"],
    }),
    hdrs = [
        "process.hpp",
    ],
    visibility = ["//visibility:public"],
)
