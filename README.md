# libtlscpp

A C++ encapsulation of [libtls](https://www.libressl.org/). I test it on `OpenBSD`, but it should work on other [OSs](https://en.wikipedia.org/wiki/LibreSSL#Adoption) which support `LibreSSL`.

## Usage

	# git clone https://github.com/NanXiao/libtlscpp.git
	# cd libtlscpp
	# mkdir build
	# cd build
	# cmake ..
	# make && make install

The `libtlscpp.so` will be installed in `/usr/local/lib/libtlscpp` directory, while header files are in `/usr/local/include/libtlscpp`.

## Example
In `build/examples` folder, there is a `tls_server` and `tls_client`. The `root.pem`, `server.crt` and `server.key` files are generated from [libtls](https://github.com/bob-beck/libtls/tree/master/CA) project.

Run `tls_server` in one terminal:  

	# ./tls_server

Run `tls_client` in another terminal:  

	# ./tls_client
	Hello world!

Back to `tls_server`'s terminal, you can see another "Hello world!" is printed out:  

	# ./tls_server
	Hello world!

## References:  
[LibreSSL](http://www.libressl.org/);  
[libtls](https://github.com/bob-beck/libtls).