from conan import ConanFile
from conan.tools.cmake import CMake, CMakeDeps, CMakeToolchain, cmake_layout
import os

class ffmplay(ConanFile):
    settings = 'os', 'compiler', 'build_type', 'arch'
    def system_requirements(self):
        if self.settings.os == "Linux":
            print("demo")

    def requirements(self):
        self.requires('ffmpeg/7.0.1', options= {
            "shared":False,
            "avdevice": False,
            "swresample": False,
            "avfilter": False,
            "with_ssl": False,
            "with_libalsa": False,
            "with_pulse": False,
            "with_freetype":False,
            "with_xcb":False,
            "with_xlib":False
            })
        self.requires('sdl/2.30.9', options={"shared":False})

    def generate(self):
        deps = CMakeDeps(self)
        deps.generate()
        tc = CMakeToolchain(self)
        tc.generate()

    def layout(self):
        cmake_layout(self)

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()