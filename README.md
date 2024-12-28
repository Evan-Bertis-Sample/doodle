# doodle

A platform-agnostic, lightweight framework and build tool for building embedded applications in C/C++, intended for user-facing applications on embedded systems. Originally intended for `Northwestern Formula Racing`'s 2025 car dashboard, but open to all.

**Building embedded applications can be a pain, particularly when building applications like user interfaces.** You end up spending a lot of time working on drivers, reflashing devices for each change, and debugging hardware issues.

`doodle` aims to solve this issue by allowing you to focus on making your application, not on the hardware. It serves as an easily extensible framework that allows for you to _write your application once, and test on any platform._ This can be used to run embedded application simulations on your desktop, or to run on real hardware.

Imagine this: you are building a dashboard for a car, it has a few gauges, some buttons, and a screen. You can write your application using `doodle`, and test it on your desktop, and get it working perfectly. Then, you can flash it to your embedded device, and it will work exactly the same. No need to worry about hardware differences, or driver issues.

## Features

- **Easily extensible framework**: `doodle` is designed to be easily extensible, allowing you to add support for new platforms, devices, and drivers with ease. Platforms are defined as a set of modules which can be easily swapped out with platform-specific implementations.
- **Platform-agnostic**: Write your application once, and run it on any platform. As long as you don't use any hardware-specific features in your doodle application, you can run it on your desktop, or on your embedded device.
- **Friendly command-line interface**: `doodle` comes with a friendly command-line interface that allows you to easily build, run, and test your applications.
- **Built-in simulation support**: `doodle` comes with built-in support for simulating your application on your desktop, allowing you to test your application without needing to flash it to your device.
- **Written completely in C99**: `doodle` is written in C99, meaning it can be easily compiled onto most platforms, and can be used in your existing projects with ease.

## Examples!

I have created a few example applications to show off the capabilities of `doodle`. These work on the native platform and ESP32 using PlatformIO. You can find them in their own [repository](https://github.com/evan-bertis-sample/doodle-examples.git).

## Some Concepts

doodle serves as a modular hardware-abstraction layer. At the core of `doodle` is the concept of a `Platform`. A `Platform` is a set of modules that define the hardware and drivers for a specific platform. Some modules that a `Platform` might include are:

- `Display`: A module that defines how to interact with a display.
- `Input`: A module that defines how to interact with input devices.
- `Output`: A module that defines how to interact with output devices.
- `Storage`: A module that defines how to interact with storage devices.
- `Audio`: A module that defines how to interact with audio devices.

In addition to this, a platform is reponsible for defining the entry point for the application, and for initializing the modules that it uses. This is super important, allowing you to target devices without standard C entry points like `main`. For example, you could target an Arduino, which uses `setup` and `loop` functions instead of `main`.

You don't need to support every module. If you find that you don't need audio, you can simply not include the `Audio` module in your platform.

A doodle application is the business logic that runs on top of a `Platform`. It is responsible for defining the application logic, and for interacting with the modules provided by the `Platform`. This is the code that you'd write to define your dashboard, or your game, or your thermostat.

## Getting started

### Prerequisites

- `CMake` version 3.10 or higher
- A C/C++ compiler that supports C99
- `git` for cloning the repository
- `python` for using the doodle CLI

### Creating a project

To create a new `doodle` project, you can use the `doodle` CLI. First, clone the repository:

```bash
git clone https://github.com/evan-bertis-sample/doodle.git
```

Then, you have to install the CLI:

```bash
cd doodle/scripts
./doodle_install.sh
```

This will install the doodle CLI for use globally. You may have to follow the instructions to add the CLI to your path if you are on Windows.

```bash
doodle create project <project_name> <path>
```

This will create a simple doodle project, which looks like this:

```c
#include <stdio.h>

#include <doodle/core/doodle_platform.h>
#include <doodle/core/doodle_app.h>

static void blank_setup(void) {
    printf("setup\n");
}

static void blank_loop(void) {
    printf("loop\n");
}

static void blank_teardown(void) {
    printf("teardown\n");
}

// This describes the entry point for the application. The actual
// entry point is defined in the platform implementation.
// For example, the native platform uses a standard C entry point.
// While the Arduino platform uses the setup and loop functions.
doodle_app_desc_t doodle_main(int32_t argc, char *argv[]) {
    doodle_app_desc_t desc = {
        .name = "blank",
        .display_size = doodle_vec2_make(800.0f, 600.0f),
        .setup = blank_setup,
        .loop = blank_loop,
        .teardown = blank_teardown,
    };
    return desc;
}
```

This is a simple doodle application that prints "setup", "loop", and "teardown" to the console. You can build and run this application using the `doodle` CLI. It also comes with all of the necessary .ini files that `doodle` needs to build and run your application. Documentation on this will be coming soon.

```bash
doodle build native .
```

This commmand will build the project for the native platform, and run it. You can also build for other platforms, like `pio` which will build for the PlatformIO platform.
