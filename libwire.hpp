
/*  NOTE(s):
 *  - Frequency sweep analysis Freq(max) -> Lamda(min)
 *    - conservative max segment length = Lambda(min) / 20 -- (use 21 segments)
 *    - maximum segment length          = Lambda(min) / 9  -- (use 9  segments)
 *  - easiest way to set source or load is set it in center of wire
 *    - MMANA-GAL: example "w1c" -- wire 1 center (other w1s, w1e)
 *    - 4NEC2:     example  tag=1 segment=2 (assume wire 1 have 3 segments)
 */

#define FILETYPE_MAA  1
#define FILETYPE_NEC  2

#define AUTOSEG_CONSERVATIVE  0
#define AUTOSEG_MAXLENGTH     1
#define AUTOSEG_MINCOUNT      AUTOSEG_MAXLENGTH


typedef struct
{
    double x, y, z;
} endpoint_t;


typedef struct
{
    double  x1,y1,z1,  x2,y2,z2;   // [m]
    double  R;                     // [mm]
    int     seg;                   // segmentation: mmana set -1 for auto segmentation
} wire_t;

/*
static void export_wire_maa( FILE *outfile, wire_t *wire );
static int  export_wire_nec( FILE *outfile, wire_t *wire );

static void export_MAA_head( FILE *outfile, double freq_MHz );
static void export_MAA_tail( FILE *outfile );
static void export_NEC_head( FILE *outfile );
static void export_NEC_tail( FILE *outfile );
*/

namespace libwire {

void export_filetype( int type, double maxMHz, int autoseg );
void export_file(  FILE *outfile, wire_t *wire, int wirecount );
void export_wires( FILE *outfile, wire_t *wire, int wirecount );

int  autosegment( wire_t *wire, double MHz_max, int maxlen );
void setWire( wire_t *wire, double x1, double y1, double z1, double x2, double y2, double z2, double R, int segments );
void addHeight( wire_t *wire, double dz, int wirecount );
void mirrorY( wire_t *wire, int wirecount );

} //  namespace libwire
