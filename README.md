# Using OpenCV With Arena SDK On Linux

Completes the example given on https://support.thinklucid.com/using-opencv-with-arena-sdk-on-linux/

The example at the above link is not readily usable.   
This repository provides the code to demonstrate the use of OpenCV with Arena SDK.

## Assumptions
- The ArenaSDK is installed at `${HOME}/localInstalls/ArenaSDK_Linux_x64`
- OpenCV is already installed and available in the system path

## Download, build, test
```bash
# Download
git clone https://github.com/mgpadalkar/arena_sdk_opencv_example.git

# build
cd arena_sdk_opencv_example
mkdir build
cd build
cmake ..
make

# test
./arena_sdk_opencv_example

# press Esc or q to stop running the example
```

## What to expect?

On the terminal you will see:
```bash
Found TRI054S-C (S/N: XXXXXXXXX)  | (Current IP: XXX.XXX.XXX.XXX) !
Starting stream.
Stopping stream.
```

Between `Starting stream.` and `Stopping stream.` a window will open showing the acquired frames.
