load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")


def rpi_bazel_repository(name):
    commit = "964b6feb8bb14b2a58876b406f17266538794c3a"
    http_archive(
        name = name,
        url = "https://github.com/mjbots/rpi_bazel/archive/{}.zip".format(commit),
        strip_prefix = "rpi_bazel-{}".format(commit),
    )