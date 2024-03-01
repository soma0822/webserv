#include "io_task.hpp"

AIOTask::AIOTask() {}

AIOTask::~AIOTask() {}

int AIOTask::GetFd() const { return fd_; }

int AIOTask::GetEvent() const { return event_; }
