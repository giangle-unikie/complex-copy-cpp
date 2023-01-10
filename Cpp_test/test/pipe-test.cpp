#include "lib/file-handler.h"
#include "lib/ipc-handler.h"
#include "lib/ipc.h"
#include "lib/ipc-pipe-receive.h"
#include "lib/ipc-pipe-send.h"
#include "gtest/gtest.h"
#include <sys/types.h>

TEST(PipeReceiveTest1, initFailOpen)
{
    optind = 0;
    char mode[] = {"./ipc_receive"};
    char pipeMethod[] = {"-p"};
    char methodName[] = {"methodName"};
    char file[] = {"-f"};
    char fileName[] = {"fileName"};
    int argc = 5;
    char *argv[] = {mode, pipeMethod, methodName, file, fileName};
    ipcHandler ipc;
    ipc.select_options(IPCMode::SEND_MODE, argc, argv);
    ipc_info info = ipc.get_options();
    IPCPipeSend pipe(info);

    int pd{0};
    pd = open(info.method_name, O_RDONLY | O_NONBLOCK);

    if (pd == -1)
    {
        EXPECT_THROW(pipe.init(), std::runtime_error);
    }
}

TEST(PipeReceiveTest2, initSuccessOpen)
{
    optind = 0;
    char mode[] = {"./ipc_receive"};
    char pipeMethod[] = {"-p"};
    char methodName[] = {"methodName"};
    char file[] = {"-f"};
    char fileName[] = {"fileName"};
    int argc = 5;
    char *argv[] = {mode, pipeMethod, methodName, file, fileName};
    ipcHandler ipc;
    ipc.select_options(IPCMode::SEND_MODE, argc, argv);
    ipc_info info = ipc.get_options();
    IPCPipeSend pipe(info);

    int pd{0};
    pd = open(info.method_name, O_RDONLY | O_NONBLOCK);

    if (pd != -1)
    {
        EXPECT_NO_THROW(pipe.init());
    }
}

TEST(PipeReceiveTest3, start)
{
    optind = 0;
    char mode[] = {"./ipc_receive"};
    char pipeMethod[] = {"-p"};
    char methodName[] = {"methodName"};
    char file[] = {"-f"};
    char fileName[] = {"fileName"};
    int argc = 5;
    char *argv[] = {mode, pipeMethod, methodName, file, fileName};
    ipcHandler ipc;
    ipc_info info;

    if (info.mode == IPCMode::RECEIVE_MODE && info.protocol == IPCProtocol::PIPE)
    {
        ipc.select_options(IPCMode::RECEIVE_MODE, argc, argv);
        ipc.get_options();
        EXPECT_NO_THROW(ipc.start());
    }
}

TEST(PipeSendTest4, initFailOpen)
{
    optind = 0;
    char mode[] = {"./ipc_send"};
    char pipeMethod[] = {"-p"};
    char methodName[] = {"methodName"};
    char file[] = {"-f"};
    char fileName[] = {"testFile/test"};
    int argc = 5;
    char *argv[] = {mode, pipeMethod, methodName, file, fileName};
    ipcHandler ipc;
    ipc.select_options(IPCMode::SEND_MODE, argc, argv);
    ipc_info info = ipc.get_options();
    IPCPipeSend pipe(info);

    int pd{0};
    pd = open(info.method_name, O_RDONLY | O_NONBLOCK);

    if (pd == -1)
    {
        EXPECT_THROW(pipe.init(), std::runtime_error);
    }
}

TEST(PipeSendTest5, initSuccessOpen)
{
    optind = 0;
    char mode[] = {"./ipc_send"};
    char pipeMethod[] = {"-p"};
    char methodName[] = {"methodName"};
    char file[] = {"-f"};
    char fileName[] = {"testFile/test"};
    int argc = 5;
    char *argv[] = {mode, pipeMethod, methodName, file, fileName};
    ipcHandler ipc;
    ipc.select_options(IPCMode::SEND_MODE, argc, argv);
    ipc_info info = ipc.get_options();
    IPCPipeSend pipe(info);

    int pd{0};
    pd = open(info.method_name, O_RDONLY | O_NONBLOCK);

    if (pd != -1)
    {
        EXPECT_NO_THROW(pipe.init());
    }
}

