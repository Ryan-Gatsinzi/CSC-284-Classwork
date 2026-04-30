// #include <iostream>
// #include <string>
// #include <cmath>

// #define STB_IMAGE_IMPLEMENTATION
// #include "stb_image.h"

// using namespace std;


// int main(){
//     const string BRIGHTNESS_GRADIENT = "@%#*+=-:.";

//     string fileName;
//     int scaleFactor = 1;

//     cout << "Enter image file: ";
//     getline(cin, fileName);

//     cout << "Enter scale factor: ";
//     cin >> scaleFactor; cin.ignore(100, '\n');

//     int width, height, channels;
//     unsigned char* data;

//     try{

//         data = stbi_load(
//             fileName.c_str(),
//             &width,
//             &height,
//             &channels,
//             0
//         );
//     }catch(...){
//         cout << "Failed to load file "  << fileName; 
//         return 1;
//     }
    
//     cout << width << " x " << height;

//     for(int y = 0; y < height; y+=scaleFactor*2){
//         for (int x = 0; x < width; x+=scaleFactor){
           
//             int index = (y * width + x) * channels;
        
//             int r = data[index + 0];
//             int g = data[index + 1];
//             int b = data[index + 2];
//             double brightness = 0.299*r + 0.587*g + 0.114*b;

//             int divisor = 255 / (BRIGHTNESS_GRADIENT.size()-1);
//             int ascii_index = round(brightness/divisor);

//             cout << BRIGHTNESS_GRADIENT[ascii_index];
//         }
//         cout << endl;
//     }





// }

#include <iostream>
#include <string>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

using namespace std;

int main() {
    const string BRIGHTNESS_GRADIENT = "@%#*+=-:. ";

    string fileName;
    int scaleFactor = 1;

    cout << "Enter image file: ";
    getline(cin, fileName);

    cout << "Enter scale factor: ";
    cin >> scaleFactor;

    if (scaleFactor <= 0) {
        cout << "Scale factor must be positive.\n";
        return 1;
    }

    int width, height, channels;

    unsigned char* data = stbi_load(
        fileName.c_str(),
        &width,
        &height,
        &channels,
        3
    );

    if (!data) {
        cout << "Failed to load file: " << fileName << "\n";
        return 1;
    }

    channels = 3;

    cout << width << " x " << height << "\n";

    for (int y = 0; y < height; y += scaleFactor * 2) {
        for (int x = 0; x < width; x += scaleFactor) {
            int index = (y * width + x) * channels;

            int r = data[index + 0];
            int g = data[index + 1];
            int b = data[index + 2];

            double brightness = 0.299 * r + 0.587 * g + 0.114 * b;

            int ascii_index =
                brightness * (BRIGHTNESS_GRADIENT.size() - 1) / 255;

            cout << BRIGHTNESS_GRADIENT[ascii_index];
        }

        cout << '\n';
    }

    stbi_image_free(data);
}