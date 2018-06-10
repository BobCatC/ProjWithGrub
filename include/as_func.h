#ifndef _AS_FUNC_H
#define _AS_FUNC_H

extern "C"{
	void _Sleep(u32);
	void _set_video_mode();
	void _set_real_mode();

	u32 _get_cr0();
	u32 _get_ds();
	u32 _get_cs();
	u32 _get_ss();

	void _start_up();
	void _add_new_address(u32);
	void _flush_cache();
	struct TIME_T* _get_time_access(u32);
	struct TIME_T* _get_time_access_novar();
	void _end_of_work();

	u8 _in_b_port_60();
	u8 _in_b_port_64();
	void _out_b(u8 iPort, u8 Data);

	u8 _check_mem();
}

#endif
