bazel clean
rm -rf build
rm -f testFile/ipc_send
rm -f testFile/ipc_receive

echo "Building the program"
bazel build ...

cp bazel-bin/main/ipc_send testFile
cp bazel-bin/main/ipc_receive testFile

echo "Running the tests"

# bazel test //test:argument-test
# bazel test //test:file-test
# bazel test //test:queue-receive-test
# bazel test //test:queue-send-test
# bazel test //test:pipe-test
# bazel test //test:shm-test
bazel test //test:senders-receivers

mkdir build

# cp bazel-testlogs/test/argument-test/test.log build/argument-test.log
# cp bazel-testlogs/test/file-test/test.log build/file-test.log
# cp bazel-testlogs/test/queue-receive-test/test.log build/queue-receive-test.log
# cp bazel-testlogs/test/queue-send-test/test.log build/queue-send-test.log
# cp bazel-testlogs/test/pipe-test/test.log build/pipe-test.log
# cp bazel-testlogs/test/shm-test/test.log build/shm-test.log
cp bazel-testlogs/test/senders-receivers/test.log build/senders-receivers.log