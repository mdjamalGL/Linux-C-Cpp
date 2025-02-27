from conan import ConanFile
from conan.tools.cmake import CMake, CMakeDeps, CMakeToolchain, cmake_layout

class ffmplay(ConanFile):
    settings = 'os', 'compiler', 'build_type', 'arch'

    def requirements(self):
        self.requires('ffmpeg/7.0.1')
        self.requires('sdl/2.30.9')

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