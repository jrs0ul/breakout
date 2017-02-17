# Sets the minimum version of CMake required to build the native
# library. You should either keep the default value or only pass a
# value of 3.4.0 or lower.

cmake_minimum_required(VERSION 3.4.1)


add_library(app-glue STATIC
            ${ANDROID_NDK}/sources/android/native_app_glue/android_native_app_glue.c)


# Creates and names a library, sets it as either STATIC
# or SHARED, and provides the relative paths to its source code.
# You can define multiple libraries, and CMake builds it for you.
# Gradle automatically packages shared libraries with your APK.

add_library( # Sets the name of the library.
             breakout

             # Sets the library as a shared library.
             SHARED

             # Provides a relative path to your source file(s).
             # Associated headers in the same location as their source
             # file are automatically included.
             src/main/cpp/breakout.cpp
             src/main/cpp/CppSingleton.cpp
             src/main/cpp/TextureLoader.cpp
             src/main/cpp/Ball.cpp
             src/main/cpp/Vectors.cpp
             src/main/cpp/BBullets.cpp
             src/main/cpp/Xml.cpp
             src/main/cpp/Particles2D.cpp
             src/main/cpp/Utils.cpp
             src/main/cpp/SysConfig.cpp
             src/main/cpp/map.cpp
             src/main/cpp/Matrix.cpp
             src/main/cpp/Padd.cpp
             )

# Searches for a specified prebuilt library and stores the path as a
# variable. Because system libraries are included in the search path by
# default, you only need to specify the name of the public NDK library
# you want to add. CMake verifies that the library exists before
# completing its build.

find_library( # Sets the name of the path variable.
              log-lib

              # Specifies the name of the NDK library that
              # you want CMake to locate.
              log )

# Specifies libraries CMake should link to your target library. You
# can link multiple libraries, such as libraries you define in the
# build script, prebuilt third-party libraries, or system libraries.

target_include_directories(breakout PRIVATE
                           ${CMAKE_CURRENT_SOURCE_DIR}
                           ${ANDROID_NDK}/sources/android/native_app_glue)




target_link_libraries( # Specifies the target library.
                       breakout
                       android
                       EGL
                       GLESv1_CM
                       app-glue
                       OpenSLES


                       # Links the target library to the log library
                       # included in the NDK.
                       ${log-lib} )
