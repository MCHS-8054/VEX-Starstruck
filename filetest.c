#define FFDEBUG
#include "inc/FlashLib.h"
unsigned char* constructName(char r, char *val){
	unsigned char tmp[16];
	tmp[0]=r;
	for(int j = 0; j < 15; j++){
		tmp[j+1] = val[j];
	}
	return tmp;
}
void setRegister(char reg, char *val){

	flash_file t;
	int i = RCFS_FindFirstFile(t);
	while(i!=RCFS_ERROR){
		if(t.name[0]==reg){
			t.name=constructName(reg,val);
			RCFS_Write(t);
			return;
		}
		i = RCFS_FindNextFile(t);
	}
	char *l = constructName(reg,val);
	RCFS_AddFile("Y",1,l);
	return;
}
task main()
{
	setRegister('C',"HELLOWORLDHELLO");
	wait1Msec(3000);
}
