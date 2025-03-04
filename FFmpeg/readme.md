# Build Image
` >> sudo docker build -t ffmpImage .`

# Create Container
` >> sudo docker run -it --name ffmpContainer ffmpImage /bin/bash`

```
>> cd FFmpeg/build
>> cmake .. -DCMAKE_TOOLCHAIN_FILE="Release/generators/conan_toolchain.cmake" -DCMAKE_BUILD_TYPE=Release
>> make
```

# Extract Binary
`>> sudo docker cp ffmpContainer:/FFmpeg/build/dplay .`
