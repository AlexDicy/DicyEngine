### Engine setup
#### Prerequisites
- [xmake](https://xmake.io/#/guide/installation)
- [JDK 23](https://www.oracle.com/java/technologies/downloads/#java23)

```bash
git submodule init
git submodule update
xmake -y
```
#### Windows - Core (C++) development
```bash
xmake project -k vsxmake
```
#### macOS - Core (C++) development
```bash
xmake project -k cmakelists
```

#### Java development
Debugging and running the engine from Java is possible by using the `runEngine` Gradle task.
If you don't have an IDE, you can run Gradle from the command line.
```bash
cd scripting/
./gradle run :runEngine
# or on Windows
.\gradlew.bat run :runEngine
```
`runEngine` will build the engine executable through xmake and then run it.

---
### Project screenshots (from recent to older)

![image](https://github.com/user-attachments/assets/53a4e6a4-7f3d-4e57-a72b-20dcdc79790c)

![image](https://github.com/user-attachments/assets/3ae0ce3c-48b8-4360-9f0e-f2bcdb324066)

![image](https://github.com/user-attachments/assets/84e251d1-14e5-49e1-8f9a-f6c5a714ec71)
