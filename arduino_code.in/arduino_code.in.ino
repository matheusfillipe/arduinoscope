
const int analogPin = A0;//pin to use
const int loopTimes = 9600/10; //Number of loops until send the average calculated value
//Increse this value if you are not getting 0 with nothing connected
const int calibrationLoopCount = 15; //Auto calibration number of reads (the values read by analog read may vary depending on the power supply)


float baseValue=0; 
int baseCount;
float avRead=0; //average voltage value

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(analogPin, INPUT_PULLUP); 
  baseCount=calibrationLoopCount;
}


void loop() {
  
  for(int i=0;i<loopTimes; i++){
    
    // read the input on analog pin 0:
    int sensorValue = analogRead(analogPin);  
    
    // Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 5V):
    float voltage = sensorValue * (5.0 / 1023.0); 

    avRead+=voltage;      
  }  

    float voltage=avRead/loopTimes;
    
    if(baseCount < 0){
    // print out the  average value you read:  
    Serial.println(baseValue-voltage); 
   
  
    }else if(baseCount == 0){
       
      baseValue=baseValue/calibrationLoopCount ;     
      //Serial.print("Base:");   
      //Serial.println(baseValue);
    }
          
    else{      
      baseValue+=voltage;  
      //Serial.print("DEBUG:");
      //Serial.println(baseValue);    
  }

   avRead=0;   
   
  if(baseCount!=-1){
    baseCount--;
  }
  
}
   

