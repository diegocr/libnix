
#include <proto/muimaster.h>

extern struct Library *MUIMasterBase;

APTR MUI_AllocAslRequestTags(ULONG type, Tag tag1, ...){
 return(MUI_AllocAslRequest(type, (struct TagItem *) &tag1));
}
