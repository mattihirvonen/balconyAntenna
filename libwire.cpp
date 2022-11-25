
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "libwire.hpp"

using namespace  std;

static int       filetype;       // NEC or MAA
static int       segmentation;   // Min count == Max length != 0, or conservative == 0
static double    freqMax;        // [MHz]

//----------------------------------------------------------------------------------

int libwire::autosegment( wire_t *wire, double MHz_max, int maxlen )
{
    // Strategy for Nseg: max segment length = 9, conservative = 21
    double Nseg = maxlen ? 9.0 : 21.0;

    double Lambda_min = 300.0 / MHz_max;         // [m]
    double segLenMax  = Lambda_min / Nseg;

    double dx = wire->x1 - wire->x2;
    double dy = wire->y1 - wire->y2;
    double dz = wire->z1 - wire->z2;

    double wirelen  = sqrt( dx*dx + dy*dy + dz*dz );
    double segments = wirelen / segLenMax;

    return int( segments ) + 1;   // Round up
}


void libwire::setWire( wire_t *wire, double x1, double y1, double z1, double x2, double y2, double z2, double R, int segments )
{
    wire->x1 = x1;   wire-> y1 = y1;   wire->z1 = z1;
    wire->x2 = x2;   wire-> y2 = y2;   wire->z2 = z2;

    wire->R  = R;
    wire->seg= segments;
}


void libwire::addHeight( wire_t *wire, double height, int wirecount )
{
    while( wirecount-- > 0 ) {
        wire->z1 += height;
        wire->z2 += height;
        wire++;
    }
}


void libwire::mirrorY( wire_t *wire, int wirecount )
{
    while( wirecount-- > 0 ) {
        if ( wire->y1 ) {
             wire->y1 = -wire->y1;
        }
        if ( wire->y2 ) {
             wire->y2 = -wire->y2;
        }
    }
}

//----------------------------------------------------------------------------------

static void export_wire_maa( FILE *outfile, wire_t *wire )
{
    fprintf( outfile, "%8.3f,%8.3f,%8.3f,%8.3f,%8.3f,%8.3f,\t%e,\t%d\n",
             wire->x1, wire->y1, wire->z1,
             wire->x2, wire->y2, wire->z2,
             wire->R / 1000.0,
            (wire->seg > 0) ? wire->seg : -1 );
}


static int export_wire_nec( FILE *outfile, wire_t *wire )
{
    static int ITG = 1;   // Tag number assigned to all segments of the wire.
           int NS  = 9;   // Number of segments into which the wire will be divided.

    if ( wire->seg > 0 ) {
        NS = wire->seg;
    }
    else {
        NS = libwire::autosegment( wire, freqMax, segmentation );
    }
    fprintf( outfile, "GW %4d %4d", ITG, NS );
    fprintf( outfile, " %8.3f %8.3f %8.3f", wire->x1, wire->y1, wire->z1 );
    fprintf( outfile, " %8.3f %8.3f %8.3f", wire->x2, wire->y2, wire->z2 );
    fprintf( outfile, " %8.4f\n", wire->R / 1000.0 );

    ITG += 1;

    return ITG;
}

//----------------------------------------------------------------------------------

static void export_MAA_head( FILE *outfile, double freq_MHz )
{
    fprintf( outfile, "%s\n", "Balcony whip antenna" );
    fprintf( outfile, "%s\n", "*" );
    fprintf( outfile, "%f\n", freq_MHz );
    fprintf( outfile, "%s\n", "***Wires***" );
}


static void export_MAA_tail( FILE *outfile )
{
    fprintf( outfile, "%s\n", "***Source***" );
    fprintf( outfile, "%s\n", "1,	0" );
    fprintf( outfile, "%s\n", "w1c,	0.0,	1.0" );
    fprintf( outfile, "%s\n", "***Load***" );
    fprintf( outfile, "%s\n", "0,	1" );
    fprintf( outfile, "%s\n", "***Segmentation***" );
    fprintf( outfile, "%s\n", "800,	80,	2.0,	2" );
    fprintf( outfile, "%s\n", "***G/H/M/R/AzEl/X***" );
    fprintf( outfile, "%s\n", "2,	0.0,	0,	50.0,	120,	60,	0.0" );
    fprintf( outfile, "%s\n", "###Comment###" );
    fprintf( outfile, "%s\n", "Mod by bbbb, aaaa MM/DD/YYYY hh:mm:ss" );
}


static void export_NEC_head( FILE *outfile )
{
    fprintf( outfile, "CM\n" );
    fprintf( outfile, "CM   Balcony whip antenna\n" );
    fprintf( outfile, "CM\n" );
    fprintf( outfile, "CE\n" );
}


static void export_NEC_tail( FILE *outfile )
{
    int   tag = 1;
    int   seg = 2;

    int   fSteps = 41;
    float fStart = 13.0;
    float fDelta = 0.05;

    float dielect = 13.0;
    float conduct = 0.05;

    fprintf( outfile, "GE   0\n" );
    fprintf( outfile, "EX   0 %d %d 0 1.0 0.0\n", tag, seg );
//  fprintf( outfile, "EK\n" );
    fprintf( outfile, "GN   3 0 0 0 %f %f\n", dielect, conduct );
    fprintf( outfile, "FR   0 %d 0 0 %f %f\n", fSteps, fStart, fDelta );
    fprintf( outfile, "EN\n" );
}

//----------------------------------------------------------------------------------

void libwire::export_filetype( int type, double maxMHz, int autoseg )
{
    filetype     = type;
    freqMax      = maxMHz;
    segmentation = autoseg;
}


void libwire::export_wires( FILE *outfile, wire_t *wire, int wirecount )
{
    if ( !((filetype == FILETYPE_MAA) || (filetype == FILETYPE_NEC)) ) {
        printf( "ERROR: Export data type not defined\n" );
        return;
    }
    while ( wirecount-- > 0 )
    {
        if ( filetype == FILETYPE_MAA ) {
            export_wire_maa( outfile, wire );
        }
        else if ( filetype == FILETYPE_NEC ) {
            export_wire_nec( outfile, wire );
        }
        wire++;
    }
}


void libwire::export_file( FILE *outfile, wire_t *wire, int wirecount )
{
    if ( !((filetype == FILETYPE_MAA) || (filetype == FILETYPE_NEC)) ) {
        printf( "ERROR: Export file type not defined\n" );
        return;
    }
    if ( filetype == FILETYPE_MAA ) {
        export_MAA_head( outfile, 14.1 );
        fprintf( outfile, "%d\n", wirecount );
        export_wires( outfile, wire, wirecount );
        export_MAA_tail( outfile );
    }
    else if ( filetype == FILETYPE_NEC ) {
        export_NEC_head( outfile );
        export_wires( outfile, wire, wirecount );
        export_NEC_tail( outfile );
    }
}

//------------------------------------------------------------------

