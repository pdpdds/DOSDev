// GLOBAL DEFINITIONS ///////////////////////////////////////////////////////

#ifndef _GLOBAL_H
#define _GLOBAL_H

// TIMING ///////////////////////////////////////////////////////////////////

#define END_MORPH  25500  			// 27 	0:27
#define END_EYE    47500 	 			// 19 	0:47
#define END_FRACT 121000  			// 50.5	2:01 // 1:08:5
#define END_VOXEL 201000  			// 80 	3:26 // 2:05
#define END_IFS   246500  			// 39.5 4:08 // 3:24
#define END_DUNE  290000  			// 41 	4:53 // 4:07
#define END_SURFA 325200  			// 34 	5:25 // 4:52

// FILENAMES ////////////////////////////////////////////////////////////////

#define SCPALFILE	  "SUNKNOWN.000"
#define SCDATA			"SUNKNOWN.001"
#define SURFACEFILE "SUNKNOWN.002"
#define SURFACEPAL	"SUNKNOWN.003"

#define FRACTWIST   "SUNKNOWN.004"
#define FRACFILE5		"SUNKNOWN.005"
#define FRACFILE6		"SUNKNOWN.006"
#define FRACFILE7		"SUNKNOWN.007"

#define VOXELPAL		"SUNKNOWN.008"
#define CLOUDFILE		"SUNKNOWN.009"
#define MARBLEBMP		"SUNKNOWN.010"

#define ZOOMPAL			"SUNKNOWN.011"
#define EYEBMP			"SUNKNOWN.013"
#define EYEPAL			"SUNKNOWN.014"

#define DUNEMAP1		"SUNKNOWN.015"
#define DUNEMAP2		"SUNKNOWN.016"

#define MORPHDATA		"SUNKNOWN.017"
#define MORPHPAL		"SUNKNOWN.018"

#define DUNEPAL			"SUNKNOWN.019"

#define BRIXFILE    "SUNKNOWN.101"
#define MASKFILE    "SUNKNOWN.103"
#define FLAMEPAL    "SUNKNOWN.104"

#define TITLE       "SUNKNOWN.020"
#define VOXPRE			"SUNKNOWN.021"
#define IFSPAL			"SUNKNOWN.022"

#define MIDIFILE    "SUNKNOWN.200"

// FIXED DEFINES ////////////////////////////////////////////////////////////

#define VGAINT	  geninterrupt(0x10)
#define DOSINT    geninterrupt(0x21)
#define MOUSEINT	geninterrupt(0x33)

#define MAXKEYS		128

#define AC_INDEX        0x03c0
#define MISC_OUTPUT     0x03c2
#define SC_INDEX        0x03c4
#define GC_INDEX	      0x03ce
#define CRTC_INDEX      0x03d4
#define INPUT_STATUS_0  0x03da
#define MAP_MASK	      0x02
#define READ_MAP        0x04
#define BIT_MASK	      0x08

// TYPEDEFS /////////////////////////////////////////////////////////////////

typedef int boolean;
typedef unsigned int word;
typedef unsigned char byte;
typedef unsigned long ulong;

#endif
