#include "lib/file-handler.h"
#include "lib/ipc-handler.h"
#include "lib/ipc.h"
#include "lib/ipc-queue-receive.h"
#include "gtest/gtest.h"
#include <boost/process.hpp>

TEST(QueueReceiveTest1, initFailOpen){
    optind =0;
    char mode[] = {"./ipc_send"};
    char queueMethod[] = {"-q"};
    char methodName[] = {"/methodName"};
    char file[] = {"-f"};
    char fileName[] = {"fileName"};
    int argc = 5;
    char* argv[] = {mode, queueMethod, methodName, file, fileName};
    ipcHandler ipc;
    ipc.select_options(IPCMode::SEND_MODE,argc, argv);
    ipc_info info = ipc.get_options();
    ipcQueueSend mq(info);
    
    mqd_t mqd{};
    mq_attr attr{0, 10, 8192, 0};
	mqd = mq_open(info.method_name,  O_RDONLY | O_NONBLOCK, 0660, &(attr));

	if (mqd == -1){
        EXPECT_THROW(mq.init(), std::runtime_error);
    }
}

TEST(QueueReceiveTest2, initSuccessOpen){
    optind =0;
    char mode[] = {"./ipc_receive"};
    char queueMethod[] = {"-q"};
    char methodName[] = {"/methodName"};
    char file[] = {"-f"};
    char fileName[] = {"fileName"};
    int argc = 5;
    char* argv[] = {mode, queueMethod, methodName, file, fileName};
    ipcHandler ipc;
    ipc.select_options(IPCMode::SEND_MODE,argc, argv);
    ipc_info info = ipc.get_options();
    ipcQueueSend mq(info);
    
    mqd_t mqd{};
    mq_attr attr{0, 10, 8192, 0};
	mqd = mq_open(info.method_name, O_RDONLY | O_NONBLOCK, 0660, &(attr));

	if (mqd != -1){
        EXPECT_NO_THROW(mq.init());
    }
}

TEST(QueueReceiveTest3, tranferFile){
    optind =0;
    boost::process::child c;

    char mode[] = {"./ipc_receive"};
    char queueMethod[] = {"-q"};
    char methodName[] = {"/methodName"};
    char file[] = {"-f"};
    char receivefileName[] = {"fileName"};
    char sendFileName[] = {"testFile/test"};
    int argc = 5;
    char* argv1[] = {mode, queueMethod, methodName, file, receivefileName};
    ipcHandler ipc;
    ipc_info info;
    ipc.select_options(IPCMode::RECEIVE_MODE,argc, argv1);
    info = ipc.get_options();
    FileHandler fileWrite(receivefileName, FileMode::WRITE);
    FileHandler fileRead(sendFileName, FileMode::READ);

    if(info.mode == IPCMode::RECEIVE_MODE && info.protocol == IPCProtocol::QUEUE){
        c = boost::process::child("testFile/ipc_send", "-q", "/methodName", "-f", "testFile/test");

    }

    ipc.start();
    c.wait();
    EXPECT_EQ(fileWrite.get_file_size(), fileRead.get_file_size());
}