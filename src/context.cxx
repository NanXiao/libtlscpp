#include <errno.h>
#include <string.h>
#include <unistd.h>

#include <memory>
#include <string>

#include <context.hxx>
#include <exception.hxx>

namespace libtlscpp
{
    void context::init_socket(Type t, std::string_view host, std::string_view port)
    {
        struct addrinfo hints{}, *res{}, *res0{};

        hints.ai_family = AF_UNSPEC;
        hints.ai_socktype = SOCK_STREAM;
        if (t == Type::Server)
        {
            hints.ai_flags = AI_PASSIVE;
        }

        if (auto error = getaddrinfo(host.data(), port.data(), &hints, &res0); error)
        {
            throw exception{gai_strerror(error)};
        }

        addrinfo_ptr.reset(res0);

        for (res = res0; res; res = res->ai_next)
        {
            if ((sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol)) < 0)
            {
                continue;
            }

            if (t == Type::Server)
            {
                if (int x = 1; setsockopt(sockfd, SOL_SOCKET, SO_REUSEPORT, &x, sizeof(x)))
                {
                    reset_socket();
                    throw_syscall_exeption("setsockopt");
                }

                if (bind(sockfd, (struct sockaddr *)res->ai_addr, res->ai_addrlen) == 0)
                {
                    break;
                }
            }
            else
            {
                if (connect(sockfd, res->ai_addr, res->ai_addrlen) == 0)
                {
                    break;
                }
            }

            reset_socket();
        }

        freeaddrinfo(addrinfo_ptr.release());

        if (sockfd != -1)
        {
            if (t == Type::Server)
            {
                if (listen(sockfd, 1) < 0)
                {
                    reset_socket();
                    throw_syscall_exeption("listen");
                }
            }
            else
            {
                if (tls_connect_socket(tls_ptr.get(), sockfd, host.data()) == -1)
                {
                    reset_socket();
                    throw_exception();
                }
            }
        }
        else
        {
            throw exception{"Initialize socket failed!"};
        }
    }

    void context::reset_socket() noexcept
    {
        if (sockfd != -1)
        {
            close(sockfd);
            sockfd = -1;
        }
    }

    void context::throw_exception()
    {
        throw exception{tls_error(tls_ptr.get())};
    }

    void context::throw_syscall_exeption(std::string_view prefix)
    {
        char err_buf[128];
        strerror_r(errno, err_buf, sizeof(err_buf));
        throw exception{std::string{prefix} + ":" + err_buf};
    }

    void context::init(Type t, std::string_view host, std::string_view port)
    {
        struct tls *c{};

        if (t == Type::Server)
        {
            c = tls_server();
        }
        else
        {
            c = tls_client();
        }

        if (!c)
        {
            if (t == Type::Server)
            {
                throw exception("Server allocates memory error!");
            }
            else
            {
                throw exception("Client allocates memory error!");
            }
        }
        else
        {
            tls_ptr.reset(c);
        }

        if (tls_configure(tls_ptr.get(), cfg.get_tls_config()) == -1)
        {
            throw_exception();
        }

        init_socket(t, host, port);
    }

    context::context(config& cfg) noexcept : cfg{cfg}
    {
    }

    context::context(config& cfg, int connfd, tls* c) noexcept :
        cfg{cfg}, sockfd{connfd}
    {
        tls_ptr.reset(c);
    }

    context::context(context&& other) noexcept :
        cfg{other.cfg}, sockfd{other.sockfd}
    {
        tls_ptr = std::move(other.tls_ptr);
        other.sockfd = -1;
    }

    context& context::operator=(context&& other)
    {
        if (this != &other)
        {
            if (std::addressof(cfg) != std::addressof(other.cfg))
            {
                throw exception("The configs are different!");
            }
            reset_socket();
            sockfd = other.sockfd;
            other.sockfd = -1;

            tls_ptr.reset(other.tls_ptr.release());
        }
        return *this;
    }

    context::~context()
    {
        reset_socket();
    }

    int context::get_socket_fd() noexcept
    {
        return sockfd;
    }

    std::size_t context::write(void *buf, std::size_t len)
    {
        auto orig_buf{static_cast<char*>(buf)};
        std::size_t proc_len{};

        while (len > 0)
        {
            auto ret = tls_write(tls_ptr.get(), orig_buf, len);
            if ((ret == TLS_WANT_POLLIN) || (ret == TLS_WANT_POLLOUT))
            {
                continue;
            }
            if (ret < 0)
            {
                if (proc_len > 0)
                {
                    break;
                }
                else
                {
                    throw_exception();
                }
            }
            orig_buf += ret;
            proc_len += ret;
            len -= ret;
        }

        return proc_len;
    }

    std::size_t context::read(void *buf, std::size_t len)
    {
        while (true)
        {
            auto ret = tls_read(tls_ptr.get(), buf, len);
            if (ret >= 0)
            {
                return ret;
            }
            else if ((ret == TLS_WANT_POLLIN) || (ret == TLS_WANT_POLLOUT))
            {
                continue;
            }
            else
            {
                throw_exception();
            }
        }
    }
}
