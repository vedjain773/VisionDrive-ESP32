# Gesture based SRA Board controller
The project sets up the [SRA esp32 board](https://github.com/SRA-VJTI/sra-board-component) to act as a websocket server, which receives real time messages from a websocket client depending on the gesture of the user, which in turn is detected by the opencv (c++) library.

## Dependencies
* [IXWebsocket](https://github.com/machinezone/IXWebSocket.git)
* [Opencv](https://github.com/opencv/opencv.git)
* [SRA Board](https://github.com/SRA-VJTI/sra-board-component) 
* [ESP-IDF](https://github.com/espressif/esp-idf)
* FreeRTOS

## Usage
To use the project, first clone the repository
```
git clone https://github.com/vedjain773/VisionDrive-ESP32.git
```

Next, switch to the board-control project and setup your esp-idf environment
```
cd board-control/
get_idf
``` 

After this, configure the wifi settings using idf.py menuconfig
```
idf.py menuconfig
```
In the menu, go to the example configuration section and enter your Wifi SSID and password and save it.

After the configuration is saved, build and flash the firmware on to the SRA Board
```
idf.py build
idf.py flash monitor
```

If everything is done correctly up to this point, you should see the IP address of the websocket server hosted by the SRA Board.

Now in a new terminal, switch to the gestureAnalysis folder
```
cd gestureAnalysis/
```

Next, open the main.cpp file and change the IP address on line 17 with the one you obtained from the websocket server and save the changes made to the file.

After this, compile the main.cpp file
```
make build SRC=main.cpp
```

After the file is compiled, execute the exe file
```
./gesture_analysis
```

If all goes according to plan, a connection should now be successfully established between the websocket client and server
Now you can successfully control the SRA Board's LEDs with hand gestures!

## Acknowledgements and Credits:
The code for the websocket client was obtained from the official example sections of the IXWebsocket documentation.
[IXWebsocket Documentation](https://machinezone.github.io/IXWebSocket/usage/)

The code for the websocket server was a modified version of the code provided by in the tutorial.
[ESP32 websocket server tutorial](https://esp32tutorials.com/esp32-esp-idf-websocket-web-server/)