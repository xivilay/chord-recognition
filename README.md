## How to build

### Windows 

#### Prepare evironment
* Download and install  [MS Build Tools](https://visualstudio.microsoft.com/downloads/#build-tools-for-visual-studio-2019)

* Download and install [Cmake](https://cmake.org/download/)


* Clone repo
    ```
    git clone https://github.com/xivilay/react-juce-chord-recognition-cpp.git
    ```

* Fetch submodules
    ``` 
    git submodule update --init --recursive
    ```
* Run `install_deps.bat` (`yarn` should be installed globally)
* Build js bundle with `build_js.bat`
* Build binary with `build.bat`