cc_test(
  name = "file-test",
  size = "small",
  srcs = ["file-test.cpp"],
  data = ["//testFile"],
  deps = ["//lib:file-handler",
          "@com_google_googletest//:gtest_main"],
)

cc_test(
  name = "argument-test",
  srcs = ["argument-test.cpp"],
  deps = ["//lib:ipc-handler",
          "@com_google_googletest//:gtest_main"],
)

cc_test(
  name = "queue-send-test",
  srcs = ["queue-send-test.cpp"],
  linkopts = ["-lpthread"],
  data = ["//testFile"],
  deps = ["//lib:file-handler",
          "//lib:ipc-handler",
          "//lib:ipc-queue-send",
          "@com_google_googletest//:gtest_main"],
)

cc_test(
  name = "queue-receive-test",
  srcs = ["queue-receive-test.cpp"],
  linkopts = ["-lpthread"],
  data = ["//testFile"],
  deps = ["//lib:file-handler",
          "//lib:ipc-handler",
          "//lib:ipc-queue-receive",
          "@com_google_googletest//:gtest_main"],
)

cc_test(
  name = "pipe-test",
  srcs = ["pipe-test.cpp"],
  linkopts = ["-lpthread"],
  data = ["//testFile"],
  deps = ["//lib:file-handler",
          "//lib:ipc-handler",
          "//lib:ipc-pipe-send",
          "//lib:ipc-pipe-receive",
          "@com_google_googletest//:gtest_main"],
)

cc_test(
  name = "shm-test",
  srcs = ["shm-test.cpp"],
  linkopts = ["-lpthread"],
  data = ["//testFile"],
  deps = ["//lib:file-handler",
          "//lib:ipc-handler",
          "//lib:ipc-shm",
          "//lib:ipc-shm-send",
          "//lib:ipc-shm-receive",
          "@com_google_googletest//:gtest_main"],
)

cc_test(
  name = "senders-receivers",
  srcs = ["senders-receivers.cpp"],
  linkopts = ["-lpthread"],
  data = ["//testFile"],
  deps = ["//lib:file-handler",
          "//lib:ipc-handler",
          "//lib:ipc-pipe-send",
          "//lib:ipc-pipe-receive",
          "@com_google_googletest//:gtest_main"],
)