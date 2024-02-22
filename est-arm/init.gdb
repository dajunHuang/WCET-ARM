# gdb script 

#b cfg.c : dump_cfg
#b cfg.c : build_cfg
b cfg.c : 522
commands
set print pretty on
p proc
end
