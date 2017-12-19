# PMSoS (Parallel Magic Square of Squares)
A program written in C to search for magic squares of perfect squares.

## Development
The IDE is Code::Blocks (http://www.codeblocks.org/). The source code requires the GNU
Multiple Precision Arithmetic Library (https://gmplib.org/).

## Usage
Once started, the program requests a so called generator string.

The generator string consists of a number (called the generator ![equation](https://latex.codecogs.com/png.latex?g)) followed optionally by a `+` (default) or `-` char. The program would then calculate ![equation](https://latex.codecogs.com/png.latex?c%20%3D%206%20%5Ccdot%20g%20&plus;%201) in case of `+` and ![equation](https://latex.codecogs.com/png.latex?c%20%3D%206%20%5Ccdot%20g%20-%201) in case of `-`.

This guarantees, that ![equation](https://latex.codecogs.com/png.latex?c%5E2%20%5Cequiv%201%7E%28%5Ctextrm%7Bmod%7D%7E24%29).
