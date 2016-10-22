# vendor-template

vendor-template is a build system for vendors to use in order to link their libraries properly into WPILib.

## Build Requirements
To run a build, a few requirements must be met:

- ARM Toolchain - To crosscompile ntcore for the roboRIO, you must have the FRC ARM toolchain installed, which can be found [here](http://first.wpi.edu/FRC/roborio/toolchains/).

In addition, python is required to generate the repository correctly. However after initial creation python is no longer necessary

## Building
Gradle is the main build system used by ntcore. All tasks are run with the `gradlew` wrapper, which is included in the root of the repository. All targets that can be accomplished by Gradle are referred to as tasks. The main task available is `build`. To run Gradle, cd into the build directory and run:

```bash
./gradlew build
```

## Setting up the repository
In order to properly set up the repository, a file called InitializeTemplate.py is provided. Running this will ask for 3 questions. The first is a project name (ex. CANJaguar, ADXRS535). The second is a java package to place the code into. The 3rd is asking if you require JNI. If there is functionality you need that is not provided by WPILib but can be accessed using either the HAL or native code, that is when to use JNI.

Once you enter that info, all files will be copied into their correct places, and the builds will be ready to go.

## Using the repository (JNI)
If you selected to use JNI, the build system will build 3 libraries. The first will be a driver/JNI C++ library. This library will link to the HAL and WPIUtil, however it has no access to either NetworkTables or WPILibC directly. The reason for this is JNI will potentially run into issues if either of those libraries are linked to. The goal of this library is to provide a low level C++ class that can be used from any language. This library should also expose a C API to access that class in order to allow interop for other FRC languages. In addition, this will be the library the JNI code is placed into, and is the native library Java will use. For this library, if you can avoid any HAL level calls, and instead use the NI provided FPGA libraries directly, this driver library should also be usable from LabVIEW. However this has not been tested, and if you do use anything from the HAL, the library will most likely not be usable from LabVIEW.

The second library is a Java library. This links to WPILibJ, and can access any functionality provided by WPILibJ. This can also use the JNI provided by the driver library

The 3rd library is the C++ Implementation library. This library has full access to WPILibC and NetworkTables, and should be used to create a public interface for WPILibC teams to use.

## Using the repository (No JNI)
If you did not select to use the JNI, the build system will provide 2 libraries. The first is a C++ library that links to WPILibC with full access to all functionality provided there. The second is a Java library with full access to WPILibJ. This Java library will not have any JNI code in it, so if you need native code, please use the JNI option instead.

## Our recommendations
If your library can be used completely using WPILib level classes and functionality, the no JNI build option should be the right one. However, if you do need native level code, in order for everything to link properly and be functional, you will most likely have to use the driver/JNI library with a thin wrapper on top written in either WPILib language.

## Packing for distribution
We are still currenly working on upgrading the library in order to pack all the libraries for release. However, manually it is not too difficult. In the arm folder, there is a folder with "Driver" in the name, and one that is not. In the one without "Driver", there is a zip called "ProjectName.zip". This zip library contains both the shared and static versions of the C++ implementation library, along with the headers, so this is the only thing needed to be released there.

In the Driver folder, The "PackageNameDriver.zip" file contains only the C++ code for the Driver library. The Java jar is located in the "libs" folder. In order to release, you will need that jar, and if you built JNI, you will need the libProjectNameDriver.so library out of the zip, and that will need to be provided.