#include "ImageSteganography.h"
#include <iostream>
#include <algorithm>  
#include <cctype>     

//single header libraries that gotta use
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image.h"
#include "stb_image_write.h"

using namespace std;

=
void ImageSteganography::setBit(uint8_t& byte, int bit, int value) {
    if (value) {
        byte |= (1u << bit);  //unsigned shift
    } else {
        byte &= ~(1u << bit);
    }
}

int ImageSteganography::getBit(uint8_t byte, int bit) const {
    return (byte >> bit) & 1;
}

string ImageSteganography::getFileExtension(const string& filename) const {
    size_t pos = filename.find_last_of('.');
    if (pos != string::npos) {
        string ext = filename.substr(pos + 1);
        transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
        return ext;
    }
    return "";
}

bool ImageSteganography::loadImage(const string& inputImage) {
    unsigned char* data = stbi_load(inputImage.c_str(), &width, &height, &channels, 0);
    if (!data) {
        cerr << "Error: Cannot load image " << inputImage << endl;
        cerr << "Reason: " << stbi_failure_reason() << endl;
        return false;
    }
    cout << "Image loaded: " << width << "x" << height << " with " << channels << " channels" << endl;
    
    size_t dataSize = static_cast<size_t>(width) * height * channels;
    pixelData.assign(data, data + dataSize);
    stbi_image_free(data);
    return true;
}

bool ImageSteganography::saveImage(const string& outputImage) {
    string ext = getFileExtension(outputImage);
    int stride = width * channels;
    bool success = false;
    
    if (ext == "png") {
        success = stbi_write_png(outputImage.c_str(), width, height, channels, pixelData.data(), stride);
    } else if (ext == "bmp") {
        success = stbi_write_bmp(outputImage.c_str(), width, height, channels, pixelData.data());
    } else if (ext == "jpg" || ext == "jpeg") {
        success = stbi_write_jpg(outputImage.c_str(), width, height, channels, pixelData.data(), 95);
    } else {
        cerr << "Error: Unsupported output format." << endl;
        return false;
    }
    
    if (!success) {
        cerr << "Failed to write output image" << endl;
        return false;
    }
    
    cout << "Image saved to " << outputImage << endl;
    return true;
}


ImageSteganography::~ImageSteganography() {
    clear();
}

void ImageSteganography::clear() {
    pixelData.clear();
    width = height = channels = 0;
}

bool ImageSteganography::encode(const string& inputImage, const string& outputImage, const string& message) {
    if (!loadImage(inputImage)) return false;
    
    size_t dataSize = pixelData.size();
    uint32_t messageLength = static_cast<uint32_t>(message.length());
    uint32_t maxCapacity = static_cast<uint32_t>(dataSize / 8) - 4;
    
    if (messageLength > maxCapacity) {
        cerr << " Max capacity: " << maxCapacity << " bytes" << endl;
        clear();
        return false;
    }
    
    cout << "Encoding message of " << messageLength << " bytes..." << endl;
    
    size_t pixelIndex = 0;
    
    //encode 32 bits
    for (int i = 0; i < 32; ++i) {
        int bit = (messageLength >> i) & 1;
        if (pixelIndex >= dataSize) {
            cerr << "Insufficient pixels for header" << endl;
            clear();
            return false;
        }
        setBit(pixelData[pixelIndex], 0, bit);
        ++pixelIndex;
    }
    
    
    for (char c : message) {
        for (int bit = 0; bit < 8; ++bit) {
            if (pixelIndex >= dataSize) {
                cerr << "Error: Insufficient pixels for message" << endl;
                clear();
                return false;
            }
            int bitValue = (static_cast<unsigned char>(c) >> bit) & 1;
            setBit(pixelData[pixelIndex], 0, bitValue);
            ++pixelIndex;
        }
    }
    
    return saveImage(outputImage);
}

bool ImageSteganography::decode(const string& inputImage, string& message) {
    if (!loadImage(inputImage)) return false;
    
    size_t dataSize = pixelData.size();
    size_t pixelIndex = 0;
    
    // Decode message length
    uint32_t messageLength = 0;
    for (int i = 0; i < 32; ++i) {
        if (pixelIndex >= dataSize) {
            cerr << "Error: Insufficient pixels for header" << endl;
            clear();
            return false;
        }
        int bit = getBit(pixelData[pixelIndex], 0);
        messageLength |= (static_cast<uint32_t>(bit) << i);
        ++pixelIndex;
    }
    
    if (messageLength == 0 || messageLength > dataSize / 8) {
        cerr << "Invalid message length or no hidden message found" << endl;
        clear();
        return false;
    }
    
    cout << "Decoding message of " << messageLength << " bytes..." << endl;
    
    //decode message
    message.clear();
    message.reserve(messageLength);
    for (uint32_t i = 0; i < messageLength; ++i) {
        uint8_t c = 0;
        for (int bit = 0; bit < 8; ++bit) {
            if (pixelIndex >= dataSize) {
                cerr << "Error: Truncated message" << endl;
                clear();
                return false;
            }
            int bitValue = getBit(pixelData[pixelIndex], 0);
            c |= (static_cast<uint8_t>(bitValue) << bit);
            ++pixelIndex;
        }
        message += static_cast<char>(c);
    }
    
    cout << "Message successfully decoded!" << endl;
    return true;
}
