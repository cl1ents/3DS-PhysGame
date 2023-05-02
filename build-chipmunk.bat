rmdir /s /q build
mkdir build
rmdir
cd build
cmake -DCMAKE_BUILD_TYPE=Release -S. -Bbuild -DBUILD_STATIC=ON -DBUILD_SHARED=OFF -DBUILD_DEMOS=OFF -DCMAKE_TOOLCHAIN_FILE=/c/devkitPro/cmake/3DS.cmake ../chipmunk
cmake --build build
xcopy /s .\build\src\libchipmunk.a ..\PhysGame\lib\libchipmunk.a /Y
cd ..