#include "io_task_manager.hpp"

std::vector<Tasks> IOTaskManager::tasks_array_;
std::vector<struct pollfd> IOTaskManager::fds_;

IOTaskManager::IOTaskManager() {}

IOTaskManager::~IOTaskManager() {
  for (unsigned int i = 0; i < tasks_array_.size(); ++i) {
    for (unsigned int j = 0; j < tasks_array_.at(i).tasks.size(); ++j) {
      delete tasks_array_.at(i).tasks.at(j);
    }
  }
}

const std::vector<Tasks> &IOTaskManager::GetTasks() { return tasks_array_; }
const std::vector<struct pollfd> &IOTaskManager::GetFds() { return fds_; }

void IOTaskManager::AddTask(AIOTask *task) {
  for (unsigned int i = 0; i < fds_.size(); ++i) {
    if (fds_.at(i).fd == task->GetFd()) {
      for (unsigned int j = 0; j < tasks_array_.at(i).tasks.size(); ++j) {
        if (tasks_array_.at(i).tasks.at(j) == NULL) {
          tasks_array_.at(i).tasks.at(j) = task;
          return;
        }
      }
      tasks_array_.at(i).tasks.push_back(task);
      return;
    }
  }
  struct pollfd fd = {task->GetFd(), POLLIN | POLLOUT, 0};
  for (unsigned int i = 0; i < fds_.size(); ++i) {
    if (fds_.at(i).fd == -1) {
      fds_.at(i) = fd;
      tasks_array_.at(i).tasks.push_back(task);
      return;
    }
  }
  Tasks tmp = {std::vector<AIOTask *>(1, task), 0};
  tasks_array_.push_back(tmp);
  fds_.push_back(fd);
}

void IOTaskManager::RemoveFd(AIOTask *task) {
  for (unsigned int i = 0; i < fds_.size(); ++i) {
    if (fds_.at(i).fd == task->GetFd()) {
      for (unsigned int j = 0; j < tasks_array_.at(i).tasks.size(); ++j) {
        delete tasks_array_.at(i).tasks.at(j);
      }
      close(fds_.at(i).fd);
      tasks_array_.at(i).tasks.clear();
      tasks_array_.at(i).index = 0;
      fds_.at(i).fd = -1;
      Logger::Info() << "接続を削除しました" << std::endl;
      return;
    }
  }
}

void IOTaskManager::DeleteTasks() {
  for (unsigned int i = 0; i < tasks_array_.size(); ++i) {
    for (unsigned int j = 0; j < tasks_array_.at(i).tasks.size(); ++j) {
      delete tasks_array_.at(i).tasks.at(j);
    }
  }
  tasks_array_.clear();
  fds_.clear();
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
      tasks_array_.at(i).index >= tasks_array_.at(i).tasks.size()
          ? tasks_array_.at(i).index = 0
          : 0;
      while (
          tasks_array_.at(i).tasks.at(tasks_array_.at(i).index) == NULL ||
          !(tasks_array_.at(i).tasks.at(tasks_array_.at(i).index)->GetEvent() &
            fds_.at(i).revents)) {
        ++(tasks_array_.at(i).index);
        if (tasks_array_.at(i).index >= tasks_array_.at(i).tasks.size()) {
          tasks_array_.at(i).index = 0;
          break;
        }
      }
      if (tasks_array_.at(i).tasks.at(tasks_array_.at(i).index) != NULL &&
          (tasks_array_.at(i).tasks.at(tasks_array_.at(i).index)->GetEvent() &
           fds_.at(i).revents)) {
        Result<int, std::string> result =
            tasks_array_.at(i).tasks.at(tasks_array_.at(i).index)->Execute(int revent);
        if (result.IsErr()) {
          DeleteTasks();
          throw std::invalid_argument("taskエラー");
        } else if (result.Unwrap() == AIOTask::kTaskDelete) {
          delete tasks_array_.at(i).tasks.at(tasks_array_.at(i).index);
          tasks_array_.at(i).tasks.at(tasks_array_.at(i).index) = NULL;
        } else if (result.Unwrap() == AIOTask::kFdDelete) {
          RemoveFd(tasks_array_.at(i).tasks.at(tasks_array_.at(i).index));
          --i;
        } else if (result.Unwrap() == AIOTask::kContinue) {
          ;
        } else if (result.Unwrap() == AIOTask::kOk) {
          ++(tasks_array_.at(i).index);
        }
      }
    }
  }
}
