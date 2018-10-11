#include <client.hxx>

namespace libtlscpp
{
    client::client(config& cfg) noexcept : context{cfg}
    {
    }

    void client::connect(std::string_view server_host, std::string_view server_port)
    {
        init(Type::Client, server_host, server_port);
    }
}

