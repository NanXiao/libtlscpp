add_executable(tls_client tls_client.cxx)
target_link_libraries(tls_client tlscpp tls ssl crypto)

add_executable(tls_server tls_server.cxx)
target_link_libraries(tls_server tlscpp tls ssl crypto)

configure_file(server.crt server.crt)
configure_file(server.key server.key)
configure_file(root.pem root.pem)