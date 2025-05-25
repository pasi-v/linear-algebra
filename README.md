# linear-algebra
Library for linear algebra calculations

## Dev Tools

These are the tools I use to develop this project:

- Compiler: `clang++` (C++11)
- Language server: `clangd`
- Build system: `make` (see `Makefile`)
- Compilation database: `Bear`
- Formatter (optional): `clang-format`

For macOS, everything is available in Homebrew.

To regenerate `compile_commands.json`:
```bash
bear -- make clean all
```
