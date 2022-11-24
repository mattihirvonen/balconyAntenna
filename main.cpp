
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "libwire.hpp"

#define  GROUND_LADDER   0
#define  GROUND_GRIDNET  1
#define  GROUND_FOLDED   2

#define  PI  3.14159

using namespace  std;
using namespace  libwire;

//--------------------------------------------------------------------------------------------------------------
// Antenna default data:

#define GROUND_TYPE  GROUND_FOLDED

#if GROUND_TYPE == GROUND_FOLDED
#define RADIATOR20m  5.8          // Radiator default length
#else
#define RADIATOR20m  5.5          // Radiator default length, default GROUND_LADDER
#endif

double  gnd20m = 4.3;    // Folded antenna:
double  gnd15m = 3.1;    // - simulated as off center feed dipole
double  gnd10m = 2.2;    // - counterpoise (=gnd) lengths

double  dx_ladder[] = { 0.00, 1.21, 2.43, 3.48 };
double  dy_ladder[] = { 0.00, 1.27  };

double  dx_grid[]   = { 0.00, 0.25, 0.50, 0.75, 1.00, 1.25, 1.50, 1.75, 2.00, 2.25, 2.50, 2.75, 3.00, 3.25, 3.48 };
double  dy_grid[]   = { 0.00, 0.25, 0.50, 0.75, 1.00, 1.27  };

double  dz     = -0.96;  // Balcony height [m]
double  height = 15.0;   // Feed point height [m]

//--------------------------------------------------------------------------------------------------------------

wire_t  wires[1000];

typedef struct
{
    int     debug;
    int     filetype;      // "-M": MAA default is NEC
    int     autoseg;       // "-m": min count (==max length), default is conservative
    double  maxFreq;       // "-f": frequency sweep max [MHz]
    double  radiator;      // "-r": Radiator length [m];
    int     ground;        // "-G": grond is grid screen
    double  rotDegr;       // "-R": rotate antenna [deg]
    double  takeoffAngle;  // "-a": 0=horizontal, 90=vertical
    int     mirrorY;       // "-y": mirror y axis coordinates
} antenna_t;


antenna_t antenna =
{
    0, FILETYPE_NEC, AUTOSEG_CONSERVATIVE, 40.0, RADIATOR20m, GROUND_TYPE
};

//--------------------------------------------------------------------------------------------------------------

int ladderX( wire_t *wire, double x, double y, double z, double dz, double *dx, int nx )
{
    int i, elememtCount = nx; // sizeof(dx)/sizeof(double) - 1;

    double R=0.8, seg=-1;

    for ( i = 0; i < elememtCount; i++ )
    {
        setWire( wire, x+dx[i],   y, z,    x+dx[i+1], y, z,    R, seg );   wire++;
        setWire( wire, x+dx[i+1], y, z,    x+dx[i+1], y, z+dz, R, seg );   wire++;
        setWire( wire, x+dx[i+1], y, z+dz, x+dx[i],   y, z+dz, R, seg );   wire++;
    }
    return (3 * elememtCount);   // wire count
}


int ladderY( wire_t *wire, double x, double y, double z, double dz, double *dy, int ny )
{
    int i, elememtCount = ny ; // sizeof(dy)/sizeof(double) - 1;

    double R=0.8, seg=-1;

    for ( i = 0; i < elememtCount; i++ )
    {
        setWire( wire, x, y+dy[i],   z,    x, y+dy[i+1], z,    R, seg );   wire++;
        setWire( wire, x, y+dy[i+1], z,    x, y+dy[i+1], z+dz, R, seg );   wire++;
        setWire( wire, x, y+dy[i+1], z+dz, x, y+dy[i],   z+dz, R, seg );   wire++;
    }
    return (3 * elememtCount);   // wire count
}


int ladderXYZ( wire_t *wire, double x, double y, double z, double dz )
{
    int Nx, Ny;

    double R = 0.8, seg = -1;  // MMANA-GAL: -1 == autosegment

    setWire( wire,   x, y, z,   x, y, z+dz,   R, seg );
    wire += 1;

    if ( antenna.ground == GROUND_GRIDNET )
    {
        Nx    = ladderX( wire, x, y, z, dz, dx_grid, 14 );
        wire += Nx;
        Ny    = ladderY( wire, x, y, z, dz, dy_grid, 5 );
    }
    else
    {
        Nx    = ladderX( wire, x, y, z, dz, dx_ladder, 3 );
        wire += Nx;
        Ny    = ladderY( wire, x, y, z, dz, dy_ladder, 1 );
    }
    return 1 + Nx + Ny;  // Count of wires
}




