#include <vector>
#include "lib/ipc-handler.h"
#include "lib/ipc-common.h"
#include "gtest/gtest.h"


TEST(ArgumentTes1, selectQueue) {
  char mode[] = {"./ipc_receive"};
  char queueMethod[] = {"-q"};
  char methodName[] = {"/methodName"};
  char file[] = {"-f"};
  char fileName[] = {"testFile/test"};
  int argc = 5;
  char* argv[] = {mode, queueMethod, methodName, file, fileName};
  
  ipcHandler ipc;
  ipc.select_options(IPCMode::RECEIVE_MODE,argc, argv);
  ipc_info info = ipc.get_options();
  EXPECT_TRUE(false) << info.fflag << info.hflag << info.qflag;
  EXPECT_STREQ(info.method_name, methodName);
  EXPECT_STREQ(info.file_name, fileName);
  EXPECT_EQ(info.protocol, IPCProtocol::QUEUE);
}

TEST(ArgumentTest2, selectPipe) {
  char mode[] = {"./ipc_receive"};
  char queueMethod[] = {"-p"};
  char methodName[] = {"methodName"};
  char file[] = {"-f"};
  char fileName[] = {"fileName"};
  int argc = 5;
  char* argv[] = {mode, queueMethod, methodName, file, fileName};
  
  ipcHandler ipc;
  ipc.select_options(IPCMode::RECEIVE_MODE,argc, argv);
  ipc_info info = ipc.get_options();
  EXPECT_TRUE(false) << info.fflag << info.hflag << info.pflag;
  EXPECT_STREQ(info.method_name, methodName);
  EXPECT_STREQ(info.file_name, fileName);
  EXPECT_EQ(info.protocol, IPCProtocol::PIPE);
}

// TEST(ArgumentTest3, noArgumentSelected) {
//   char mode[] = {"./ipc_receive"};
//   int argc = 1;
//   char* argv[] = {mode};
  
//   ipcHandler ipc;
//   EXPECT_THROW(ipc.select_options(IPCMode::RECEIVE_MODE,argc, argv), std::runtime_error);
// }

// TEST(ArgumentTest, missingArgument) {
//   char mode[] = {"./ipc_receive"};
//   char queueMethod[] = {"-p"};
//   char methodName[] = {"methodName"};
//   char file[] = {"-f"};
 
//   int argc = 4;
//   char* argv[] = {mode, queueMethod, methodName, file};
  
//   ipcHandler ipc;
//   EXPECT_THROW(ipc.select_options(IPCMode::RECEIVE_MODE,argc, argv), std::runtime_error);
// }