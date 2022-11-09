cc_library(
    name = "ipc-common",
    hdrs = ["ipc-common.h"],
    visibility = ["//visibility:public"],
    deps = [],
)

cc_library(
    name = "ipc-handler",
    srcs = ["ipc-handler.cpp"],
    hdrs = ["ipc-handler.h"],
    visibility = ["//visibility:public"],
    deps = [
        "//lib:ipc-common",
        "//lib:ipc-queue-receive",
        "//lib:ipc-queue-send",
    ],
)

cc_library(
    name = "file-handler",
    srcs = ["file-handler.cpp"],
    hdrs = ["file-handler.h"],
    visibility = ["//visibility:public"],
    deps = [],
)

cc_library(
    name = "ipc-queue-send",
    srcs = ["ipc-queue-send.cpp"],
    hdrs = ["ipc-queue-send.h"],
    linkopts = ["-lrt"],
    visibility = ["//visibility:public"],
    deps = [
        "//lib:file-handler",
        "//lib:ipc-common",
    ],
)

cc_library(
    name = "ipc-queue-receive",
    srcs = ["ipc-queue-receive.cpp"],
    hdrs = ["ipc-queue-receive.h"],
    linkopts = ["-lrt"],
    visibility = ["//visibility:public"],
    deps = [
        "//lib:file-handler",
        "//lib:ipc-common",
       
    ],
)

