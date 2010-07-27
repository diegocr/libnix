
#include <proto/muimaster.h>

extern struct Library *MUIMasterBase;

Object *MUI_MakeObject(LONG type, ...){
	return(MUI_MakeObjectA(type, (ULONG *)(((ULONG)&type)+4)));
}
