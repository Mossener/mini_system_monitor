Package: fmt:x64-linux@12.1.0

**Host Environment**

- Host: x64-linux
- Compiler: GNU 11.4.0
- CMake Version: 3.30.1
-    vcpkg-tool version: 2025-11-19-da1f056dc0775ac651bea7e3fbbf4066146a55f3
    vcpkg-scripts version: 1d611efb49 2025-11-24 (7 小时前)

**To Reproduce**

`vcpkg install `

**Failure logs**

```
-- Using cached fmtlib-fmt-12.1.0.tar.gz
-- Cleaning sources at /home/yuirka/c_area/mini_system_monitor/extern/vcpkg/buildtrees/fmt/src/12.1.0-54f1f91231.clean. Use --editable to skip cleaning for the packages you specify.
-- Extracting source /home/yuirka/c_area/mini_system_monitor/extern/vcpkg/downloads/fmtlib-fmt-12.1.0.tar.gz
-- Using source at /home/yuirka/c_area/mini_system_monitor/extern/vcpkg/buildtrees/fmt/src/12.1.0-54f1f91231.clean
-- Configuring x64-linux
-- Building x64-linux-dbg
-- Building x64-linux-rel
-- Fixing pkgconfig file: /home/yuirka/c_area/mini_system_monitor/extern/vcpkg/packages/fmt_x64-linux/lib/pkgconfig/fmt.pc
CMake Error at scripts/cmake/vcpkg_find_acquire_program.cmake:166 (message):
  Could not find pkg-config.  Please install it via your package manager:

      sudo apt-get install pkg-config
Call Stack (most recent call first):
  scripts/cmake/vcpkg_fixup_pkgconfig.cmake:193 (vcpkg_find_acquire_program)
  ports/fmt/portfile.cmake:19 (vcpkg_fixup_pkgconfig)
  scripts/ports.cmake:206 (include)



```

**Additional context**

<details><summary>vcpkg.json</summary>

```
{
  "name": "system-event-monitor",
  "version": "0.1.0",
  "dependencies": [
    "fmt",
    "spdlog",
    "nlohmann-json"
  ]
}

```
</details>
