#include "lib/file-handler.h"
#include "lib/ipc-handler.h"
#include "lib/ipc-common.h"
#include "lib/ipc-queue-send.h"
#include "gtest/gtest.h"

// TEST(QueueSendTest1, initFailOpen){
//     char mode[] = {"./ipc_send"};
//     char queueMethod[] = {"-q"};
//     char methodName[] = {"/methodName"};
//     char file[] = {"-f"};
//     char fileName[] = {"testFile/test"};
//     int argc = 5;
//     char* argv[] = {mode, queueMethod, methodName, file, fileName};
//     ipcHandler ipc;
//     ipc.select_options(IPCMode::SEND_MODE,argc, argv);
//     ipc_info info = ipc.get_options();
//     ipcQueueSend mq(info);
    
//     mqd_t mqd{};
//     mq_attr attr{0, 10, 8192, 0};
// 	mqd = mq_open(info.method_name, O_CREAT | O_EXCL | O_WRONLY | O_NONBLOCK, 0660, &(attr));

// 	if (mqd == -1){
//         EXPECT_THROW(mq.init(), std::runtime_error);
//     }
// }

// TEST(QueueSendTest2, initSuccessOpen){
//     char mode[] = {"./ipc_send"};
//     char queueMethod[] = {"-q"};
//     char methodName[] = {"/methodName"};
//     char file[] = {"-f"};
//     char fileName[] = {"testFile/test"};
//     int argc = 5;
//     char* argv[] = {mode, queueMethod, methodName, file, fileName};
//     ipcHandler ipc;
//     ipc.select_options(IPCMode::SEND_MODE,argc, argv);
//     ipc_info info = ipc.get_options();
//     ipcQueueSend mq(info);
    
//     mqd_t mqd{};
//     mq_attr attr{0, 10, 8192, 0};
// 	mqd = mq_open(info.method_name, O_CREAT | O_EXCL | O_WRONLY | O_NONBLOCK, 0660, &(attr));

// 	if (mqd != -1){
//         EXPECT_NO_THROW(mq.init());
//     }
// }

// TEST(QueueSendTest3, start){
//     char mode[] = {"./ipc_send"};
//     char queueMethod[] = {"-q"};
//     char methodName[] = {"/methodName"};
//     char file[] = {"-f"};
//     char fileName[] = {"testFile/test"};
//     int argc = 5;
//     char* argv[] = {mode, queueMethod, methodName, file, fileName};
//     ipcHandler ipc;
//     ipc_info info;
//     FileHandler fileRead(fileName, FileMode::READ);
    
//     if(info.mode == IPCMode::SEND_MODE && fileRead.get_file_size() == 0){
//         ipc.select_options(IPCMode::SEND_MODE,argc, argv);
//         ipc.get_options();
//         EXPECT_THROW(ipc.start(), std::runtime_error);
//     } else if(info.mode == IPCMode::SEND_MODE && info.protocol == IPCProtocol::QUEUE){
//         ipc.select_options(IPCMode::SEND_MODE,argc, argv);
//         ipc.get_options();
//         EXPECT_NO_THROW(ipc.start());
//     }
// }

TEST(QueueSendTest4, start){
    char mode[] = {"./ipc_send"};
    char queueMethod[] = {"-q"};
    char methodName[] = {"/methodName"};
    char file[] = {"-f"};
    char fileName[] = {"testFile/test"};
    int argc = 5;
    char* argv[] = {mode, queueMethod, methodName, file, fileName};
    ipcHandler ipc;
    ipc_info info;
    FileHandler fileRead(fileName, FileMode::READ);
    
    if(info.mode == IPCMode::SEND_MODE && fileRead.get_file_size() == 0){
        ipc.select_options(IPCMode::SEND_MODE,argc, argv);
        ipc.get_options();
        EXPECT_THROW(ipc.start(), std::runtime_error);
    } else if(info.mode == IPCMode::SEND_MODE && info.protocol == IPCProtocol::QUEUE){
        ipc.select_options(IPCMode::SEND_MODE,argc, argv);
        ipc.get_options();
        EXPECT_NO_THROW(ipc.start());
    }
}