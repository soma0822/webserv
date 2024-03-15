#include "io_task_manager.hpp"

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

const std::vector<std::vector<AIOTask *> > &IOTaskManager::GetTasks() {
  return tasks_;
}
const std::vector<struct pollfd> &IOTaskManager::GetFds() { return fds_; }

void IOTaskManager::AddTask(AIOTask *task) {
  for (unsigned int i = 0; i < fds_.size(); ++i) {
    if (fds_.at(i).fd == task->GetFd()) {
      std::vector<AIOTask *>::iterator it =
          std::find(tasks_.at(i).begin(), tasks_.at(i).end(), (AIOTask *)NULL);
      if (it != tasks_.at(i).end())
        *it = task;
      else
        tasks_.at(i).push_back(task);
      return;
    }
  }
  struct pollfd fd = {task->GetFd(), POLLIN | POLLOUT, 0};
  for (unsigned int i = 0; i < fds_.size(); ++i) {
    if (fds_.at(i).fd == -1) {
      fds_.at(i) = fd;
      tasks_.at(i).push_back(task);
      return;
    }
  }
  std::vector<AIOTask *> tmp;
  tmp.push_back(task);
  tasks_.push_back(tmp);
  fds_.push_back(fd);
}

void IOTaskManager::RemoveReadTask(AIOTask *task) {
  ReadRequestFromClient *tmp = dynamic_cast<ReadRequestFromClient *>(task);
  if (!tmp) return;
  for (unsigned int i = 0; i < fds_.size(); ++i) {
    if (fds_.at(i).fd == task->GetFd()) {
      for (unsigned int j = 0; j < tasks_.at(i).size(); ++j) {
        delete tasks_.at(i).at(j);
      }
      close(fds_.at(i).fd);
      tasks_.at(i).clear();
      fds_.at(i).fd = -1;
      Logger::Info() << "接続を削除しました" << std::endl;
      return;
    }
  }
}

void IOTaskManager::DeleteTasks() {
  for (unsigned int i = 0; i < tasks_.size(); i++) {
    for (unsigned int j = 0; j < tasks_.at(i).size(); j++) {
      if (tasks_.at(i).at(j) != NULL) delete tasks_.at(i).at(j);
    }
  }
  tasks_.clear();
  fds_.clear();
}

void IOTaskManager::RemoveWriteTask(AIOTask *task) {
  WriteResponseToClient *tmp = dynamic_cast<WriteResponseToClient *>(task);
  if (!tmp) return;
  for (unsigned int i = 0; i < fds_.size(); ++i) {
    if (fds_.at(i).fd == task->GetFd()) {
      *(std::find(tasks_.at(i).begin(), tasks_.at(i).end(), task)) = NULL;
      delete task;
      Logger::Info() << "ライトタスクを削除しました" << std::endl;
      return;
    }
  }
}

void IOTaskManager::ExecuteTasks() {
  while (true) {
    int ret = poll(&fds_[0], fds_.size(), poll_time_out_);
    if (ret == -1) {
      std::cerr << "poll error" << std::endl;
      return;
    }
    for (unsigned int i = 0; i < fds_.size(); ++i) {
      if (fds_.at(i).fd == -1) continue;
      for (unsigned int j = 0; j < tasks_.at(i).size(); ++j) {
        if (tasks_.at(i).at(j) == NULL) continue;
        if (fds_.at(i).revents & tasks_.at(i).at(j)->GetEvent()) {
          Result<int, std::string> result = tasks_.at(i).at(j)->Execute();
          if (result.IsErr()) {
            DeleteTasks();
            throw std::invalid_argument("taskエラー");
          } else if (result.Unwrap() == AIOTask::kWriteDelete) {
            RemoveWriteTask(tasks_.at(i).at(j));
            --j;
          } else if (result.Unwrap() == AIOTask::kReadDelete) {
            RemoveReadTask(tasks_.at(i).at(j));
            --i;
          }
        }
      }
    }
  }
}
