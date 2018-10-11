#pragma once

#include <string_view>

#include <config.hxx>
#include <context.hxx>

namespace libtlscpp
{
    class server : public context
    {
    public:
        server(config& cfg) noexcept;

        void listen(std::string_view host, std::string_view port);

        context accept_connection();
    };
}

