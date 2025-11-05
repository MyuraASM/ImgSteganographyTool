# ImageSteganography

A lightweight C++ library and command-line tool for image steganography using Least Significant Bit (LSB) substitution. Embed and extract text messages in BMP, PNG, or JPEG images with no visible changes.

## Overview

This tool hides text in images by tweaking the least significant bits of pixels. 

## Features
- Load/save BMP, PNG, JPEG via STB libraries.
- Checks if your message fits before encoding.
- Handles errors like bad files or tiny images.
- Works on RGB/RGBA; traverses pixels sequentially.
- No deps beyond C++ and STB headers.
- Simple CLI menu.

## Requirements
- C++11+ compiler (GCC, Clang, etc.).
- STB Image and STB Image Write headers (public domain, single-file).

## Installation
1. Grab `stb_image.h` and `stb_image_write.h` from [nothings/stb](https://github.com/nothings/stb) and drop them in your dir.

2. Build:
   ```bash
   # Compile all into executable
   g++ -std=c++11 -o stego main.cpp ImageSteganography.cpp

   # Or with optimization
   g++ -std=c++11 -O2 -o stego main.cpp ImageSteganography.cpp
   ```

3. Run:
   ```bash
   ./stego
   ```

## Usage
Interactive menu for encode/decode.

### Encode
```
Choose option (1 or 2): 1
Enter input image path (bmp/png/jpg): input.png
Enter output image path (bmp/png/jpg): output.png
Enter secret message: Hello from the shadows.
```
Success if it fits; warns on capacity.

### Decode
```
Choose option (1 or 2): 2
Enter image path with hidden message: output.png
```
Prints the message if found.

### As a Library
```cpp
#include "ImageSteganography.h"

int main() {
    ImageSteganography steg;
    std::string msg;
    if (steg.decode("image.png", msg)) {
        std::cout << msg << std::endl;
    }
    return 0;
}
```

## API
### Public
- `bool encode(in_path, out_path, message)`: Hide and save.
- `bool decode(in_path, &message)`: Pull out message.
- `void clear()`: Free memory.



## How It Works
**Encode**:
- Load pixels.
- Calc space: (pixels / 8) - 4 bytes for length.
- Stuff 32-bit len into first 32 LSBs.
- Each message byte: 8 pixels' LSBs.
- Save.

**Decode**:
- Load.
- Read len from first 32 bits.
- Grab next len*8 bits for chars.











---
Миура
Version 1.0.0
