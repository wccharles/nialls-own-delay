```bash
cmake -B [BUILD_FOLDER] (-G [GENERATOR_NAME])

cmake --build [BUILD_FOLDER] (--config Release/Debug/...)
```

```bash
cmake -B [IOS_BUILD_FOLDER] -G Xcode -DCMAKE_SYSTEM_NAME=iOS -DCMAKE_OSX_DEPLOYMENT_TARGET=9.3
cmake --build [IOS_BUILD_FOLDER] --target <targetName> -- -sdk iphonesimulator
```
