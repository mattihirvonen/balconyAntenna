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
 applications. All other simulation configuration parameters should set in
 antenna simulation application.

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

 "sampleExe.zip" contain ready compiled 64 bits example runtime code "balconyAntenna.exe".
 Runtime executable code might be later a bit out dated from source codes:
 - read executable file's current command command line options with command **"balcomyAntenna -?"**
 - check antenna model wiring vith antenna simulation applications "view/visualize antenna" operation

 Command Line Options
 --------------------

 Usage:  balconyAntenna  [**-d**] [**-M**] [**-m**] [**-f** ***MHz***] [**-r** ***lenght***] [**-a** ***angle***] [**-G | -F**] [**-y**]

 Where

 - **?**                 this help
 - **M**                 output format MAA (default NEC)
 - **m**                 output segmentation strategy max length (default conservative)
 - **f** ***MHz***       frequency sweep max value [MHz] (default 40.000000)
 - **r** ***length***    radiator length [m] (default 5.500000)
 - **G**                 ground type grid net (default ladder)
 - **F**                 ground type folded dipole (default ladder)
 - **T** ***angle***     radiator angle [deg] (take off default 0=horizontal, 90=vertical)
 - **R** ***angle***     radiator angle [deg] (rotation clockwise)
 - **y**                 mirror y axis cordinate values (y -> -y)

 Usage Ideas
 -----------
 Simulate helicoil type shortened or full quarter wave radial impedance matching
 with full length radial (abt 5.5m horizontal and 5.1m vertical).

 Simulate radiation pattern with shortened radial (radial length same as helicoil
 antennas mechanical length). Dont care about impedance mismatch!

 Example command to generate MAA file
 - balconyAntenna -M >antenna20m.maa

 Example command to generate NEC file
 - balconyAntenna >antenna20m.nec
