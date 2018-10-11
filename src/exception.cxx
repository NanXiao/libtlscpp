#include <exception.hxx>
namespace libtlscpp
{
    exception::exception(const std::string& what_arg) :
        std::runtime_error{"[libtlscpp exception: " + what_arg + "]"}
    {
    }

    exception::exception(const char* what_arg) :
        std::runtime_error("[libtlscpp exception: " + std::string{what_arg} + "]")
    {
    }

}

