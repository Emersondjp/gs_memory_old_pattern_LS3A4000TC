#ifndef GS_MEMORY_TEST_H
#define GS_MEMORY_TEST_H
#include "gs_memory_class.hpp"
bool oldPatternTest_8w6r( gs_regfile_128x64_8sw6sr & rf86 );
bool oldPatternTest_4w4r( gs_regfile_128x64_4sw4sr & rf44 );
bool oldPatternTest_3w5r( gs_cp0q_ram_64x128_3sw5sr & cp35 );
bool oldPatternTest_2w5r( gs_cp0q_ram_48x64_2sw5sr & cp25 );
bool oldPatternTest_cam464v( gs_cam_464v_64x64_1wrs & cam464v );
bool oldPatternTest_cambtb( gs_cam_btb_30x96_1w1s & cambtb );
#endif//GS_MEMORY_TEST_H
