
#include <proto/dos.h>
#include <devices/conunit.h>

LONG ConsoleWidth(BPTR fd)
{
	struct FileHandle *fh;
	LONG result = 77;

	if(!(fd))fd=Output();

	if((fh = BADDR(fd))->fh_Type != NULL)
	{
		char __id[sizeof(struct InfoData) + 3];
		struct InfoData *id =
			(struct InfoData *)(((long)__id + 3L) & ~3L);

		if(DoPkt(fh->fh_Type,ACTION_DISK_INFO,((ULONG)id)>>2,0,0,0,0))
		{
			struct IOStdReq *ios;

			if((ios = (struct IOStdReq *) id->id_InUse) && !((int)ios & 1))
			{
				if(ios->io_Unit && ((struct ConUnit *) ios->io_Unit)->cu_Window == ((struct Window *) id->id_VolumeNode))
				{
					result = ((struct ConUnit *) ios->io_Unit)->cu_XMax + 1;
				}
			}
		}
	}

	return result;
}
