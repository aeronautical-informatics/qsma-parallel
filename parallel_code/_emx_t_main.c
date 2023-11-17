#include "_emx_main.h"

void run_main(int core) {
	switch(core) {
		case 0: {
			fltastep_p0();
			break;
		}
		case 1: {
			fltastep_p1();
			break;
		}
		case 2: {
			fltastep_p2();
			break;
		}
		case 3: {
			fltastep_p3();
			break;
		}
		default:{
			break;
		}
	}
	
	EMX_Join(core);
}

int main(int argc, char *argv[]) {
	int core = EMX_Init(argc, argv);
	
	EMX_Run(core);

	EMX_Finalize();
	
	return 0;
}
