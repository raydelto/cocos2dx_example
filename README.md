# Simple Cocos2d-x example

This is a simple game created using the Cocos2d-x C++ framework.

# Happy Bunny
This is a simple game about a bunny that has to catch the carrots and avoid the bombs. With this simple example you'll get some snippets that would help you with:

1. Cocos2d-x Physics
2. Transitions between Scenes
3. True Type Fonts
4. Bitmap Fonts
5. Handling touch events
6. Playing , pausing and changing audio volume
7. Sprite handling
8. Cocos2d-x Schedulers
9. Use the accelerometer 

# About Cocos2d-x

[Cocos2d-x](http://www.cocos2d-x.org "Cocos2d-x  page") is a game development framework that supports C++, javascript or lua for creating 2D and 3D games.

This framework allows you to deploy in the following platforms using the same codebase:

1. Android
2. iOS (iPhone and iPad)
3. Windows (win32 , win 8.1 and win 10)
4. Linux
5. Mac OS
6. Tizen
7. QNX BlackBerry X (Only branch 2 of Cocos2d-x)

## Building the Game

### Linux

In order to build this code you will need to follow these steps:

1. Download the latest  [Cocos2d-x version](http://www.cocos2d-x.org/download "Cocos2d-x download page")
2. Extract the zip file
3. Execute the `setup.py` script (Make sure taht you have Python 2.7 installed on your system)
4. Restart the console
5. Execute the `download-deps.py` script
6- Execute the `install-deps-linux.sh` script
7. Create a new Cocos Project executing the following command: `cocos new HappyBunny -p com.yourname.happybunny -l cpp -d ./projects`
8. Copy the ./projects/HappyBunny/cocos2d folder into the directory where you have cloned this repo
9. Execute `cocos build -p linux`

## Executing the game

Once you have followed the steps described in the previous sections you will need to execute the following command in order to execute the game:

`cocos run -p linux`

If you are interested in Cocos2d-x C++ game development take a look at the [book](https://www.amazon.com/Building-Android-Cocos2d-x-Raydelto-Hernandez/dp/1785283839 "Cocos2d-x book")  that I wrote about it

If you have any comment feel free to contact me:

Raydelto Hernandez raydelto@yahoo.com

![Game Screen Shot](http://www.raydelto.org/bunny.png "Screen shot")
