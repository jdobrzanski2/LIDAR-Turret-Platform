#
_XDCBUILDCOUNT = 
ifneq (,$(findstring path,$(_USEXDCENV_)))
override XDCPATH = C:/ti_v8/bios_6_76_03_01/packages;C:/Users/Joseph/Desktop/Romalo_project/screen_w_BIOS/.config
override XDCROOT = C:/ti_v8/xdctools_3_51_01_18_core
override XDCBUILDCFG = ./config.bld
endif
ifneq (,$(findstring args,$(_USEXDCENV_)))
override XDCARGS = 
override XDCTARGETS = 
endif
#
ifeq (0,1)
PKGPATH = C:/ti_v8/bios_6_76_03_01/packages;C:/Users/Joseph/Desktop/Romalo_project/screen_w_BIOS/.config;C:/ti_v8/xdctools_3_51_01_18_core/packages;..
HOSTOS = Windows
endif
