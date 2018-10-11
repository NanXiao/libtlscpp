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

        while (1)
        {
            char response[1024]{};
            auto n = c.read(response, sizeof(response));
            if (n)
            {
                std::cout << response;
            }
            else
            {
                break;
            }
        }
        std::cout << '\n';
    }
    catch(libtlscpp::exception& e)
    {
        std::cout << "Read/Write exception: " << e.what() << '\n';
        return 1;
    }

    return 0;
}
