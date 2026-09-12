# linear-algebra
Library for linear algebra calculations

## Dev Tools

These are the tools I use to develop this project:

- Compiler: `clang++` (C++11)
- Language server: `clangd`
- Build system: `make` (see `Makefile`)
- Compilation database: `Bear`
- Formatter (optional): `clang-format`, clang-format-17

For macOS, everything is available in Apple Developer Tools and MacPorts (Bear).

To regenerate `compile_commands.json`:
```bash
bear -- make clean all
```

To run test coverage:
```sh
make coverage        # build + run instrumented tests, print per-file report
make coverage-html   # same, plus a browsable report at coverage/html/index.html
```
