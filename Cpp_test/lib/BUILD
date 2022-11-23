cc_library(
    name = "ipc",
    hdrs = ["ipc.h"],
    visibility = ["//visibility:public"],
    deps = ["//lib:file-handler"],
)

cc_library(
    name = "ipc-handler",
    srcs = ["ipc-handler.cpp"],
    hdrs = ["ipc-handler.h"],
    visibility = ["//visibility:public"],
    deps = ["//lib:ipc-queue-receive", "//lib:ipc-queue-send"],
)

cc_library(
    name = "file-handler",
    srcs = ["file-handler.cpp"],
    hdrs = ["file-handler.h"],
    linkopts = ["-lboost_filesystem"],
    visibility = ["//visibility:public"],
    deps = [],
)

cc_library(
    name = "ipc-queue-send",
    srcs = ["ipc-queue-send.cpp"],
    hdrs = ["ipc-queue-send.h"],
    linkopts = ["-lrt"],
    visibility = ["//visibility:public"],
    deps = ["//lib:ipc",],
)

cc_library(
    name = "ipc-queue-receive",
    srcs = ["ipc-queue-receive.cpp"],
    hdrs = ["ipc-queue-receive.h"],
    linkopts = ["-lrt"],
    visibility = ["//visibility:public"],
    deps = ["//lib:ipc"],
)



