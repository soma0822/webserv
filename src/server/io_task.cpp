#include "io_task.hpp"

AIOTask::AIOTask() {}

AIOTask::AIOTask(int fd, int event, int timeout_s)
    : fd_(fd), event_(event), timeout_s_(timeout_s) {}

AIOTask::~AIOTask() {}

int AIOTask::GetFd() const { return fd_; }

int AIOTask::GetEvent() const { return event_; }

int AIOTask::GetTimeout() const { return timeout_s_; }
