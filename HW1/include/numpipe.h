#ifndef __NUMPIPE_H
#define __NUMPIPE_H

int init_numpipe();

int insert_numpipe_info(int cycle_after);

int do_numpipe();

int close_numpipe_buffer();

int clear_numpipe(int id);

#endif // __NUMPIPE_H