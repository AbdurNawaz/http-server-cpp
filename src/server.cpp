#include <iostream>
#include <cstdlib>
#include <string>
#include <cstring>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>

int main()
{
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(4221);

    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        std::cerr << "Failed to bind to port " << 4221 << std::endl;
        return 1;
    }

    int connection_backlog = 5;
    if (listen(server_fd, connection_backlog) < 0)
    {
        std::cerr << "Failed to listen" << std::endl;
        return 1;
    }

    struct sockaddr_in client_addr;
    int client_addr_len = sizeof(client_addr);

    int client_fd = accept(server_fd, (sockaddr *)&client_addr, (socklen_t *)&client_addr_len);
    if (client_fd < 0)
    {
        std::cerr << "Failed to accept response" << std::endl;
        return 1;
    }

    std::string request(1024, '\0');
    size_t recvd = recv(client_fd, (void *)&request[0], request.size(), 0);
    if (recvd < 0)
    {
        std::cerr << "Failed to receive request" << std::endl;
        return 1;
    }

    std::cout << request << std::endl;

    std::string response;

    if (request.starts_with("GET / HTTP/1.1"))
    {
        response = "HTTP/1.1 200 OK\r\n\r\n";
    }
    else
    {
        response = "HTTP/1.1 404 Not Found\r\n\r\n";
    }
    send(client_fd, response.c_str(), response.length(), 0);

    close(server_fd);

    return 0;
}