# invoke SourceDir generated makefile for main_file.p28L
main_file.p28L: .libraries,main_file.p28L
.libraries,main_file.p28L: package/cfg/main_file_p28L.xdl
	$(MAKE) -f C:\Users\Joseph\Desktop\Romalo_project\screen_w_BIOS/src/makefile.libs

clean::
	$(MAKE) -f C:\Users\Joseph\Desktop\Romalo_project\screen_w_BIOS/src/makefile.libs clean

