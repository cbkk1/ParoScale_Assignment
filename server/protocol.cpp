#include "protocol.hpp"
#include "index.hpp"

const char* const RESP_OK    = "OK\n";
const char* const RESP_FAIL  = "FAIL\n";
const char* const RESP_ERROR = "ERROR\n";

// split breaks s on delim keeping empty fields, so N delimiters give N+1 fields.
static std::vector<std::string> split(const std::string& s, char delim) {
    std::vector<std::string> out;
    std::string cur;
    for (char c : s) {
        if (c == delim) { out.push_back(cur); cur.clear(); }
        else cur.push_back(c);
    }
    out.push_back(cur);
    return out;
}

bool parseLine(const std::string& line, std::string& cmd, std::string& pkg,
               std::vector<std::string>& deps) {
    std::string l = line;
    if (!l.empty() && l.back() == '\r') l.pop_back();

    auto parts = split(l, '|');
    if (parts.size() != 3) return false;

    cmd = parts[0];
    pkg = parts[1];
    if (pkg.empty()) return false;
    if (cmd != "INDEX" && cmd != "REMOVE" && cmd != "QUERY") return false;

    deps.clear();
    if (!parts[2].empty())
        for (auto& d : split(parts[2], ','))
            if (!d.empty()) deps.push_back(d);
    return true;
}

std::string process(const std::string& line, Index& idx) {
    std::string cmd, pkg;
    std::vector<std::string> deps;
    if (!parseLine(line, cmd, pkg, deps)) return RESP_ERROR;
    if (cmd == "INDEX")  return idx.index(pkg, deps) ? RESP_OK : RESP_FAIL;
    if (cmd == "REMOVE") return idx.remove(pkg)      ? RESP_OK : RESP_FAIL;
    return idx.query(pkg) ? RESP_OK : RESP_FAIL;     // QUERY
}
