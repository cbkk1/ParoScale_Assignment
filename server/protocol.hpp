#pragma once

#include <string>
#include <vector>

class Index;

extern const char* const RESP_OK;
extern const char* const RESP_FAIL;
extern const char* const RESP_ERROR;

bool parseLine(const std::string& line, std::string& cmd, std::string& pkg,
               std::vector<std::string>& deps);
std::string process(const std::string& line, Index& idx);

