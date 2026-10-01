## flappy-dot

Flappy bird clone.

Builds for linux and windows.

Uses various jfc- and gdk- libraries found on this github account. See the submodules in the `thirdparty` directory for a full list

## Building

```
git clone --recursive https://github.com/jfcameron/flappy-dot.git
cd flappy-dot
cmake --preset linux-gcc        # or linux-clang, macos-clang, windows-msvc
cmake --build --preset linux-gcc
```

The executable is written to `out/build/<preset>/`.

On Linux, glfw needs the X11 and Wayland development packages (on Debian/Ubuntu: `xorg-dev libwayland-dev libxkbcommon-dev wayland-protocols`). The `linux-clang` preset builds against libc++ (`libc++-dev libc++abi-dev`), since OpenAL does not currently compile with clang and libstdc++. See `.github/workflows/ci.yml` for the full list of packages.
