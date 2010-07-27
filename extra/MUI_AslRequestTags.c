
#include <proto/muimaster.h>

extern struct Library *MUIMasterBase;

BOOL MUI_AslRequestTags(APTR req, Tag tag1, ...){
 return(MUI_AslRequest(req, (struct TagItem *) &tag1));
}
