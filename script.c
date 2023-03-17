#include <Servo.h>
Servo lock;
const int inputPin = A0, tolerance = 10, lockClose = 120, lockOpen = 85;

int inputList[2], ini=0, inL=2, inputDiff, inputNew, inputOld;
int Time = 0, i, csi, tsi, trigger, tsp = 0;
int Cs[15]={40,20,20,40,40,80,40,20,20,40,40,40,40,40,40}, Ts[15], Len = 15;

void setup() {
  lock.attach(9);
  lock.write(lockClose); //一開始先關起來
}

void loop() {
  for(i=0; i<50; i++){
    inputOld = inputNew;
    inputNew = analogRead(inputPin);
    inputDiff = inputNew - inputOld; //計算增加了多少分貝
    
    if(inputDiff > 20 && Time > 5){ //增加超過20分貝就視為一次敲擊
      
      if(tsp==Len) tsp=0;
      Ts[tsp++] = Time; //紀錄敲擊時間
      trigger = 1;
    
      tsi = tsp; //計算是否符合正確節奏
      for(csi=0; csi<Len; csi++){
        if(tsi+csi==Len) tsi=-csi;
        if(abs(Cs[csi]-Ts[tsi+csi])>tolerance){ //超出容許誤差範圍
          trigger = 0;
          break;
        }
      }
      
      if(trigger==1){ //所有敲擊時間間隔都在誤差範圍內
        for(csi=0; csi<Len; csi++) Ts[csi]=0; //把剛剛敲過的紀錄清除
        for(int i=lockClose; i>lockOpen; i--){ //開鎖
          lock.write(i);
          delay(3);
        }
        delay(3000);
        for(int i=lockOpen; i<lockClose; i+=0{ //關鎖
          lock.write(i);
          delay(3);
        }
      }
      Time = 0; //計時器歸零
    }
  }
  Time++; //一秒約178圈
}