TEST(PipeSendTest6, start)
{
    optind = 0;
    char mode[] = {"./ipc_send"};
    char pipeMethod[] = {"-p"};
    char methodName[] = {"methodName"};
    char file[] = {"-f"};
    char fileName[] = {"testFile/test"};
    int argc = 5;
    char *argv[] = {mode, pipeMethod, methodName, file, fileName};
    ipcHandler ipc;
    ipc_info info;
    FileHandler fileRead(fileName, FileMode::READ);

    if (info.mode == IPCMode::SEND_MODE && fileRead.get_file_size() == 0)
    {
        ipc.select_options(IPCMode::SEND_MODE, argc, argv);
        ipc.get_options();
        EXPECT_THROW(ipc.start(), std::runtime_error);
    }
    else if (info.mode == IPCMode::SEND_MODE && info.protocol == IPCProtocol::PIPE)
    {
        ipc.select_options(IPCMode::SEND_MODE, argc, argv);
        ipc.get_options();
        EXPECT_NO_THROW(ipc.start());
    }
}

TEST(pipeSendTest7, tranferFile)
{
    optind = 0;
    pid_t pid;

    char mode1[] = {"./ipc_send"};
    char mode2[] = {"testFile/ipc_receive"};
    char pipeMethod[] = {"-p"};
    char methodName[] = {"methodName"};
    char file[] = {"-f"};
    char receivefileName[] = {"fileName"};
    char sendFileName[] = {"testFile/test"};
    int argc = 5;
    char *argv1[] = {mode1, pipeMethod, methodName, file, sendFileName};
    char *argv2[] = {mode2, pipeMethod, methodName, file, receivefileName};
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
    fileWrite.close_file();
    std::cout << "send :" << fileRead.get_file_size() << std::endl;
    std::cout << "receive :" << fileWrite.get_file_size() << std::endl;

    EXPECT_EQ(fileWrite.get_file_size(), fileRead.get_file_size());
}

TEST(pipeSendTest8, tranferFile_1Sender_2Receivers)
{
    optind = 0;

    char mode1[] = {"./ipc_send"};
    char mode2[] = {"testFile/ipc_receive"};
    char pipeMethod[] = {"-p"};
    char methodName[] = {"pipe_methodName"};
    char file[] = {"-f"};
    char receivefileName1[] = {"fileName1"};
    char receivefileName2[] = {"fileName2"};
    char sendFileName[] = {"testFile/test"};
    int argc = 5;
    char *argv1[] = {mode1, pipeMethod, methodName, file, sendFileName};
    char *argv2[] = {mode2, pipeMethod, methodName, file, receivefileName1};
    char *argv3[] = {mode2, pipeMethod, methodName, file, receivefileName2};
    ipcHandler ipc;
    ipc_info info;
    FileHandler fileWrite1(receivefileName1, FileMode::WRITE);
    FileHandler fileWrite2(receivefileName2, FileMode::WRITE);
    FileHandler fileRead(sendFileName, FileMode::READ);

    pid_t child_receiver_1;

    child_receiver_1 = fork();

    if (child_receiver_1 == 0)
    { // This is the child_receiver_1 process.
        std::cout << "child 1" << std::endl;
        ipc.select_options(IPCMode::RECEIVE_MODE, argc, argv2);
        info = ipc.get_options();
        ipc.start();
    }
    else if (child_receiver_1 > 0)
    {
        pid_t child_receiver_2 = fork();

        if (child_receiver_2 == 0)
        { // This is the child_receiver_2 process.
            std::cout << "child 2" << std::endl;
            ipc.select_options(IPCMode::RECEIVE_MODE, argc, argv3);
            info = ipc.get_options();
            ipc.start();
        }
        else if (child_receiver_2 > 0)
        { // This is the father process.
            std::cout << "parent" << std::endl;
            ipc.select_options(IPCMode::SEND_MODE, argc, argv1);
            info = ipc.get_options();
            ipc.start();
        }
    }

    std::cout << "send :" << fileRead.get_file_size() << std::endl;
    std::cout << "receive 1 :" << fileWrite1.get_file_size() << std::endl;
    std::cout << "receive 2:" << fileWrite2.get_file_size() << std::endl;
    if (fileWrite1.get_file_size() == 0)
    {
        EXPECT_EQ(fileWrite2.get_file_size(), fileRead.get_file_size());
    }
    else if (fileWrite2.get_file_size() == 0)
    {
        EXPECT_EQ(fileWrite1.get_file_size(), fileRead.get_file_size());
    }
}
