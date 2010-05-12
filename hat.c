#include "globaldefs.h"
#include "datatypes.h"
#include "hat.h"



int HAT_Hash(PID proccessID,int pageNumber)
{
    return (proccessID*pageNumber)%NumOfPagesInMM;
}

bool HAT_Init()
{
    HATArray = calloc(NumOfPagesInMM,sizeof(IPT_t));
    if(!HATArray)
        return FALSE;
    else
        return TRUE;
}
