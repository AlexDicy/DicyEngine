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

<img width="643" alt="image" src="https://github.com/user-attachments/assets/84e251d1-14e5-49e1-8f9a-f6c5a714ec71">

<img width="643" alt="image" src="https://github.com/user-attachments/assets/cec10ddc-a71c-4b96-8cdd-b3f16de92cab">

<img width="643" alt="image" src="https://github.com/AlexDicy/DicyEngine/assets/11839341/36d1367e-a957-4aae-805f-1299eda8dbb1">

<img width="643" alt="image" src="https://github.com/AlexDicy/DicyEngine/assets/11839341/e0b67f06-ac5a-43df-80b7-ed7b4c7e26a3">

<img width="643" alt="image" src="https://github.com/AlexDicy/DicyEngine/assets/11839341/e9ff1080-324e-4e20-a97e-62ba2d8fac59">
