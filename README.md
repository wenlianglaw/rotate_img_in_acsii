# Rotate Image In Ascii

A lightweight, terminal-based 3D engine written in C++20. This project loads 2D images, extrudes them into 3D voxel spaces based on pixel brightness, and renders them in real-time as rotating ASCII art directly in your command line.

## Architecture

1. The World Class
  
    The World is the core data structure of the engine
    
    - Voxel Storage: Managing the 3D grid and mapping 3D coordinates (x, y, z) to the 1D vector.
    
    - Transformations: Computing the math for rotating and scaling the voxel grid for the next frame.
    
    - Rendering: Projecting the 3D space onto a 2D plane (Z-projection) with Double Buffering
    
    - Aspect Ratio Correction: Granular text rendering via configurable $n \times m$ horizontal and vertical character repetition (defaulting to 2x1 to create perfectly square text-pixels).

2. The ImageLoader Class

    - It uses the CImg library to read image files (like .jpg or .png) and translates the color data into a grayscale brightness map.

## The Rotating Algorithm

$$ x = x' \cos(\theta) + z' \sin(\theta) $$
$$ y = y' $$
$$ z = -x' \sin(\theta) + z' \cos(\theta) $$

where `theta` is the rotation angle.

# Requirements

- CImg: https://cimg.eu/download.html

- ImageMagick: https://imagemagick.org/download/#gsc.tab=0

### Installing Dependencies

## macOS:

```
brew install imagemagick
```

## Linux (Ubuntu/Debian):

```
sudo apt update && sudo apt install imagemagick
```

## How to Run

```sh
make
./main my_image.jpg
```

*(If you run ./main without an image path, the engine will generate a test 3D block to demonstrate the rotation).*
