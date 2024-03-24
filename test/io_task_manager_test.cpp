#include "io_task_manager.hpp"

#include <gtest/gtest.h>

TEST(IOTaskManager, DefaultTest) {
  Config conf;
  struct sockaddr_in client_addr;
  std::memset(&client_addr, 0, sizeof(client_addr));
  Accept *ac = new Accept(3, "8080", "", conf);
  IOTaskManager::AddTask(ac);
  ASSERT_EQ(1, IOTaskManager::GetTasks().size());
  ASSERT_EQ(1, IOTaskManager::GetFds().size());
  ReadRequestFromClient *req =
      new ReadRequestFromClient(4, "8080", "", client_addr, conf);
  IOTaskManager::AddTask(req);
  ASSERT_EQ(2, IOTaskManager::GetTasks().size());
  ASSERT_EQ(2, IOTaskManager::GetFds().size());
  WriteResponseToClient *res =
      new WriteResponseToClient(4, new HTTPResponse, new HTTPRequest);
  IOTaskManager::AddTask(res);
  ASSERT_EQ(2, IOTaskManager::GetTasks().size());
  ASSERT_EQ(2, IOTaskManager::GetTasks().at(1).tasks.size());
  ASSERT_EQ(2, IOTaskManager::GetFds().size());
  IOTaskManager::RemoveFd(req);
  ASSERT_EQ(2, IOTaskManager::GetTasks().size());
  ASSERT_EQ(2, IOTaskManager::GetFds().size());
  // ASSERT_EQ(0, IOTaskManager::GetTasks().at(0).size());
  ASSERT_EQ(-1, IOTaskManager::GetFds().at(1).fd);
  ASSERT_EQ(0, IOTaskManager::GetTasks().at(1).tasks.size());
  req = new ReadRequestFromClient(5, "8080", "", client_addr, conf);
  IOTaskManager::AddTask(req);
  ASSERT_EQ(2, IOTaskManager::GetTasks().size());
  ASSERT_EQ(2, IOTaskManager::GetFds().size());
  res = new WriteResponseToClient(5, new HTTPResponse, new HTTPRequest);
  ;
  IOTaskManager::AddTask(res);
  ASSERT_EQ(2, IOTaskManager::GetTasks().size());
  ASSERT_EQ(2, IOTaskManager::GetTasks().at(1).tasks.size());
  ASSERT_EQ(2, IOTaskManager::GetFds().size());
  ASSERT_EQ(5, IOTaskManager::GetFds().at(1).fd);
  IOTaskManager::DeleteTasks();
  ASSERT_EQ(0, IOTaskManager::GetFds().size());
  ASSERT_EQ(0, IOTaskManager::GetTasks().size());
  IOTaskManager::DeleteTasks();
}
