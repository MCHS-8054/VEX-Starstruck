#pragma config(Sensor, I2C_1,  ,               sensorQuadEncoderOnI2CPort,    , AutoAssign)
#pragma config(Sensor, I2C_2,  ,               sensorQuadEncoderOnI2CPort,    , AutoAssign)
#pragma config(Sensor, I2C_3,  ,               sensorQuadEncoderOnI2CPort,    , AutoAssign)
#pragma config(Sensor, I2C_4,  ,               sensorQuadEncoderOnI2CPort,    , AutoAssign)
#pragma config(Motor,  port2,           fL,            tmotorVex393HighSpeed_MC29, openLoop, encoderPort, I2C_1)
#pragma config(Motor,  port3,           bL,            tmotorVex393HighSpeed_MC29, openLoop, encoderPort, I2C_2)
#pragma config(Motor,  port4,           fR,            tmotorVex393HighSpeed_MC29, openLoop, encoderPort, I2C_4)
#pragma config(Motor,  port5,           bR,            tmotorVex393HighSpeed_MC29, openLoop, encoderPort, I2C_3)



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


int mPort[4];
int speed[4];
int multiplier[4];
int adjVal = 32;
bool withinMarg = false;
float margOfError = 100;
int slowIndex;
int margin;


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////





int getSlowIndex(int test_mSecs) {
	int slowIndex = 0;

	for(int i = 0; i<4; i++){
		nMotorEncoder[mPort[i]]= 0;
		motor[mPort[i]]=speed[i];
	}

	wait1Msec(test_mSecs);
	if(nMotorEncoder[mPort[0]]<nMotorEncoder[mPort[1]]&&nMotorEncoder[mPort[0]]<nMotorEncoder[mPort[2]]&&nMotorEncoder[mPort[0]]<nMotorEncoder[mPort[3]]){
		slowIndex=mPort[0];
	}
	else if(nMotorEncoder[mPort[1]]<nMotorEncoder[mPort[0]]&&nMotorEncoder[mPort[1]]<nMotorEncoder[mPort[2]]&&nMotorEncoder[mPort[1]]<nMotorEncoder[mPort[3]]){
		slowIndex=mPort[1];
	}
	else if(nMotorEncoder[mPort[2]]<nMotorEncoder[mPort[0]]&&nMotorEncoder[mPort[2]]<nMotorEncoder[mPort[1]]&&nMotorEncoder[mPort[2]]<nMotorEncoder[mPort[3]]){
		slowIndex=mPort[2];
	}
	else if(nMotorEncoder[mPort[3]]<nMotorEncoder[mPort[0]]&&nMotorEncoder[mPort[3]]<nMotorEncoder[mPort[1]]&&nMotorEncoder[mPort[3]]<nMotorEncoder[mPort[2]]){
		slowIndex=mPort[3];
	}


	return slowIndex;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



int getMargin(){

	for(int i = 0;i<4;i++){
		margin+=nMotorEncoder[mPort[i]]-nMotorEncoder[slowIndex];
	}

	return margin;
}




//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



void calcMultipliers() {
	getSlowIndex(1000);

	for(int i = 0; i<4; i++){
		motor[mPort[i]]=speed[i];
	}
	while(withinMarg==false){

		for(int i = 0; i<4; i++){
			if(i!=slowIndex){
				nMotorEncoder[mPort[i]]= 0;
					nMotorEncoder[slowIndex] = 0;
				motor[mPort[i]]=speed[i];
				int newAdjVal=adjVal;
				for(int j = 0; j<7;j++){
					nMotorEncoder[mPort[i]]= 0;
					nMotorEncoder[slowIndex] = 0;
					sleep(500);
					if(nMotorEncoder[mPort[i]]<nMotorEncoder[slowIndex]){
						speed[i]+=newAdjVal;
						motor[mPort[i]]=speed[i];
					}
					else if(nMotorEncoder[mPort[i]]>nMotorEncoder[slowIndex]){
						speed[i]-=newAdjVal;
						motor[mPort[i]]=speed[i];
					}else{
				break;
				}
					newAdjVal=newAdjVal/2;
				}


			}//end j loop

		}//end i loop
		if(getMargin()<=margOfError){
			withinMarg=true;
		}

	}// end while loop
}









////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


task main(){
	mPort[0]= fL; mPort[1]= bL; mPort[2]= bR; mPort[3]= fR;
	speed[0]=100; speed[1]=100; speed[2]=100; speed[3]=100;
	multiplier[0]=1;multiplier[1]=1;multiplier[2]=1;multiplier[3]=1;
	calcMultipliers();

}
