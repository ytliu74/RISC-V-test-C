# RISC-V Test C Code

You need Docker to compile. Then 

```
docker pull ytliu74/cva6:latest
```

to get the docker image

## Compile

Please put your C code in the same directory as the `Makefile`, then simply run `make`. Be aware that it currently only supports one C file.
But the `Makefile` is easy to understand. You can modify it accordingly.