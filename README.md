# png-base64-webp

`png-base64-webp` is a versatile header-only library designed to encode PNG images into base64 strings, optimized with WebP compression. This library offers a seamless way to reduce the size of your PNG images while maintaining the convenience of base64 encoding.

## Table of Contents

- [png-base64-webp](#png-base64-webp)
  - [Table of Contents](#table-of-contents)
  - [Installation](#installation)
  - [Usage](#usage)
  - [Building the Example](#building-the-example)
  - [Dependencies](#dependencies)
  - [Dependency Installation](#dependency-installation)
    - [Debian (and Ubuntu)](#debian-and-ubuntu)
    - [Fedora](#fedora)
    - [Arch Linux (and Manjaro)](#arch-linux-and-manjaro)
    - [From Source](#from-source)
  - [Decoding the Base64 String](#decoding-the-base64-string)
    - [C](#c)
    - [C++](#c-1)
    - [JavaScript](#javascript)
    - [Node.JS](#nodejs)
    - [Python](#python)
  - [Online Demo](#online-demo)
  - [Notes](#notes)

## Installation

To integrate `png-base64-webp` into your project, simply include the `png-base64-webp.h` header.

## Usage

To utilize the library, include the header and invoke the `encodePNGToBase64` function:

```c
#include "png-base64-webp.h"

int main() {
    const char* imagePath = "/path/to/your/image.png";
    char* encodedData = encodePNGToBase64(imagePath);
    if (encodedData) {
        // Utilize the encoded data
        free(encodedData);  // Ensure to release the memory post usage
    }
    return 0;
}
```

## Building the Example

To compile the provided example:

```bash
mkdir -p build
cd build
cmake ..
make
```

Execute the generated binary to encode the image `example.png`:

```bash
./png-base64-webp ../example/example.png
```

## Dependencies

- `libpng`: Essential for PNG image processing.
- `zlib`: A requisite for `libpng`.
- `libwebp`: For WebP compression.

To verify the installation of `libpng` and `zlib` on your system:

```bash
pkg-config --exists libpng && echo "libpng: Installed" || echo "libpng: Not Installed"; pkg-config --exists zlib && echo "zlib: Installed" || echo "zlib: Not Installed"
```

## Dependency Installation

### Debian (and Ubuntu)

```bash
sudo apt update
sudo apt install libpng-dev zlib1g-dev libwebp-dev
```

### Fedora

```bash
sudo dnf install libpng-devel zlib-devel libwebp-devel
```

### Arch Linux (and Manjaro)

```bash
sudo pacman -S libpng zlib libwebp
```

### From Source

If you prefer to compile from source or if your distribution isn't listed above:

1 . Fetch the source for [zlib](https://zlib.net/), [libpng](http://www.libpng.org/pub/png/libpng.html), and [libwebp](https://developers.google.com/speed/webp/download).

2 . Compile and install `zlib`:

```bash
git clone https://github.com/madler/zlib.git
cd zlib
./configure
make
sudo make install
```

3 . Compile and install `libpng`:

```bash
git clone https://github.com/glennrp/libpng.git
cd libpng
./configure
make
sudo make install
```

4 . Compile and install `libwebp`:

```bash
git clone https://chromium.googlesource.com/webm/libwebp
mkdir build && cd build && cmake ../
make
sudo make install
```

After installing the dependencies, you can include and use the `png-base64-webp` library in your projects.

## Decoding the Base64 String

To decode the generated base64 string back to a WebP file:

### C

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <webp/decode.h>

void decodeBase64ToWebP(const char* base64String, const char* outputPath) {
    // Assuming you have a function to decode base64 to binary
    size_t binarySize;
    uint8_t* binaryData = decodeBase64(base64String, &binarySize);

    FILE* file = fopen(outputPath, "wb");
    if (file) {
        fwrite(binaryData, 1, binarySize, file);
        fclose(file);
    }

    free(binaryData);
}
```

### C++

```cpp
#include <fstream>
#include <string>
#include <vector>
#include <webp/decode.h>

void decodeBase64ToWebP(const std::string& base64String, const std::string& outputPath) {
    // Assuming you have a function to decode base64 to binary
    std::vector<uint8_t> binaryData = decodeBase64(base64String);

    std::ofstream file(outputPath, std::ios::binary);
    file.write(reinterpret_cast<char*>(binaryData.data()), binaryData.size());
}
```

### JavaScript

```javascript
function decodeBase64ToWebP(base64String, filename) {
    const binaryData = atob(base64String);
    const len = binaryData.length;
    const bytes = new Uint8Array(len);
    for (let i = 0; i < len; i++) {
        bytes[i] = binaryData.charCodeAt(i);
    }
    const blob = new Blob([bytes], {type: "image/webp"});
    const link = document.createElement('a');
    link.href = window.URL.createObjectURL(blob);
    link.download = filename;
    link.click();
}
```

### Node.JS

```javascript
const fs = require('fs');

function decodeBase64ToWebP(base64String, outputPath) {
    const binaryData = Buffer.from(base64String, 'base64');
    fs.writeFileSync(outputPath, binaryData);
}
```

### Python

```python
import base64

def decode_base64_to_webp(base64_string, output_path):
    with open(output_path, 'wb') as f:
        f.write(base64.b64decode(base64_string))
```

## Online Demo

Test the generated base64 string and decode it back to a WebP image using the online demo: [https://sayom.me/png-base64-webp](https://sayom.me/png-base64-webp/)

For a hands-on example of the decoding process, refer to the script element in `<project_root>/docs/index.html`.

## Notes

- Post invocation, `encodePNGToBase64` returns a dynamically allocated string. It's imperative for the caller to manage this memory.
- For multi-threaded applications, ensure thread safety, especially if `libpng` or `libwebp` is concurrently accessed elsewhere in your application.
