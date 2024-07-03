# Dracord
Dracord is an application built within SDL2 that uses the Discord API under the hood.

> [!WARNING]
> ### IN VERY EARLY DEVELOPMENT
> This is a beginner project and my first proper project within C++. <br/>
> Do not expect some features to work correctly or to be implemented at all.

## Features
- [x] Access to users friends, guilds, channels, and messages
- [x] Error handling for various HTTP status codes and functions/calls
- [x] Saves token and encrypts into a binary file (Can also reload the token)

## Planned Features
> [!CAUTION]
> These features could not be implemented at all. <br/>
> They're here for features I would "like" to add, but aren't certain to be added.

- [ ] Screensharing
- [ ] Functional Application
- [ ] Windows support
- [ ] Login through email and password (rather than token)
- [ ] 2FA support
- [ ] Websocket support (listening for event types from Discord to make changes live)
- [ ] Fix the naming conventions used throughout the source code. (Help maintain code)

## Prerequisites
Before you begin, ensure you have met the following requirements:

- Install the following libraries either through a package manager or your OS of choice.
  ```
  SDL2pp
  JSONCPP
  CPR
  ```
  If you're using Arch Linux you can use yay with the following command <br>
  ` yay -Sy sdl2pp cpr && sudo pacman -Sy jsoncpp `

  Windows users, I would much recommended installing vcpkg and running the command <br>
  ` vckpkg install sdl2pp jsoncpp cpr `

- Ensure CMake is installed on your system. It can be installed from their officical website: [CMake Website](https://cmake.org/download/) <br>
  Arch users can install CMake through pacman -> ` sudo pacman -Sy cmake `

- Have a Discord account (obvious one)

## Installing Dracord

To install the Dracord, follow these steps: 

1. Clone the repository
2. Navigate to the project directory
3. Create a folder called build ` mkdir build `
4. Change Directory to build ` cd build `
5. Run ` cmake -S .. -B . ` 
> [!IMPORTANT]
> If you're using vcpkg you will need to specify the path to vcpkg.cmake using `-DCMAKE_TOOLCHAIN_FILE=` <br>
> To find vcpkg.make it is normally under your vcpkg path file.<br>Example: `C:\User\[NAME]\vcpkg\scripts\buildsystems\`
6. Run ` cmake --build . `
> [!IMPORTANT]
> You may also specify certain targets that I had created for testing purposes. Those being client and requests <br>
> To run them use the following command ` cmake --build . --target [TARGET_NAME] `, replacing [TARGET_NAME] with `client` or `requests`.

## Using Dracord

To use the Dracord, follow these steps:

1. Run the application 
2. Enter your Discord token when prompted (This will not be asked for the next time)

## Contributing to Dracord

To contribute to Dracord, follow these steps:

1. Fork the repository
2. Create a new branch: `git checkout -b '<branch_name>'`
3. Make your changes and commit them: `git commit -m '<commit_message>'`
4. Push to the original branch: `git push origin '<project_name>/<location>'`
5. Create the pull request

## License

This project uses the following license: [MIT LICENSE](https://github.com/aitezera/dracord/blob/main/LICENSE).