int foldedXYZ( wire_t *wire, double z, double dz )
{
    int     Nwires = 0;
    double  R      = 0.8;

    setWire( wire, dx_ladder[0], 0,  z,    dx_ladder[1], 0, z,        R, -1 );    wire++;   Nwires++;
    setWire( wire, dx_ladder[1], 0,  z,    dx_ladder[2], 0, z,        R, -1 );    wire++;   Nwires++;
    setWire( wire, dx_ladder[2], 0,  z,    dx_ladder[3], 0, z,        R, -1 );    wire++;   Nwires++;

    if ( gnd20m > (dx_ladder[3] + fabs(dz)) )
    {
        double dx20m = gnd20m - (dx_ladder[3] + fabs(dz));
        double x20m  = dx_ladder[3] - dx20m;

        setWire( wire, dx_ladder[3], 0,  z,    dx_ladder[3],  0, z+dz,     R, -1 );    wire++;   Nwires++;
        setWire( wire, dx_ladder[3], 0,  z+dz, x20m,          0, z+dz,     R, -1 );    wire++;   Nwires++;
    }
    else
    {
        double dz20m = gnd20m - dx_ladder[3];

        setWire( wire, dx_ladder[3], 0,  z,    dx_ladder[3], 0, z-dz20m,   R, -1 );    wire++;   Nwires++;
    }

    if ( gnd15m > (dx_ladder[2] + fabs(dz)) )
    {
        double dx15m = gnd20m - (dx_ladder[2] + fabs(dz));
        double x15m  = dx_ladder[2] + dx15m;

        setWire( wire, dx_ladder[2], 0,  z,    dx_ladder[2],  0, z+dz,     R, -1 );    wire++;   Nwires++;
        setWire( wire, dx_ladder[2], 0,  z+dz, x15m,          0, z+dz,     R, -1 );    wire++;   Nwires++;
    }
    else
    {
        double dz15m = gnd15m - dx_ladder[2];

        setWire( wire, dx_ladder[2], 0,  z,    dx_ladder[2], 0, z-dz15m,   R, -1 );    wire++;   Nwires++;
    }

    double dz10m = gnd10m - dx_ladder[1];
//  setWire( wire, dx_ladder[1], 0,  z,    dx_ladder[1], 0, z-dz10m,   R, -1 );    wire++;   Nwires++;

    return Nwires;
}


void setRadiator( wire_t *wire, double radiatorLen, double takeoffAngle, double deltaZ )
{
    double  R   = 0.8;
    int     seg = -1;

    wire->x1 = 0; wire->y1 = 0; wire->z1 = deltaZ;

    wire->x2 = 0;
    wire->y2 = -radiatorLen * cos( takeoffAngle * PI / 180.0 );
    wire->z2 =  radiatorLen * sin( takeoffAngle * PI / 180.0 ) + deltaZ;
    wire->R  =  R;
    wire->seg=  seg;
}

//------------------------------------------------------------------------------------------------------

void help( const char *command )
{
    printf( "\n" );
    printf( "Usage:  %s [-d] [-M] [-m] [-f MHz] [-r lenght] [-G | -F] [-a angle] [-y]\n" );
    printf( "\n" );
    printf( "Where\n" );
    printf( "\n" );
    printf( "-?          this help\n" );
    printf( "-M          output format MAA (default NEC)\n" );
    printf( "-m          output segmentation max len (default conservative)\n" );
    printf( "-f MHz      frequency sweep max value [MHz] (default %f)\n",antenna.maxFreq );
    printf( "-r length   radiator length [m] (default %f)\n", antenna.radiator );
    printf( "-G          ground type grid net (default ladder)\n" );
    printf( "-F          not implemented)\n" );
    printf( "-a angle    radiator angle [deg] (90=vertical, default 0=horizontal\n" );
    printf( "-y          mirror y axis cordinate values (y -> -y)\n" );
    printf( "\n" );
}


int parse_args( int argc, const char *argv[] )
{
//  printf("argc=%d\n",argc);

    for ( int i = 1; i < argc; i++ )
    {
    //  printf("test argv[%d]: %s\n",i,argv[i]);

             if ( !strcmp(argv[i],"-?" ) )  { return -1;                                              }
        else if ( !strcmp(argv[i],"-d" ) )  { antenna.debug    = 1;                                   }
        else if ( !strcmp(argv[i],"-M" ) )  { antenna.filetype = FILETYPE_MAA;                        }
        else if ( !strcmp(argv[i],"-m" ) )  { antenna.autoseg  = AUTOSEG_MAXLENGTH;                   }
        else if ( !strcmp(argv[i],"-f" ) )  { sscanf(argv[i+1], "%lf", &antenna.maxFreq);       i++;  }
        else if ( !strcmp(argv[i],"-r" ) )  { sscanf(argv[i+1], "%lf", &antenna.radiator);      i++;  }
        else if ( !strcmp(argv[i],"-G" ) )  { antenna.ground   = GROUND_GRIDNET;                      }
        else if ( !strcmp(argv[i],"-F" ) )  { antenna.ground   = GROUND_FOLDED;                       }
        else if ( !strcmp(argv[i],"-a" ) )  { sscanf(argv[i+1], "%lf", &antenna.takeoffAngle);  i++;  }
        else if ( !strcmp(argv[i],"-y" ) )  { antenna.mirrorY  = 1;                                   }
        else {
            printf( "Unsupported command line argument: %s\n", argv[i] );
            exit ( 1 );
        }
    }
    return 0;
}


void print_args( void )
{
    printf("filetype  %d\n",antenna.filetype);
    printf("autoseg   %d\n",antenna.autoseg);
    printf("maxFreq   %f\n",antenna.maxFreq);
    printf("radiator  %f\n",antenna.radiator);
}


int main( int argc, const char *argv[] )
{
    FILE    *outfile = stdout;
    wire_t  *wire    = wires;
    int      Nwires;

//  cout << "Hello world!" << endl;

    if ( parse_args(argc, argv)  < 0 ) {
        help( argv[0] );
        exit( 1 );
    }
    if ( antenna.debug ) {
        print_args();
    }
    setWire( wire, 0,0,0,   0,0,0.25,   0.8,  3 );                       wire++;  // Feed point element
    setRadiator( wire, antenna.radiator, antenna.takeoffAngle, 0.25 );   wire++;  // Radiator   element

    if ( antenna.ground == GROUND_FOLDED ) {  Nwires = 2 + foldedXYZ( &wires[2], 0, dz );         }
    else                                   {  Nwires = 2 + ladderXYZ( &wires[2], 0, 0, 0, dz );   }

    addHeight( wires, height, Nwires );
    /*
    if ( antenna.mirrorY ) {
        mirrorY( wires, Nwires );
    }
    */
    export_filetype( antenna.filetype, antenna.maxFreq, antenna.autoseg );
    export_file( outfile, wires, Nwires );

    return 0;
}
