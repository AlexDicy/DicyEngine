### DicyEngine 3D
DicyEngine currently supports Windows and macOS.
- 3D, PBR (Physically Based Rendering) pipeline
- C++ and Java interoperability
- Chromium Embedded Framework integration
- Scene/scripts/assets serialization

---

![image](https://github.com/user-attachments/assets/9f161671-9bd6-44cd-b1c6-a70ffd302ea4)

---

### Engine setup
#### Prerequisites
- [xmake](https://xmake.io/#/guide/installation)
- [JDK 23](https://www.oracle.com/java/technologies/javase/jdk23-archive-downloads.html)

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

#### Running the Editor's UI
This will start a local server for the UI with HMR.
Eventually the UI will be built statically.
```bash
cd ui/
npm run dev
```
