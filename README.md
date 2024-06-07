# HydraOS
## Introduction
HydraOS is a simple operating system for the M5Cardputer. It consists of an App system, basic utilities, and a simple 
component system. The App system allows for the creation of applications that can be run on the M5Cardputer. The basic
utilities provide a way to interact with the system and the component system allows for the creation of components that
can be used in applications.
## Bundled Apps
Some basic apps are bundled with the full version of HydraOS. These apps include:
- Calculator: A simple calculator app based on tinyexpr
- LED App: A simple app that allows you to control the onboard LED
- Settings: A simple settings app that allows you to change the system settings
## Concepts
### Apps
Apps are the main way to interact with the system. They can be created using the App Class and can be registered in the
AppManager. Check the Calculator app for an example of how to create a basic app.
### Components
Components are reusable pieces of app functionality. They can be created using the Component Class can be used in
ComponentApp instances. Check the Settings app for an example of how to use predefined components, and the LAppComponent
for an example of how to create a custom component.
### Utilities
Utilities are basic functions that can be reused across the system. They are stored in the Utils class and can be called
statically.
## Installation
To install HydraOS, clone the repository and run the following command:
```bash
pio run -t upload
```
This will compile the code and upload it to the M5Cardputer. 
## Roadmap
The following features are planned for future versions of HydraOS:
1. [ ] Standalone usable components
2. [ ] More bundled apps
3. [ ] Launching of third-party apps
4. [ ] App store
## Contributing
If you would like to contribute to HydraOS, please feel free to fork the repository and submit a pull request.
You can also submit issues if you find any bugs or have any feature requests.
We are always looking for ways to improve the system and would love to hear your ideas. If you have developed an app
for HydraOS, please let us know and we will consider bundling it with the system.

## License
HydraOS is licensed under the MIT License. Please see the LICENSE file for more information.

## Detailed Overview
Please note that this is just an overview of the system. For a more detailed guide, please refer to the code itself.
### Screen
The Screen is 240x135 pixels. HydraOS StatusBar uses the top 25 pixels, the bottom, left, and right 10 pixels. You are
advised to use the coordinates (10, 25) to (230, 125) for your app's ui elements.
### Basic Classes
#### AppManager
import using `#include "AppManager.h"`

The AppManager is a class that manages the apps in the system. It can be used to register and run apps. The AppManager
is a singleton and can be accessed using the `AppManager::getInstance()` method. The AppManager has the following methods:
- `void addApp(const std::string& name, App* app)`: Adds an app to the system
- `void openApp(const std::string& name)`: Opens an app
- `void closeCurrentApp()`: Closes the current app
- `void tickCurrentApp()`: Ticks the current app
- `void draw()`: Draws the current app
- `String getCurrentAppName()`: Gets the name of the current app
- `std::map<std::string, App *> listApps()`: Lists all the apps in the system
- `App* getApp(const std::string& name)`: Gets an app by name
- `App* currentApp`: The current app, not to be changed manually
#### ScreenManager
import using `#include "ScreenManager.h"`

The ScreenManager contains two simple methods to prepare the screen for drawing:
- `void StatusBar::draw()`: Draws the status bar
- `M5Canvas canvas`: The canvas to draw on
#### Utils
import using `#include "Utils.h"`

The Utils class contains some basic utility functions that can be used across the system. The Utils class can be used 
entirely statically.
- `void Utils::waitForInput(String &input, int x = 27, int hdl_t = 10, bool passwordMode = false)`: Waits for input
- `void Utils::waitForKey()`: Waits for a key press
- `void Utils::popup(const String& text, int waitMode = 0, float textSize = 1)`: Shows a popup
- `void Utils::initCanvas()`: Resets most canvas properties, please draw status bar after using this
### App
A basic app can be created by extending the App class and implementing the following methods:
- `void onAppOpen()`: Called when the app is opened
- `void onAppClose()`: Called when the app is closed
- `void onAppTick()`: Called every tick, prepare the canvas here
- `void draw()`: Called every frame to draw the app, stamp the canvas here if needed

Check the `Calculator` app for an example of how to create a custom app.

### Component
A basic component can be created by extending the Component class and implementing the following methods:
- `void initComponent()`: Called when the component is opened
- `void renderComponent()`: Called every tick, draw the component here
- `void updateComponent()`: Called every tick, prepare canvas and update the component here
- `void closeComponent()`: Called when the component is closed
- `void forceRerender()`: Forces the component to rerender

Check the `LAppComponent` for an example of how to create a custom component.

### ComponentApp
A basic app that uses components can be created by extending the ComponentApp class and implementing the following methods:
- `Component &initApp()`: Called when the app is opened, return the component to use

Check the `Settings` app for an example of how to create a custom app that uses components.

### Registering Apps
In the `main.cpp` file, near the bottom of the setup, you can register apps using the `AppManager::getInstance()->addApp` method. For example:
```cpp
AppManager::getInstance()->addApp("calculator", new Calculator());
```

That's it! You should now have a basic understanding of how HydraOS works. 
If you have any questions, feel free to ask me on Discord: wauhundeland. Happy coding! 

