#pragma once

#include <tls.h>

#include <memory>
#include <string_view>

namespace libtlscpp
{
    class config
    {
    public:
        tls_config* get_tls_config();

        void set_ca_file(std::string_view ca_file);

        void set_cert_file(std::string_view cert_file);

        void set_key_file(std::string_view key_file);

        void set_ocsp_staple_file(std::string_view staple_file);

        void set_protocols(std::string_view protostr);

        void set_ciphers(std::string_view ciphers);

        void config_insecure_noverifyname();

        void config_insecure_noverifycert();

        void config_ocsp_require_stapling();

     private:
         std::unique_ptr<tls_config, void(*)(tls_config*)> config_ptr{
             tls_config_new(),
             [](auto v){tls_config_free(v);}};

         void throw_exception();
    };
}
