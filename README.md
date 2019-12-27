BSMacro++
=========
[![C++](https://img.shields.io/badge/language-c++-brightgreen)](#)
[![License](https://img.shields.io/github/license/george-lim/bsmacroplusplus)](https://github.com/george-lim/bsmacroplusplus/blob/master/LICENSE)

**The easiest way to dynamically generate BlueStacks macros**

[BlueStacks 4](https://www.bluestacks.com) introduces a helpful [macro recording](https://support.bluestacks.com/hc/en-us/articles/360032816491-How-can-I-use-Macro-Recorder-) feature that stores recorded keypresses and mouse actions in a JSON object. This makes macros easy to create, import and export - but very difficult to read and modify once created.

The biggest problem with recorded macros is that timestamps are recorded for each action **relative to the recording start time** instead of the previous action, which means modifying the timestamp of one action requires modifying the timestamp of every successive action to keep timings consistent. This makes adding, removing, and reordering actions extremely tedious.

Additionally, user actions are recorded as a sequence of many sub-actions that make recordings hard to read and tweak manually. For example, a mouse drag consists of a `MouseDown` event, (typically) hundreds of `MouseMove` events, and a `MouseUp` event. Therefore, tweaking a drag action manually is infeasible.

BSMacro++ builds macros using the same JSON format as BlueStacks, but allows users to program user actions without worrying about absolute timestamps or sub-actions. It is designed so that each user action can be written in one line, and every parameter of an action can be tweaked. Actions can be easily inserted and swapped without affecting timestamps.

NOTE: BSMacro++ installation and usage is very technical, and requires basic C++ programming knowledge.

[Features](#features)
| [Coordination](#coordination)
| [Usage](#usage)
| [Compilation](#compilation)
| [License](#license)

## Features
* Supports mouse clicks, linear mouse drags, and keypresses
* Dynamically calculates absolute timestamps for every action
* Automatically generates all sub-actions for a given action
* Organized, easy to read and write macro scripts
* Lightweight implementation in C++, compiled easily with g++

## Coordination
BSMacro++ is intended to be used alonside BlueStack's macro recording feature. Bot creators should use BlueStack's macro recording feature to record action segments, then convert the recording data to BSMacro++ actions. This reduces the amount of time required to calculate coordinates and timings from scratch. Then, creators can tweak the BSMacro++ actions to easily tweak the macro.

## Usage
All action programming is done in the BSMacro++ code itself. Navigate to the `main` function and modify the value of the `name` variable to give the macro a new name. Then, add actions by appending elements to the `events` array.

A few notes on writing actions:
1. All coordinates represent screen percentages. Thus, a coordinate (X,Y) is bounded by (0,0) representing the top left corner (with 0% width and 0% height) and (99.9,99.9) representing the bottom right corner (with 100% width and 100% height).
2. Never modify the parameter `time` in any action. It is used to calculate absolute timestamps for all actions during compilation.
3. `DELAY` and `DURATION` parameters are measured in milliseconds and are optional - they can be omitted and will assume their default values of 500 and 200 respectively.
4. `DELAY` is relative to the previous action in the `events` array.
5. Coordinate parameters `X`, `Y`, `X2`, `Y2` must be decimal values. With the exception of 0, every value must end in a non-zero value. For example, (50,50) must be changed to (50.01, 50.01).
6. `DELAY`, `DURATION`, `MOVEACTION` parameters must contain only whole numbers >= 0.
7. `KEY` parameter must be a string (encapsulated in double quotes) and must have only the first character capitalized. For example, `"Esc"` and `"A"` are valid `KEY` values.

* Mouse click actions take on the form of `{X, Y, time, DELAY, DURATION}`. The macro will wait `DELAY` milliseconds, then click at (X,Y) for `DURATION` before releasing.
* Keypress actions take on the form of `{KEY, time, DELAY, DURATION}`. The macro will wait `DELAY` milliseconds, then type a single `KEY` for `DURATION` before releasing.
* Mouse drag actions take on the form of `{X, Y, X2, Y2, MOVEACTIONS, time, DELAY, DURATION}`. The macro will wait for `DELAY` milliseconds, then drag from (X,Y) to (X2,Y2) for `DURATION` before releasing. `MOVEACTIONS` determines the number of `MouseMove` actions to be recorded in between `MouseDown` and `MouseUp` actions. Generally, The more `MouseMove` actions, the smoother and more accurate the drag. Most drag actions have between 100-200 `MouseMove` actions.

The following represents sample code to create a macro:
```
const std::string name = "Demo"; // Specify new macro name
const Action events[] = { // Provide a list of actions to print
  {0, 0, time, 0}, // Click at (0,0) with no initial delay and default duration
  {0, 0, 50.01, 50.01, 100, time, 500, 2000}, // Drag from (0,0) to (50.01,50.01) over 100 move actions, with 500ms delay and 2000ms duration
  {"Esc", time}, // Type 'ESC' (equivalent to pressing back button) using default delay and duration
};
```

## Compilation
After writing the macro script in BSMacro++, it needs to be compiled and run in order to generate the BlueStacks-compatible JSON macro script.

BSMacro++ requires g++ to compile. Users running macOS should already have g++ installed, but Windows and Linux users will either have to install it manually, or paste the BSMacro++ code into an online g++ compiler like [OnlineGDB](https://www.onlinegdb.com/online_c++_compiler).

To compile and run BSMacro++, execute the following in a command line shell:
```
g++ -std=c++11 BSMacro++.cc -o BSMacro++
./BSMacro++ > macro.json
```
This will create the final macro script called `macro.json`.
Finally, import `macro.json` into BlueStacks to test out the new macro!

## License
This project is copyrighted under the MIT license. Complete license can be found here: https://github.com/george-lim/bsmacroplusplus/blob/master/LICENSE
