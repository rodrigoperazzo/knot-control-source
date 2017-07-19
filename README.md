# knot-control-source

KNoT Control is part of KNoT project. It is a service daemon running in KNoT gateway and is responsible for controlling the execution and configuration of all other services/daemons running in the gateway.

Dependencies: none

How to build (daemon):
```shell
 $ gcc -o knotctld src/main.c
```

How to build (CLI):
```shell
 $ gcc -o knotctl tools/knotctl.c
```
