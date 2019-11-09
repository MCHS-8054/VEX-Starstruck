#pragma config(Sensor, I2C_1,  ,               sensorQuadEncoderOnI2CPort,    , AutoAssign)
#pragma config(Sensor, I2C_2,  ,               sensorQuadEncoderOnI2CPort,    , AutoAssign)
#pragma config(Sensor, I2C_3,  ,               sensorQuadEncoderOnI2CPort,    , AutoAssign)
#pragma config(Sensor, I2C_4,  ,               sensorQuadEncoderOnI2CPort,    , AutoAssign)
#pragma config(Motor,  port2,           fL,            tmotorVex393HighSpeed_MC29, openLoop, encoderPort, I2C_1)
#pragma config(Motor,  port3,           bL,            tmotorVex393HighSpeed_MC29, openLoop, encoderPort, I2C_2)
#pragma config(Motor,  port4,           fR,            tmotorVex393HighSpeed_MC29, openLoop, encoderPort, I2C_4)
#pragma config(Motor,  port5,           bR,            tmotorVex393HighSpeed_MC29, openLoop, encoderPort, I2C_3)

int mPort[4];
int speed[4];
byte counts[4];
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
byte MIN(byte a, byte b){
return a<b?a:b;
}
byte slowest(byte m1,byte m2,byte m3,byte m4){
	byte t = MIN(MIN(m3,m4),MIN(m1,m2));
	if(m1==t)return 0;
	if(m2==t)return 1;
	if(m3==t)return 2;
	if(m4==t)return 3;
	return 4;
}
void calibrate(){
	speed[0]=100; speed[1]=100; speed[2]=100; speed[3]=100;
	byte m1,m2,m3,m4;
	for(int i = 0; i<5;i++){
		switch(getSlowIndex(1000)){
		case 0:
			m1++;return;
		case 1:
			m2++;return;
		case 2:
			m3++;return;
		case 3:
			m4++;return;
		}
	}
	byte slowestM = slowest(m1,m2,m3,m4);
	for(;;){
		wait1Msec(1000);
		for(int i = 0; i<4; i++){
			nMotorEncoder[mPort[i]]= 0;
			motor[mPort[i]]=speed[i];
		}
		for(int i = 0; i<4; i++){
			if(i==slowestM)continue;
			if(nMotorEncoder[mPort[i]]>nMotorEncoder[mPort[slowestM]])counts[i]++;return;
		}
		for(int i = 0; i<4; i++){
			if(i==slowestM)continue;
			if(counts[i]>3)counts[i]=0;speed[i]-=1;
		}
	}
}
task main()
{
	mPort[0]= fL; mPort[1]= bL; mPort[2]= bR; mPort[3]= fR;
	calibrate();


}
