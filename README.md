# ImageSteganography

[![Build Status](https://img.shields.io/badge/build-passing-brightgreen.svg)](https://github.com/yourusername/ImageSteganography/actions) [![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT) [![C++11](https://img.shields.io/badge/C%2B%2B-11-blue.svg)](https://en.cppreference.com/w/cpp/11)

A lightweight C++ library and command-line tool for **image steganography** using Least Significant Bit (LSB) substitution. This implementation enables secure embedding and extraction of text messages within common image formats (BMP, PNG, JPEG) with minimal visual artifacts.

## Overview

ImageSteganography provides a simple, efficient way to conceal sensitive data in digital images. By altering the least significant bits of pixel values, messages are hidden without detectable changes to the image's appearance. The library supports:

- **Encoding**: Embed a message with automatic capacity validation.
- **Decoding**: Extract messages with length-prefixed integrity checks.
- **Format Compatibility**: BMP, PNG, and JPEG (lossy compression may introduce minor noise).

This tool is ideal for educational purposes, secure communication prototypes, or lightweight data hiding applications.

## Table of Contents
- [Features](#features)
- [Requirements](#requirements)
- [Installation](#installation)
- [Usage](#usage)
- [API Reference](#api-reference)
- [Algorithm Details](#algorithm-details)
- [Limitations](#limitations)
- [Troubleshooting](#troubleshooting)
- [Contributing](#contributing)
- [License](#license)

## Features
- **Cross-Format Support**: Load and save images in BMP, PNG, and JPEG formats using STB libraries.
- **Capacity Estimation**: Pre-checks image size to ensure the message fits (requires ~4 extra bytes for length header).
- **Error-Resilient**: Comprehensive error handling for file I/O, insufficient pixels, and invalid data.
- **Efficient Pixel Traversal**: Sequential access across all channels (RGB/RGBA) for optimal performance.
- **No External Dependencies**: Relies solely on standard C++ and STB single-header libraries.
- **Command-Line Interface**: Interactive menu for quick encoding/decoding.

## Requirements
- **Compiler**: C++11 or later (e.g., GCC 4.8+, Clang 3.3+, MSVC 2013+).
- **Libraries**:
  - STB Image (`stb_image.h`) and STB Image Write (`stb_image_write.h`)—single-header, public domain.
- **Platform**: Cross-platform (Linux, macOS, Windows); tested on x86_64 architectures.

## Installation
1. **Download STB Libraries**:
   - Clone or download [STB](https://github.com/nothings/stb) repository.
   - Copy `stb_image.h` and `stb_image_write.h` to your project directory.

2. **Build the Project**:
   Create `main.cpp` (CLI entrypoint) and `ImageSteganography.cpp` (implementation) from the provided source code.
   ```bash
   # Compile all into executable
   g++ -std=c++11 -o stego main.cpp ImageSteganography.cpp

   # Or with optimization (optional)
   g++ -std=c++11 -O2 -o stego main.cpp ImageSteganography.cpp

   # Or with CMake (recommended for larger projects)
   mkdir build && cd build
   cmake ..
   make
   ```

3. **Run**:
   ```bash
   ./stego
   ```

## Usage
The CLI provides an interactive menu for encoding and decoding.

### Encoding a Message
```bash
Choose option (1 or 2): 1
Enter input image path (bmp/png/jpg): ./input.png
Enter output image path (bmp/png/jpg): ./output.png
Enter secret message: [Your message here]
```
- **Output**: "Success!" if encoded; otherwise, an error (e.g., "Max capacity: 12345 bytes").

### Decoding a Message
```bash
Choose option (1 or 2): 2
Enter image path with hidden message: ./output.png
```
- **Output**: "Hidden message: [Decoded text]" if successful.

### Programmatic Usage (Library)
Integrate the class directly:
```cpp
#include "ImageSteganography.h"

int main() {
    ImageSteganography steg;
    std::string message;
    if (steg.decode("stego_image.png", message)) {
        std::cout << "Decoded: " << message << std::endl;
    }
    return 0;
}
```

## API Reference
### Constructor/Destructor
- `ImageSteganography()`: Default constructor.
- `~ImageSteganography()`: Destructor; calls `clear()`.

### Public Methods
| Method | Description | Parameters | Returns |
|--------|-------------|------------|---------|
| `bool encode(const std::string& inputImage, const std::string& outputImage, const std::string& message)` | Encodes message into image and saves to output. | `inputImage`: Path to source image.<br>`outputImage`: Path for modified image.<br>`message`: Text to hide. | `true` on success. |
| `bool decode(const std::string& inputImage, std::string& message)` | Decodes and extracts message. | `inputImage`: Path to stego image.<br>`message`: Output reference for decoded text. | `true` on success. |
| `void clear()` | Releases pixel data and resets dimensions. | - | - |

### Private Helpers (Internal)
- `void setBit(uint8_t& byte, int bit, int value)`: Sets/clears a bit in a byte.
- `int getBit(uint8_t byte, int bit) const`: Retrieves a bit from a byte.
- `std::string getFileExtension(const std::string& filename) const`: Extracts lowercase extension.
- `bool loadImage(const std::string& inputImage)`: Loads image into `pixelData`.
- `bool saveImage(const std::string& outputImage)`: Saves `pixelData` to file.

## Algorithm Details
1. **Encoding**:
   - Load image pixels into a contiguous buffer.
   - Compute capacity: `(width * height * channels / 8) - 4` bytes.
   - Encode 32-bit length header into LSB of first 32 pixels.
   - For each message byte: Encode 8 bits into subsequent pixels' LSB (bit 0).
   - Save modified image.

2. **Decoding**:
   - Load pixels.
   - Extract 32-bit length from first 32 LSBs.
   - Reconstruct message by reading `length * 8` LSBs into bytes.
   - Validate: Length > 0 and ≤ capacity.

**Security Note**: LSB is simple and fast but vulnerable to statistical analysis. For production, combine with encryption (e.g., AES on the message before encoding).

## Limitations
- **No Encryption**: Messages are plaintext; add your own cipher for confidentiality.
- **Lossy Formats**: JPEG compression post-encoding may corrupt data—prefer PNG/BMP for fidelity.
- **Capacity**: Limited by image size; e.g., 512x512 RGB image holds ~98KB (minus header).
- **No Multi-Message Support**: Overwrites any existing hidden data.
- **Alpha Channel**: Included in traversal but may affect transparency in PNGs.

## Troubleshooting
| Issue | Possible Cause | Solution |
|-------|----------------|----------|
| "Cannot load image" | Invalid path/format. | Verify file exists; use supported extensions. Check `stbi_failure_reason()`. |
| "Insufficient pixels" | Message too long/small image. | Use larger images or shorter messages. |
| Compilation errors | Missing STB headers. | Ensure `stb_image.h` and `stb_image_write.h` are in include path. |
| Distorted output | High JPEG quality not set. | Library uses quality=95; adjust in `saveImage` if needed. |

For detailed logs, compile with `-DDEBUG` flag (add custom logging).

## Contributing
Contributions welcome! Please:
1. Fork the repository.
2. Create a feature branch (`git checkout -b feature/AmazingFeature`).
3. Commit changes (`git commit -m 'Add some AmazingFeature'`).
4. Push to branch (`git push origin feature/AmazingFeature`).
5. Open a Pull Request.

Focus areas: Encryption integration, GUI wrapper, batch processing, or format extensions (e.g., TIFF).

## License
This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

---

**Author**: [Your Name]  
**Version**: 1.0.0  
**Last Updated**: November 04, 2025  

For questions or support, open an issue on GitHub.
