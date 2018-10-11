#include <iostream>
#include <memory>
#include <thread>

#include <exception.hxx>
#include <server.hxx>

int main()
{
    libtlscpp::config cfg;
    try
    {
        cfg.set_cert_file("server.crt");
        cfg.set_key_file("server.key");
    }
    catch (libtlscpp::exception& e)
    {
        std::cout << "Init config exception: " << e.what() << '\n';
        return 1;
    }

    libtlscpp::server s{cfg};

    try
    {
        s.listen("localhost", "60001");
    }
    catch (libtlscpp::exception& e)
    {
        std::cout << "Listen exception: " << e.what() << '\n';
        return 1;
    }

    while (1)
    {
        try
        {
            auto func = [](libtlscpp::context conn)
            {
                char buf[1024]{};
                conn.write(buf, conn.read(buf, sizeof(buf)));
                std::cout << buf << '\n';
            };
            auto conn = s.accept_connection();
            std::thread t{func, std::move(conn)};
            t.detach();
        }
        catch (libtlscpp::exception& e)
        {
            std::cout << "Accept exception: " << e.what() << '\n';
            return 1;
        }
    }

    return 0;
}
