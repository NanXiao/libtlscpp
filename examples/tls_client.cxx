#include <iostream>

#include <client.hxx>
#include <exception.hxx>

int main()
{
    libtlscpp::config cfg;
    cfg.set_ca_file("root.pem");
    libtlscpp::client c{cfg};

    try
    {
        c.connect("localhost", "60001");
    }
    catch(libtlscpp::exception& e)
    {
        std::cout << "Connect exception: " << e.what() << '\n';
        return 1;
    }

    try
    {
        char greeting[] = "Hello world!";
        c.write(greeting, strlen(greeting));

        char response[1024]{};
        auto n = c.read(response, sizeof(response) - 1);
        if (n)
        {
            std::cout << response << '\n';
        }
    }
    catch(libtlscpp::exception& e)
    {
        std::cout << "Read/Write exception: " << e.what() << '\n';
        return 1;
    }

    return 0;
}
