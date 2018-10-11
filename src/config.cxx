#include <cstdint>
#include <string>

#include <config.hxx>
#include <exception.hxx>

namespace libtlscpp
{
    tls_config* config::get_tls_config()
    {
        return config_ptr.get();
    }

    void config::set_ca_file(std::string_view ca_file)
    {
        if (tls_config_set_ca_file(config_ptr.get(), ca_file.data()) == -1)
        {
            throw_exception();
        }
    }

    void config::set_cert_file(std::string_view cert_file)
    {
        if (tls_config_set_cert_file(config_ptr.get(), cert_file.data()) == -1)
        {
            throw_exception();
        }
    }

    void config::set_key_file(std::string_view key_file)
    {
        if (tls_config_set_key_file(config_ptr.get(), key_file.data()) == -1)
        {
            throw_exception();
        }
    }

    void config::set_ocsp_staple_file(std::string_view staple_file)
    {
        if (tls_config_set_ocsp_staple_file(config_ptr.get(), staple_file.data()) == -1)
        {
            throw_exception();
        }
    }

    void config::set_protocols(std::string_view protostr)
    {
        std::uint32_t protocols{};
        if (tls_config_parse_protocols(&protocols, protostr.data()) == -1)
        {
            throw exception{"invalid TLS protocols: " + std::string(protostr)};
        }
        if (tls_config_set_protocols(config_ptr.get(), protocols) == -1)
        {
            throw_exception();
        }
    }

    void config::set_ciphers(std::string_view ciphers)
    {
        if (tls_config_set_ciphers(config_ptr.get(), ciphers.data()) == -1)
        {
            throw_exception();
        }
    }

    void config::config_insecure_noverifyname()
    {
        tls_config_insecure_noverifyname(config_ptr.get());
    }

    void config::config_insecure_noverifycert()
    {
        tls_config_insecure_noverifycert(config_ptr.get());
    }

    void config::config_ocsp_require_stapling()
    {
        tls_config_ocsp_require_stapling(config_ptr.get());
    }

    void config::throw_exception()
    {
        throw exception{tls_config_error(config_ptr.get())};
    }
}

