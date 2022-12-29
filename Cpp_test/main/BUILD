cc_binary(
    name = "ipc_receive",
    srcs = ["ipc_receive.cpp"],
    deps = ["//lib:ipc-handler","@org_llvm_libcxx//:libcxx"],
    linkstatic = 1,
)

cc_binary(
    name = "ipc_send",
    srcs = ["ipc_send.cpp"],
    deps = ["//lib:ipc-handler","@org_llvm_libcxx//:libcxx"],
    linkstatic = 1,
)
