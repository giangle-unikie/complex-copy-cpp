#include "lib/file-handler.h"
#include "lib/ipc-handler.h"
#include "lib/ipc.h"
#include "lib/ipc-pipe-receive.h"
#include "lib/ipc-pipe-send.h"
#include "gtest/gtest.h"
#include <sys/types.h>

TEST(pipeTest1, tranferFile_1Sender_2Receivers)
{
    optind = 0;

    char mode_s[] = {"testFile/ipc_send"};
    char mode_rc[] = {"testFile/ipc_receive"};
    char pipeMethod[] = {"-p"};
    char methodName[] = {"pipe_methodName"};
    char file[] = {"-f"};
    char receivefileName1[] = {"fileName1"};
    char receivefileName2[] = {"fileName2"};
    char sendFileName[] = {"testFile/test"};
    int argc = 5;
    char *argv1[] = {mode_s, pipeMethod, methodName, file, sendFileName};
    char *argv2[] = {mode_rc, pipeMethod, methodName, file, receivefileName1};
    char *argv3[] = {mode_rc, pipeMethod, methodName, file, receivefileName2};
    ipcHandler ipc;
    ipc_info info;
    FileHandler fileWrite1(receivefileName1, FileMode::WRITE);
    FileHandler fileWrite2(receivefileName2, FileMode::WRITE);
    FileHandler fileRead(sendFileName, FileMode::READ);

    pid_t child_receiver_1;
    pid_t child_receiver_2;

    child_receiver_1 = fork();
    if (child_receiver_1 == 0)
    { // This is the child_receiver_1 process.
        std::cout << "child 1 " << child_receiver_1 << std::endl;
        ipc.select_options(IPCMode::RECEIVE_MODE, argc, argv2);
        info = ipc.get_options();
        ipc.start();
    }
    else if (child_receiver_1 > 0)
    {
        child_receiver_2 = fork();

        if (child_receiver_2 == 0)
        { // This is the child_receiver_2 process.
            std::cout << "child 2 " << child_receiver_2 << std::endl;
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
            wait(NULL);
           
            if (fileWrite1.get_file_size() == 0)
            {
                EXPECT_EQ(fileWrite2.get_file_size(), fileRead.get_file_size());
            }
            else
            {
                EXPECT_EQ(fileWrite1.get_file_size(), fileRead.get_file_size());
            }
        }
    }
}

TEST(pipeTest2, tranferFile_2Sender_1Receivers)
{
    optind = 0;

    char mode_s[] = {"testFile/ipc_send"};
    char mode_rc[] = {"testFile/ipc_receive"};
    char pipeMethod[] = {"-p"};
    char methodName[] = {"pipe_methodName"};
    char file[] = {"-f"};
    char receivefileName[] = {"fileName1"};
    char sendFileName1[] = {"testFile/test_copy"};
    char sendFileName2[] = {"testFile/test"};
    int argc = 5;
    char *argv1[] = {mode_s, pipeMethod, methodName, file, sendFileName1};
    char *argv2[] = {mode_s, pipeMethod, methodName, file, sendFileName2};
    char *argv3[] = {mode_rc, pipeMethod, methodName, file, receivefileName};
    ipcHandler ipc;
    ipc_info info;
    FileHandler fileWrite(receivefileName, FileMode::WRITE);
    FileHandler fileRead1(sendFileName1, FileMode::READ);
    FileHandler fileRead2(sendFileName2, FileMode::READ);

    pid_t child_send_1;
    pid_t child_send_2;

    child_send_1 = fork();
    if (child_send_1 == 0)
    { // This is the child_receiver_1 process.
        std::cout << "child 1 " << child_send_1 << std::endl;
        ipc.select_options(IPCMode::SEND_MODE, argc, argv1);
        info = ipc.get_options();
        ipc.start();
    }
    else if (child_send_1 > 0)
    {
        child_send_2= fork();

        if (child_send_2 == 0)
        { // This is the child_receiver_2 process.
            std::cout << "child 2 " << child_send_2 << std::endl;
            ipc.select_options(IPCMode::SEND_MODE, argc, argv2);
            info = ipc.get_options();
            ipc.start();
        }
        else if (child_send_2 > 0)
        { // This is the father process.
            std::cout << "parent" << std::endl;
            ipc.select_options(IPCMode::RECEIVE_MODE, argc, argv3);
            info = ipc.get_options();
            ipc.start();
            wait(NULL);
            std::cout << "send test 1: " << fileRead1.get_file_size();
            std::cout << "send test 2: " << fileRead2.get_file_size();
            std::cout << "receiver test: " << fileWrite.get_file_size();
           
            EXPECT_TRUE((fileRead1.get_file_size() == fileWrite.get_file_size()) || (fileRead2.get_file_size() == fileWrite.get_file_size()));
        }
    }
}