#ifndef IMAGE_STEGANOGRAPHY_H
#define IMAGE_STEGANOGRAPHY_H

#include <vector>
#include <string>
#include <cstdint>

class ImageSteganography {
private:
    std::vector<uint8_t> pixelData;
    int width = 0, height = 0, channels = 0;
    
    void setBit(uint8_t& byte, int bit, int value);
    int getBit(uint8_t byte, int bit) const;
    std::string getFileExtension(const std::string& filename) const;
    bool loadImage(const std::string& inputImage);
    bool saveImage(const std::string& outputImage);

public:
    ImageSteganography() = default;
    ~ImageSteganography();
    void clear();
    
    bool encode(const std::string& inputImage, const std::string& outputImage, const std::string& message);
    bool decode(const std::string& inputImage, std::string& message);
};

#endif // IMAGE_STEGANOGRAPHY_H
