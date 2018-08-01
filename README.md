# Build steps
  
  ```
  cd <path-to-project-src>
  cmake -Bbuild -H. -DCMAKE_INSTALL_PREFIX=<path-to-install> -DCMAKE_BUILD_TYPE=<build-type> -DGTEST_ROOT=<path-to-google-test-libs>
  cmake --build build
  cmake --build build --target-install
  ```
  
**Options**:

`<build-type>`: `Release` or `Debug` (`Release` by default)

