#include "globaldefs.h"
#include "datatypes.h"
#include "hat.h"



int Hash(PID proccessID,int pageNumber)
{
    return (proccessID*pageNumber)%NumOfPagesInMM;
}

bool InitHAT()
{
    HATArray = calloc(NumOfPagesInMM,sizeof(IPT_t));
    if(!HATArray)
        return FALSE;
    else
        return TRUE;
}
