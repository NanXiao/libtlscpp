#include <netdb.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>

#include <server.hxx>

namespace libtlscpp
{
    server::server(config& cfg) noexcept : context(cfg)
    {
    }

    void server::listen(std::string_view host, std::string_view port)
    {
        init(Type::Server, host, port);
    }

    context server::accept_connection()
    {
        if (auto connfd = accept(sockfd, nullptr, nullptr); connfd > 0)
        {
            struct tls *tls_cctx{};
            if (tls_accept_socket(tls_ptr.get(), &tls_cctx, connfd) == -1)
            {
                throw_exception();
            }
            else
            {
                return context{cfg, connfd, tls_cctx};
            }
        }
        else
        {
            throw_syscall_exeption("accept");
        }
    }
}
