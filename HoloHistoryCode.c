#pragma config(Sensor, I2C_1,  ,               sensorQuadEncoderOnI2CPort,    , AutoAssign)
#pragma config(Sensor, I2C_2,  ,               sensorQuadEncoderOnI2CPort,    , AutoAssign)
#pragma config(Sensor, I2C_3,  ,               sensorQuadEncoderOnI2CPort,    , AutoAssign)
#pragma config(Sensor, I2C_4,  ,               sensorQuadEncoderOnI2CPort,    , AutoAssign)
#pragma config(Motor,  port2,           fL,            tmotorVex393HighSpeed_MC29, openLoop, encoderPort, I2C_1)
#pragma config(Motor,  port3,           bL,            tmotorVex393HighSpeed_MC29, openLoop, encoderPort, I2C_2)
#pragma config(Motor,  port8,           fR,            tmotorVex393HighSpeed_MC29, openLoop, encoderPort, I2C_4)
#pragma config(Motor,  port9,           bR,            tmotorVex393HighSpeed_MC29, openLoop, encoderPort, I2C_3)
const unsigned byte HistLen = 100;
unsigned int Hist[4][HistLen];
unsigned long lastHist[4] = {0,0,0,0};
unsigned byte histPl = 0;
int mPort[4];
int speed[4];
task hister(){
	for(;;){
		for(int i = 0; i < 4;i++){
			unsigned long tmpEn = nMotorEncoder[mPort[i]];
			Hist[i][histPl]=tmpEn-lastHist[i];
			lastHist[i]=tmpEn;
		}
		histPl++;
		histPl=histPl%HistLen;
		wait1Msec(100);
	}
}
task main()
{

	mPort[0]= fL; mPort[1]= bL; mPort[2]= bR; mPort[3]= fR;

	speed[0]=100; speed[1]=100; speed[2]=100; speed[3]=100;

	for(int i = 0; i<4; i++){
		nMotorEncoder[mPort[i]]= 0;
		motor[mPort[i]]=speed[i];
	}
	startTask(hister);
	while(Hist[0][99]==0){}
	for(;;){
		wait1Msec(1000);
		float tmpadd[4];
		for(int i = 0; i < 100;i++){
			tmpadd[0]+=Hist[0][i];
			tmpadd[1]+=Hist[1][i];
			tmpadd[2]+=Hist[2][i];
			tmpadd[3]+=Hist[3][i];
		}
		tmpadd[0]/=100;
		tmpadd[1]/=100;
		tmpadd[2]/=100;
		tmpadd[3]/=100;
		clearDebugStream();
		writeDebugStreamLine("%0.3f/%0.3f/%0.3f/%0.3f",tmpadd[0],tmpadd[1],tmpadd[2],tmpadd[3]);
	}
}
