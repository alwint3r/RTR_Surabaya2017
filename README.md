Road To RIoT Surabaya
=====================

These code are shown during my presentation at Road to RIoT Surabaya event which is held at July 26, 2017.

You can view & download my slides [here](https://www.slideshare.net/alwin_wint3r/introduction-to-esp32-programming-road-to-riot-2017).

### Building and flashing the Source

Each directory is a project that uses ESP-IDF except `arduino_with_platformio`, it uses PlatformIO as the development tool and Arduino as the framework.

To build the source run one of this commands:

```bash
make flash
```

or alternatively you can provide `-jN` flag where `N` is the number task that will be run during the build process.

For PlatformIO project, run this command:

```bash
platformio run -t upload
```

or simply

```bash
pio run -t upload
```

