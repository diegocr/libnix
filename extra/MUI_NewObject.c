
#include <proto/muimaster.h>

extern struct Library *MUIMasterBase;

Object *MUI_NewObject(char *classname, Tag tag1, ...){
 return(MUI_NewObjectA(classname, (struct TagItem *) &tag1));
}
