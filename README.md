# balconyAntenna

 Brief
 -----
 This is small stydy to create antenna simulator "source" file generator.
 My usage is to simulate mobile whip type antenna installed on balcony rail.
 Problem is good ground (counterpoise) for whip antenna.
 This generator generates template file
 - NEC file format for EZnec and 4NEC2 applications
 - MAA file format for MMANA-GAL application
 Main idea of this generator is to generate wire net list for antenna simulation
 applications. All other simulation configuration should set in simulation application.

 Toolchain
 ---------
 Install CodeBlocks with Minimal GNU Windows (MinGW)

 Build
 -----
 - Open project file "balconyAntenna.cbp" with CodeBlock IDE
 - Select from menus: Build / Rebuild

 Required DLL files
 ------------------
 Runtime executable required DLL files can find from CodeBlock/MinGW installation tree
 - libgcc_s_seh-1.dll
 - libstdc++-6.dll
 - libwinpthread-1.dll

 Command Line Options
 --------------------

 Usage:  balconyAntenna  [-d] [-M] [-m] [-f MHz] [-r lenght] [-G | -F] [-a angle] [-y]

 Where

 - ?          this help
 - M          output format MAA (default NEC)
 - m          output segmentation max len (default conservative)
 - f MHz      frequency sweep max value [MHz] (default 40.000000)
 - r length   radiator length [m] (default 5.500000)
 - G          ground type grid net (default ladder)
 - F          not implemented)
 - a angle    radiator angle [deg] (default 0=horizontal, 90=vertical)
 - y          mirror y axis cordinate values (y -> -y)

