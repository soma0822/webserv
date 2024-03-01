#include "io_task.hpp"

AIOTask::AIOTask(){}

AIOTask::~AIOTask(){}

int AIOTask::GetFd() { return fd_; }

int AIOTask::GetEvent { return event_; }

const std::string &AIOTask::GetPort { return port_; }