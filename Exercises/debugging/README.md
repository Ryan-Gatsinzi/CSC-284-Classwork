# Broken Code Debug Challenge

## Goal
Use the VSCode debugger to find and fix bugs in a C++ program.

The program is supposed to:

1. Store a list of integers
2. Compute their average
3. Print all values greater than the average

Expected correct output:

```text
Average: 30
Values above average: 40 50
```

## Setup

Open this folder in VSCode.

Make sure you have:

- VSCode
- The Microsoft C/C++ extension
- `g++`
- `gdb` installation:
   - Mac: `brew install gdb`
   - Windows 
      - MSYS2 / MinGW: `pacman -S mingw-w64-x86_64-gdb`
      - MSYS2 / UCRT64: `pacman -S mingw-w64-ucrt-x86_64-gdb`
   - Linux: `sudo apt install gdb`

## How to Build

In VSCode:

1. Open the Terminal menu
2. Choose **Run Build Task**
3. Select **Build Debug Version** if asked

Or press:

- Windows/Linux: `Ctrl + Shift + B`
- Mac: `Cmd + Shift + B`

## How to Debug

1. Open `main.cpp`
2. Click to the left of a line number to set a breakpoint
3. Press `F5`
4. Use the debug toolbar to:
   - Continue
   - Step Over
   - Step Into
   - Step Out
   - Stop

## Suggested First Breakpoints

Try setting breakpoints at:

```cpp
int sum;
```

```cpp
double avg = computeAverage(values);
```

```cpp
std::vector<int>& filtered = filterAboveAverage(values, avg);
```

## Your Task

Find and fix all bugs.

Do not rewrite the program from scratch.
