## Welcome to my Raytracer in one week~~end~~ (or two)

#### Thanks to Peter Shirley and his book _Ray Tracing in one weekend_ for making this exercise so easy and enjoyable.

### Compile

Assuming a file called `raytrace.cpp` containing scene description exists, use
`g++ -Wall -std=c++11 -o raytrace raytrace.cpp`


### Run

with `./raytrace`

Example usage (for mac):

* If `bin/` doesn't exist yet: `mkdir bin`
* `g++ -O3 -Wall -std=c++11 -o bin/raytrace raytrace.cpp && ./bin/raytrace > img.ppm && open img.ppm`

There are also examples and tests that can be compiled and ran automatically using the bash scripts found in the respective folders.
Note that on Windows you'll have to have `nomacs` installed and an alias set to be able to show the result.

