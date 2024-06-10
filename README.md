# Dracord
Dracord is an application built within SDL2 that uses the Discord API under the hood.


> [!WARNING]
> This is a beginner project and my first proper project within C++. <br/>
> Do not expect some features to work correctly or to be implemented at all.


## Features
- [x] Access to user's friends, guilds, channels, and messages
- [x] Error handling for various HTTP status codes

## Planned Features
> [!CAUTION]
> These features could not be implemented at all. <br/>
> They're here for features I would "like" to add, but aren't certain to be added.

- [ ] Screensharing
- [ ] Functional Application
- [ ] Windows support
- [ ] Login through email and password (rather than token)
- [ ] 2FA support

## Prerequisites
Before you begin, ensure you have met the following requirements:

- You have the following libraries installed either through a package manager or your OS of choice.
  ```
  SDL2
  SDL2_Image
  JSONCPP
  CPR
  ```
- Ensure you have `make` installed on your operating system.
- You have a Discord account (obvious one)

## Installing Discord

To install the Dracord, follow these steps:

1. Clone the repository
2. Navigate to the project directory
3. Run `make` command

## Using Dracord

To use the Dracord, follow these steps:

1. Run the application
2. Enter your Discord token when prompted

## Contributing to Dracord

To contribute to Dracord, follow these steps:

1. Fork the repository
2. Create a new branch: `git checkout -b '<branch_name>'`
3. Make your changes and commit them: `git commit -m '<commit_message>'`
4. Push to the original branch: `git push origin '<project_name>/<location>'`
5. Create the pull request

## License

This project uses the following license: [MIT LICENSE](https://github.com/aitezera/dracord/blob/main/LICENSE).