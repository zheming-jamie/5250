# Lab 13 (MinGW64)

## Build (Git Bash / MSYS2 MinGW64)

From the project folder:

```bash
mingw32-make clean
mingw32-make
```

## Run

```bash
./main.exe        # runs both n=64 and n=128
./main.exe 64
./main.exe 128
```

If you get `Exec format error`, you are trying to run a non-Windows binary.
Delete any old `deblur` file and rebuild using the commands above.
