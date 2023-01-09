#include "lib/file-handler.h"
#include "lib/ipc-handler.h"
#include "lib/ipc.h"
#include "lib/ipc-shm.h"
#include "lib/ipc-shm-receive.h"
#include "lib/ipc-shm-send.h"
#include "gtest/gtest.h"
#include <sys/types.h>

TEST(shmSendTest1, initFailOpen_InvalidMethodname)
{
    optind = 0;
    char mode[] = {"./ipc_send"};
    char shmMethod[] = {"-s"};
    char methodName[] = {""};
    char file[] = {"-f"};
    char fileName[] = {"testFile/test"};
    int argc = 5;
    char *argv[] = {mode, shmMethod, methodName, file, fileName};
    ipcHandler ipc;
    ipc.select_options(IPCMode::SEND_MODE, argc, argv);
    ipc_info info = ipc.get_options();
    IPCShmSend shm(info);
    EXPECT_THROW(shm.open_shm(), std::runtime_error);
}

TEST(shmSendTest2, initSuccessOpen)
{
    optind = 0;
    char mode[] = {"./ipc_send"};
    char shmMethod[] = {"-s"};
    char methodName[] = {"/methodName"};
    char file[] = {"-f"};
    char fileName[] = {"testFile/test"};
    int argc = 5;
    char *argv[] = {mode, shmMethod, methodName, file, fileName};
    ipcHandler ipc;
    ipc.select_options(IPCMode::SEND_MODE, argc, argv);
    ipc_info info = ipc.get_options();
    IPCShmSend shm(info);
    EXPECT_NO_THROW(shm.open_shm());
}

TEST(shmSendTest3, start_sendMode)
{
    optind = 0;
    char mode[] = {"./ipc_send"};
    char shmMethod[] = {"-s"};
    char methodName[] = {"/methodName"};
    char file[] = {"-f"};
    char fileName[] = {"testFile/test"};
    int argc = 5;
    char *argv[] = {mode, shmMethod, methodName, file, fileName};
    ipcHandler ipc;
    ipc_info info;
    FileHandler fileRead(fileName, FileMode::READ);

    if (info.mode == IPCMode::SEND_MODE && fileRead.get_file_size() == 0)
    {
        ipc.select_options(IPCMode::SEND_MODE, argc, argv);
        ipc.get_options();
        EXPECT_THROW(ipc.start(), std::runtime_error);
    }
    else if (info.mode == IPCMode::SEND_MODE && info.protocol == IPCProtocol::SHARE)
    {
        ipc.select_options(IPCMode::SEND_MODE, argc, argv);
        ipc.get_options();
        EXPECT_NO_THROW(ipc.start());
    }
}

TEST(shmSendTest4, tranferFile)
{
    optind = 0;
    pid_t pid;

    char mode1[] = {"./ipc_send"};
    char mode2[] = {"testFile/ipc_receive"};
    char sharedMethod[] = {"-s"};
    char methodName[] = {"methodName"};
    char file[] = {"-f"};
    char receivefileName[] = {"fileName"};
    char sendFileName[] = {"testFile/test"};
    int argc = 5;
    char *argv1[] = {mode1, sharedMethod, methodName, file, sendFileName};
    char *argv2[] = {mode2, sharedMethod, methodName, file, receivefileName};
    ipcHandler ipc;
    ipc_info info;
    FileHandler fileWrite(receivefileName, FileMode::WRITE);
    FileHandler fileRead(sendFileName, FileMode::READ);

    pid = fork();
    if (pid == 0)
    {
        // child process
        std::cout << "child" << std::endl;
        ipc.select_options(IPCMode::RECEIVE_MODE, argc, argv2);
        info = ipc.get_options();
        ipc.start();
    }
    else
    {
        // parent process
        std::cout << "parent" << std::endl;
        ipc.select_options(IPCMode::SEND_MODE, argc, argv1);
        info = ipc.get_options();
        ipc.start();
    }

    wait(&pid);

    EXPECT_EQ(fileWrite.get_file_size(), fileRead.get_file_size());
}

TEST(shmReceiveTest5, startReceiveFail_TimeWait10s)
{
    optind = 0;
    char mode[] = {"./ipc_receive"};
    char shmMethod[] = {"-s"};
    char methodName[] = {"/methodName"};
    char file[] = {"-f"};
    char fileName[] = {"filename"};
    int argc = 5;
    char *argv[] = {mode, shmMethod, methodName, file, fileName};
    ipcHandler ipc;
    ipc_info info;
    ipc.select_options(IPCMode::RECEIVE_MODE, argc, argv);
    ipc.get_options();
    if (info.mode == IPCMode::RECEIVE_MODE && info.protocol == IPCProtocol::SHARE)
    {
        EXPECT_THROW(ipc.start(), std::runtime_error);
    }
}

TEST(shmSendTest6, tranferFile2)
{
    optind = 0;
    pid_t pid;

    char mode1[] = {"./ipc_send"};
    char mode2[] = {"testFile/ipc_receive"};
    char sharedMethod[] = {"-s"};
    char methodName[] = {"methodName"};
    char file[] = {"-f"};
    char receivefileName[] = {"fileName"};
    char sendFileName[] = {"testFile/test"};
    int argc = 5;
    char *argv1[] = {mode1, sharedMethod, methodName, file, sendFileName};
    char *argv2[] = {mode2, sharedMethod, methodName, file, receivefileName};
    ipcHandler ipc;
    ipc_info info;
    FileHandler fileWrite(receivefileName, FileMode::WRITE);
    FileHandler fileRead(sendFileName, FileMode::READ);

    pid = fork();
    if (pid == 0)
    {
        // child process
        std::cout << "parent" << std::endl;
        ipc.select_options(IPCMode::SEND_MODE, argc, argv1);
        info = ipc.get_options();
        ipc.start();
    }
    else
    {
        // parent process

        std::cout << "child" << std::endl;
        ipc.select_options(IPCMode::RECEIVE_MODE, argc, argv2);
        info = ipc.get_options();
        ipc.start();
    }

    wait(&pid);

    EXPECT_EQ(fileWrite.get_file_size(), fileRead.get_file_size());
}
