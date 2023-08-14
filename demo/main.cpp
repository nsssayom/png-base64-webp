#include <iostream>
#include <string>
#include "png-base64.h"

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <path_to_image.png>" << std::endl;
        return 1;
    }

    std::string image_path = argv[1];
    char* encodedData = encodePNGToBase64(image_path.c_str());

    if (encodedData) {
        std::string image_base64(encodedData);
        std::cout << "Base64 encoded data: " << image_base64 << std::endl;
        // print the raw size of the string with unit
        std::cout << "Base64 String size: " << image_base64.size() << " bytes" << std::endl;
        free(encodedData);  // Free the dynamically allocated memory
    } else {
        std::cerr << "Failed to encode the image." << std::endl;
    }

    return 0;
}
