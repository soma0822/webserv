#include "IOTaskManager.hpp"

std::vector<std::vector<AIOTask *> > IOTaskManager::tasks_;
std::vector<struct pollfd> IOTaskManager::fds_;

IOTaskManager::IOTaskManager() {}

IOTaskManager::~IOTaskManager() {
  for (unsigned int i = 0; i < tasks_.size(); ++i) {
    for (unsigned int j = 0; j < tasks_.at(i).size(); ++j) {
      delete tasks_.at(i).at(j);
    }
  }
}

void IOTaskManager::AddTask(AIOTask *task) {
  for (unsigned int i = 0; i < fds_.size(); ++i) {
    if (fds_.at(i).fd == task->GetFd()) {
      tasks_.at(i).push_back(task);
      std::cout << "AddTask: " << task->GetFd() << task->GetEvent()
                << std::endl;
      return;
    }
  }
  struct pollfd fd = {task->GetFd(), POLLIN | POLLOUT, 0};
  std::vector<AIOTask *> tmp;
  tmp.push_back(task);
  tasks_.push_back(tmp);
  fds_.push_back(fd);
  std::cout << "AddTask: " << task->GetFd() << task->GetEvent() << std::endl;
}

void IOTaskManager::RemoveTask(AIOTask *task) {
  ReadRequestFromClient *tmp = dynamic_cast<ReadRequestFromClient *>(task);
  for (unsigned int i = 0; i < fds_.size(); ++i) {
    if (fds_.at(i).fd == task->GetFd()) {
      for (unsigned int j = 0; j < tasks_.at(i).size(); ++j) {
        if (tmp) {  // ReadRequestFromClientを消すときは接続を切るとき。同時にwriteも消すべき。それ以外のときはそのタスクのみを消す
          delete tasks_.at(i).at(j);
        } else if (tasks_.at(i).at(j) == task) {
          delete tasks_.at(i).at(j);
          tasks_.at(i).erase(tasks_.at(i).begin() + j);
        }
      }
      if (tmp) {  // 接続を切る
        close(fds_.at(i).fd);
        tasks_.erase(tasks_.begin() + i);
      }
      return;
    }
  }
}

// TODO:
// pollのエラーハンドリング。Executeの返り値について。Taskを消した時に接続を切る場合のiもどす必要ある。
void IOTaskManager::ExecuteTasks() {
  while (true) {
    int ret = poll(&fds_[0], fds_.size(), 5000);
    if (ret == -1) {
      std::cerr << "poll error" << std::endl;
      return;
    }
    for (unsigned int i = 0; i < fds_.size(); ++i) {
      for (unsigned int j = 0; j < tasks_.at(i).size(); ++j) {
        if (fds_.at(i).revents & tasks_.at(i).at(j)->GetEvent()) {
          Result<int, std::string> result = tasks_.at(i).at(j)->Execute();
          if (result
                  .IsErr())  // errを返すのは致命的なエラーのみの予定。他はOkの数値を見て判断
            throw std::invalid_argument("taskエラー");
          else if (result.Unwrap() == kDelete) {
            RemoveTask(tasks_.at(i).at(j));
            --j;
          }
        }
      }
    }
  }
}
