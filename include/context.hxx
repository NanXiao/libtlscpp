#pragma once

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

#include <string_view>

#include <config.hxx>

namespace libtlscpp
{
    class context
    {
    public:
        context(config& cfg) noexcept;

        context(config& cfg, int connfd, tls* c) noexcept;

        context(const context& other) = delete;

        context(context&& other) noexcept;

        context& operator=(const context&& other) = delete;

        context& operator=(context&& other);

        virtual ~context();

        int get_socket_fd() noexcept;

        std::size_t write(void *buf, std::size_t len);

        std::size_t read(void *buf, std::size_t len);
    protected:
        enum class Type {Server, Client};

        config& cfg;

        int sockfd{-1};

        std::unique_ptr<tls, void(*)(tls*)> tls_ptr{
            nullptr, [](auto v){tls_close(v); tls_free(v);}};

        [[noreturn]] void throw_exception();

        [[noreturn]] void throw_syscall_exeption(std::string_view prefix);

        void init(Type t, std::string_view host, std::string_view port);
   private:
       std::unique_ptr<struct addrinfo, void(*)(struct addrinfo*)> addrinfo_ptr{
           nullptr, [](auto v){freeaddrinfo(v);}};

       void init_socket(Type t, std::string_view host, std::string_view port);

       void reset_socket() noexcept;
    };
}
