cc_library(
    name = "ipc",
    hdrs = ["ipc.h"],
    visibility = ["//visibility:public"],
    deps = [
        "//lib:file-handler"
    ],
)

cc_library(
    name = "ipc-handler",
    srcs = ["ipc-handler.cpp"],
    hdrs = ["ipc-handler.h"],
    visibility = ["//visibility:public"],
    deps = [
        "//lib:ipc-queue-receive", 
        "//lib:ipc-queue-send",
        "//lib:ipc-pipe-receive", 
        "//lib:ipc-pipe-send",
        "//lib:ipc-shm-receive", 
        "//lib:ipc-shm-send"
    ],
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
    deps = [
        "//lib:ipc",
    ],
)

cc_library(
    name = "ipc-queue-receive",
    srcs = ["ipc-queue-receive.cpp"],
    hdrs = ["ipc-queue-receive.h"],
    linkopts = ["-lrt"],
    visibility = ["//visibility:public"],
    deps = [
        "//lib:ipc"
    ],
)

cc_library(
    name = "ipc-pipe-send",
    srcs = ["ipc-pipe-send.cpp"],
    hdrs = ["ipc-pipe-send.h"],
    linkopts = ["-lrt"],
    visibility = ["//visibility:public"],
    deps = [
        "//lib:ipc",
    ],
)

cc_library(
    name = "ipc-pipe-receive",
    srcs = ["ipc-pipe-receive.cpp"],
    hdrs = ["ipc-pipe-receive.h"],
    linkopts = ["-lrt"],
    visibility = ["//visibility:public"],
    deps = [
        "//lib:ipc",
    ],
)

cc_library(
    name = "ipc-shm",
    srcs = ["ipc-shm.cpp"],
    hdrs = ["ipc-shm.h"],
    linkopts = ["-lpthread"],
    visibility = ["//visibility:public"],
)

cc_library(
    name = "ipc-shm-send",
    srcs = ["ipc-shm-send.cpp"],
    hdrs = ["ipc-shm-send.h"],
    linkopts = ["-lpthread"],
    visibility = ["//visibility:public"],
    deps = [
        "//lib:ipc",
        "//lib:ipc-shm",
    ],
)

cc_library(
    name = "ipc-shm-receive",
    srcs = ["ipc-shm-receive.cpp"],
    hdrs = ["ipc-shm-receive.h"],
    linkopts = ["-lpthread"],
    visibility = ["//visibility:public"],
    deps = [
        "//lib:ipc",
        "//lib:ipc-shm",
    ],
)
