### DicyEngine 3D
DicyEngine currently supports Windows and macOS.
- 3D, PBR (Physically Based Rendering) pipeline
- C++ and Java interoperability
- Chromium Embedded Framework integration
- Scene/scripts/assets deserialization

---

![image](https://github.com/user-attachments/assets/c599a1d1-ef22-423a-94c4-e9a67623f757)

---

### Engine setup
#### Prerequisites
- [xmake](https://xmake.io/#/guide/installation)
- [JDK 23](https://www.oracle.com/java/technologies/downloads/#java23)

```bash
git submodule init
git submodule update
xmake -y
```
#### Visual Studio / Rider - Core (C++) development
```bash
xmake project -k vsxmake
```
#### Xcode / CLion / CMake - Core (C++) development
```bash
xmake project -k cmakelists
```

#### Java development
Debugging and running the engine from Java is possible by using the `runEngine` Gradle task.
If you don't have an IDE, you can run Gradle from the command line.
```bash
cd scripting/
./gradlew run :runEngine
# or on Windows
.\gradlew.bat run :runEngine
```
`runEngine` will build the engine executable through xmake and then run it.
