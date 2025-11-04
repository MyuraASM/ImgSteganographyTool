#include <iostream>
#include "ImageSteganography.h"

using namespace std;

int main() {
    ImageSteganography steg;
    int choice;
    
    cout << "1. Encode message into image" << endl;
    cout << "2. Decode message from image" << endl;
    cout << "Choose option (1 or 2): ";
    cin >> choice;
    cin.ignore();  
    
    if (choice == 1) {
        string inputImage, outputImage, message;
        cout << "Enter input image path (bmp/png/jpg): ";
        getline(cin, inputImage);
        cout << "Enter output image path (bmp/png/jpg): ";
        getline(cin, outputImage);
        cout << "Enter secret message: ";
        getline(cin, message);
        
        if (steg.encode(inputImage, outputImage, message)) {
            cout << "Success!" << endl;
        } else {
            cout << "Failed to encode message." << endl;
        }
    } else if (choice == 2) {
        string inputImage, message;
        cout << "Enter image path with hidden message: ";
        getline(cin, inputImage);
        
        if (steg.decode(inputImage, message)) {
            cout << "Hidden message: " << message << endl;
        } else {
            cout << "Failed to decode message." << endl;
        }
    } else {
        cout << "Invalid choice!" << endl;
    }
    
    return 0;
}
