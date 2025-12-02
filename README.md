# MountainPaths (PrPr-Bergpfade): Pathfinding on Mountain Terrain

**MountainPaths** is a C-based graphical and algorithmic tool for analyzing elevation data and computing the optimal (least-effort) paths across mountain terrain. It is built as part of the *Programmier Projekt* course and uses SDL2 for visual output.


## Features

- Reads terrain data from binary matrix files.
- Implements pathfinding algorithms to compute optimal hiking paths.
- Visualizes paths and elevation data using SDL2 graphics.
- Command-line interface for flexible usage.


## Dependencies

To build and run MountainPaths, you’ll need the following:

### Build Tools
- `gcc` or any modern C compiler
- `make`, `autoconf`, `automake`, `libtool` (for building via Autotools)

### SDL2 and libraries
Install using your package manager (Debian/Ubuntu example):

```bash
sudo apt install libsdl2-dev libsdl2-image-dev libsdl2-ttf-dev libsdl2-mixer-dev
```


## Installation and Build

This project uses **Autotools** and supports out-of-source builds.

### Step-by-step:
#### OPTION 1: with the build script
```bash
git clone https://gitlab.oth-regensburg.de/mar33051/pp-bergpfade-malhotra-koeberl.git
cd pp-bergpfade-malhotra-koeberl

# Run the provided build script
./build.sh
```

> To clean the build folder, run:  
> `./build.sh clean`

#### OPTION 2: Manual build
```bash
git clone https://gitlab.oth-regensburg.de/mar33051/pp-bergpfade-malhotra-koeberl.git
cd pp-bergpfade-malhotra-koeberl

# Create out-of-source build directory
mkdir build
cd build

# Configure the build system
../configure

# Build the project
make
```


## Usage

After building, the executable will be found at `build/src/mountainpath`.

### Syntax:

```bash
./mountainpath -f <input_file> [ROWS] [COLUMNS] [OPTIONS]
```

- `-f <input_file>`: Path to binary terrain data file.  
  *(Default: `../../data/testMountains.dat`)*
- `ROWS`: number of rows in the matrix. 
- `COLUMNS`: number of columns in the matrix.
- `OPTIONS`: Additional options:
  - `-b`: Only draw the best GREEDY path.
  - `-i`: Enter the interactive mode.
  - `-c`: Draw a colored map.
  - `-d`: Use the Dynamic Programming algorithm.
  - `-z`: Use the Dijkstra algorithm.
  - `-o`: Dont use the Greedy alogrithm (only Interactive mode).
  - `-h`: Show help message.

## Examples

```bash
./mountainpath -f ../../data/Colorado_844x480.dat 480 844
```

This will visualize the terrain and compute the least-effort path across it.

```bash
./mountainpath -f ../../data/Colorado_844x480.dat 480 844 -d -z -i -o -c
```

The terrain will be visualized in color, and the least-effort path will be computed using both the dynamic programming and Dijkstra algorithms, starting from the point(s) the user selects on the map.


> **Important:** The two sample/test files in the Project have the names in the format `Colorado_{COLUMNS}x{ROWS}.dat`. The program however expects the first argument to be `ROWS`. So if the program doesn't work as expected, ensure you are passing the correct number of rows and columns as arguments.



## Project Structure

```
.
├── src/               # All source code
├── data/              # Terrain matrix files
├── build.sh           # Script to build in out-of-source folder
├── configure.ac       # Autotools metadata
├── Makefile.am        # Build system instructions
```



## License

This project is educational and licensed for academic use.



##  Credits

Developed as part of the *Programmier Projekt* course at OTH Regensburg.