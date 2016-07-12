#include <opari2/pomp2_lib.h>

#define POMP2_DLIST_00001 shared(pomp2_region_1)
static POMP2_Region_handle pomp2_region_1 = NULL;
static const char pomp2_ctc_1[] = "239*regionType=parallelfor*sscl=/home/jawa/Documents/studies/cad/CAD-COC472/2/2/bin/../wave/old/main_best.cc:203:203*escl=/home/jawa/Documents/studies/cad/CAD-COC472/2/2/bin/../wave/old/main_best.cc:223:223*scheduleType=dynamic*hasCollapse=1**";
extern "C" 
{
void POMP2_Init_reg_19008547_1()
{
    POMP2_Assign_handle( &pomp2_region_1, pomp2_ctc_1 );
}
}
