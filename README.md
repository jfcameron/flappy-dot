## flappy-dot

Flappy bird clone.

Builds for linux and windows.

Uses various jfc- and gdk- libraries found on this github account. See the submodules in the `thirdparty` directory for a full list

## Building

```
git clone --recursive https://github.com/jfcameron/flappy-dot.git
cmake -S flappy-dot -B build
cmake --build build
./build/flappy
```

On Linux, glfw builds its Wayland backend by default, which needs `wayland-scanner`. To build for X11 only, configure with `-DGLFW_BUILD_WAYLAND=OFF`.
