#include "index.hpp"
#include "protocol.hpp"

#include <string>
#include <thread>
#include <csignal>
#include <cstdio>

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

static constexpr int PORT = 8080;

static bool writeAll(int fd, const std::string& s) {
    size_t sent = 0;
    while (sent < s.size()) {
        ssize_t n = ::send(fd, s.data() + sent, s.size() - sent, MSG_NOSIGNAL);
        if (n <= 0) return false;
        sent += static_cast<size_t>(n);
    }
    return true;
}

static void handleConnection(int fd, Index* idx) {
    std::string buf;
    char tmp[4096];
    while(1) {
        ssize_t n = ::recv(fd, tmp, sizeof(tmp), 0);
        if (n <= 0) break;
        buf.append(tmp, static_cast<size_t>(n));
        size_t nl;
        while ((nl = buf.find('\n')) != std::string::npos) {
            std::string line = buf.substr(0, nl);
            buf.erase(0, nl + 1);
            if (!writeAll(fd, process(line, *idx))) { ::close(fd); return; }
        }
    }
    ::close(fd);
}

int main() {
    ::signal(SIGPIPE, SIG_IGN); // a peer disconnect must not kill the server

    int listenFd = ::socket(AF_INET, SOCK_STREAM, 0);
    int opt = 1;
    ::setsockopt(listenFd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    sockaddr_in addr{};
    addr.sin_family = AF_INET; // IPv4
    addr.sin_addr.s_addr = htonl(INADDR_ANY); //host to network byte order  
    addr.sin_port = htons(PORT); //defined 8080

    if (::bind(listenFd, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) < 0) {
        std::perror("bind");
        return 1;
    }
    if (::listen(listenFd, SOMAXCONN) < 0) {
        std::perror("listen");
        return 1;
    }

    Index idx;
    while(1) {
        int fd = ::accept(listenFd, nullptr, nullptr);
        if (fd < 0) continue;
        std::thread(handleConnection, fd, &idx).detach();
    }
}
