#pragma once

#include <string_view>

#include <config.hxx>
#include <context.hxx>

namespace libtlscpp
{
    class client : public context
    {
    public:
        client(config& cfg) noexcept;

        void connect(std::string_view server_host, std::string_view server_port);
    };
}

