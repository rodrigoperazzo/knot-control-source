# knot-control-source

KNoT Control is part of KNoT project. It is a service daemon running in KNoT gateway and is responsible for controlling the execution and configuration of all other services/daemons running in the gateway.

Dependencies:
glib >= 2.28

How to install dependencies:
	sudo apt-get install glib2.0

How to build (daemon):
```shell
 $ gcc $(pkg-config --cflags glib-2.0) -Wall -Werror -Wextra -Wno-unused-parameter -o knotctld src/main.c $(pkg-config --libs glib-2.0)
```

How to build (CLI):
```shell
 $ gcc $(pkg-config --cflags glib-2.0) -Wall -Werror -Wextra -Wno-unused-parameter -o knotctl tools/knotctl.c $(pkg-config --libs glib-2.0)
```
