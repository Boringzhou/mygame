del /s *.obj *.suo *.ncb *.idb *.aps *.res *.plg *.opt *.ncb *.tmp *.idb *.exp *.ilk  *.pdb *.tli *.tlh disasm.asm *.trg *.pch *.~* *.ddp  *.sbr *.SCC *.vssscc *.vspscc *.htm


for /d /r %%a in (Debug?,Release?,Release_Unicode?) do rmdir /s /q %%a



pause
