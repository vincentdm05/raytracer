## Welcome to my Raytracer in one week~~end~~ (or two)

#### Thanks to Peter Shirley and his book [_Ray Tracing in one weekend_](http://in1weekend.blogspot.com/2016/01/ray-tracing-in-one-weekend.html) for making this exercise so easy and enjoyable.

### Compile & run

Assuming a file called `raytrace.cpp` containing scene description similar to the sample programs, use
`g++ -Wall -std=c++11 -o raytrace raytrace.cpp`

All sample programs will take as argument the name of a file to write to, if no argument is given the program name is used.
`./raytrace output_file`

Then simply open the generated `output_file.ppm` with your image viewer of choice.


### Samples

There are examples and tests that can be compiled and ran automatically using the bash scripts found in the respective folders.
Note that on Windows you'll have to have `nomacs` installed and an alias set for the example script to be able to show the result. Otherwise you can simply open the resulting file manually.

All example programs can produce a PPM image, written to a file for which the name can be given as parameter. If no parameter is given, the name of the executable is used to generate the output file.

