# Gesture based SRA Board controller
The project sets up the [SRA esp32 board](https://github.com/SRA-VJTI/sra-board-component) to act as a websocket server, which receives real time messages from a websocket client depending on the gesture of the user, which in turn is detected by the opencv (c++) library.

## Dependencies
* [IXWebsocket]()
    * Used for creating a websocket client to act along aside the gesture detection code
* [Opencv]()
    * Used to detect and analyze hand gesture's through the webcam
* [SRA Board](https://github.com/SRA-VJTI/sra-board-component) 
* ESP-IDF
* FreeRTOS