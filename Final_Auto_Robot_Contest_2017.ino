//-Count Line///
    int count_sensor = 0;// Count Line 
    int last_state_sensor = 0; //Check State before Count Line

//Data Condition to do of Auto Robot
    int start_line = 6;
    static int get_line = -1;
    static int drop_line = -1;
    static int box_color = 0;
    static int back_location = 1;  
    int start_keep_slide = 1;
    
//Set PIN with sensor
    const int sensor_1 = 22;  
    const int sensor_2 = 23;
    const int sensor_3 = 24;
    const int sensor_4 = 25;
    const int sensor_5 = 26;
//test
//Save Data to Read from Line Sensor 
    int data_sensor_1 = 0;
    int data_sensor_2 = 0;
    int data_sensor_3 = 0;
    int data_sensor_4 = 0;
    int data_sensor_5 = 0;

    
//Set PIN to Motor && Power Txxn
    const int dir_1 = 12; //Motor Txxn Right
    const int dir_2 = 10; //Motor Txxn Left
    const int pwm_1 = 11;
    const int pwm_2 = 9;
        
//Set PIN to Motor && Power Arm
    const int dir_3 = 4; //Motor Keep
    const int dir_4 = 6; //Motor Slide
    const int pwm_3 = 5; 
    const int pwm_4 = 7; 

//Switch Top && Bottom to Stop Motor Slide 
    const int sw_top = 18;
    const int sw_bottom = 20;
    
    int data_sw_top;
    int data_sw_bottom;    
    int sw_last_state_top;
    int sw_last_state_bottom;

//Switch Neep && Vang to Stop Motor Keep
    const int sw_neep = 25;
    const int sw_vang = 23;
    
    int data_sw_neep;
    int data_sw_vang;        
    int sw_last_state_neep;
    int sw_last_state_vang;
    
//Switch Count Color Box
    const int sw_box_1 = 47;
    const int sw_box_2 = 45;
    const int sw_box_3 = 43;
    const int sw_box_4 = 41;

    int data_sw_box_1;
    int data_sw_box_2;
    int data_sw_box_3;
    int data_sw_box_4;
    
    int last_state_box_1 = 0;
    int last_state_box_2 = 0;
    int last_state_box_3 = 0;
    int last_state_box_4 = 0;

//    int box_1 = 1;
//    int box_2 = 2;
//    int box_3 = 4;
//    int box_4 = 3;

    int box_1 = 0;
    int box_2 = 0;
    int box_3 = 0;
    int box_4 = 0;
    
void setup(){
    Serial.begin(250000);
    //Motor Txxn
    pinMode(dir_1,OUTPUT);
    pinMode(dir_2,OUTPUT);
    pinMode(pwm_1,OUTPUT);
    pinMode(pwm_2,OUTPUT);
    
    //Motor arm
    pinMode(dir_3, OUTPUT);
    pinMode(dir_4, OUTPUT);
    pinMode(pwm_3, OUTPUT) ;
    pinMode(pwm_4, OUTPUT);   
        
    //Line Detection Sensor
    pinMode(sensor_1,INPUT);
    pinMode(sensor_2,INPUT);
    pinMode(sensor_3,INPUT);
    pinMode(sensor_4,INPUT);
    pinMode(sensor_5,INPUT);

    //Switch Top && Bottom to Stop Motor Slide 
    pinMode(sw_top, INPUT);
    pinMode(sw_bottom, INPUT);
    
    //Switch Neep && Vang to Stop Motor Keep
    pinMode(sw_neep, INPUT);
    pinMode(sw_vang, INPUT);
    
    //Switch Mode
    pinMode(sw_box_1, INPUT);
    pinMode(sw_box_2, INPUT);
    pinMode(sw_box_3, INPUT);
    pinMode(sw_box_4, INPUT);
    
}//End of SetUp----------------------------------------------------------------------------------------

void loop()
{
   /** 
     * function motor_start(Right Motor, Left Motor)
     * if you use function control_mRight(PWM Min, PWM Max); ==> please set Max Pwm in Right and Low Pwm Left
     * if you use function control_mLeft(PWM Max, PWM Min); == > please set Max Pwm in Left and Low Pwm Right 
   */
   control_Sensor();//Read Data From Sensor
   count_Line();// Count Line when sensor_1 ==> sensor_5 == HIGH
   control_switch();
   sw_box_state();

    static int stateZone =  0;
    static int checkZone = 0;

    switch(stateZone){
      case 0:
      checkZone = start_zone();
      if(checkZone == 1) stateZone = 1;//Zone get
      break;
      case 1:
      checkZone = get_zone();
      if(checkZone == 3) stateZone = 3;//back Zone
      break;
      case 2:
      checkZone = drop_zone();
      if(checkZone == 0) stateZone = 0;// Start Zone
      break;
      case 3:
      checkZone = back_zone();
      if(checkZone == 2) stateZone = 2;//drop Zone
      break;
    }
    
}//End of Looper Controller-----------------------------------------------------------------------------------------

  int start_zone(){    
    control_switch();//Read State From Switch    
    sw_state(); // Chect Status State of Switch
   
    
    while(start_keep_slide <= 1)//Start to Check Motor Keep && Slide
    {
        robotArm_keep_slide_setup();
        start_keep_slide++;        
    }//To do One time to Start Auto Robot
    
    static int returnData;
    
    condition_motor(140,140);
    if(count_sensor == start_line){
      count_sensor = 10;
      get_line = 0;
      get_line = count_sensor + back_location;
      returnData = 1;
    }else{
      returnData = 0;
    }  
    return returnData;
  }

  int get_zone(){    
    control_switch();//Read State From Switch    
    sw_state(); // Chect Status State of Switch
    
    static int returnData;
    
    if(count_sensor == 10){
      condition_motor(100, 100);
    }else{
      condition_motor(130,130);
    }
    
    if(count_sensor == get_line){
      robot_turn_right(2, 3);
    }

    if(count_sensor == 34){
      
      delay(100);
      motor_start(0,0);     
      delay(200);
      motor_start(70,70); 
      delay(300);    
      motor_start(0,0); 
      delay(1000); 
      
      bool exitloopNeep = false;
      bool callNeep = false;    
      bool exitloopTop = false;
      bool callTop = false;
  
      while(!exitloopNeep)
      {
         data_sw_neep = digitalRead(sw_neep);
         if(!(data_sw_neep == 0)){
          if(!callNeep){
            motor_arm_keep_start(200);
            callNeep = true;
          }
         }else{
          delay(250);
          sw_state_neep();
          delay(200);
          exitloopNeep = true;
         }
      }
      while(!exitloopTop)
      {
         data_sw_top = digitalRead(sw_top);
         if(!(data_sw_top == 1)){
          if(!callTop){
            motor_arm_slide_back(200);
            callTop = true;
          }
         }else{
          delay(100);
          sw_state_top();
           delay(200);
          exitloopTop = true;
         }
      }
      delay(500);
      if(data_sw_neep == LOW && data_sw_top == HIGH){
        box_color = box_1; 
        robot_circle_back(2,3);  
      } 
      
    }else if(count_sensor == 37){
      delay(100);
      motor_start(0,0);     
      delay(200);
      motor_start(70,70); 
      delay(300);    
      motor_start(0,0); 
      delay(1000); 
      
      bool exitloopNeep = false;
      bool callNeep = false;    
      bool exitloopTop = false;
      bool callTop = false;
  
      while(!exitloopNeep)
      {
         data_sw_neep = digitalRead(sw_neep);
         if(!(data_sw_neep == 0)){
          if(!callNeep){
            motor_arm_keep_start(200);
            callNeep = true;
          }
         }else{
          delay(250);
          sw_state_neep();
          delay(200);
          exitloopNeep = true;
         }
      }
      while(!exitloopTop)
      {
         data_sw_top = digitalRead(sw_top);
         if(!(data_sw_top == 1)){
          if(!callTop){
            motor_arm_slide_back(200);
            callTop = true;
          }
         }else{
          delay(100);
          sw_state_top();
           delay(200);
          exitloopTop = true;
         }
      }
      delay(500);
      if(data_sw_neep == LOW && data_sw_top == HIGH){
        box_color = box_2; 
        robot_circle_back(2,3);  
      } 
    }else if(count_sensor == 40){
      delay(100);
      motor_start(0,0);     
      delay(200);
      motor_start(70,70); 
      delay(300);    
      motor_start(0,0); 
      delay(1000); 
      
      bool exitloopNeep = false;
      bool callNeep = false;    
      bool exitloopTop = false;
      bool callTop = false;
  
      while(!exitloopNeep)
      {
         data_sw_neep = digitalRead(sw_neep);
         if(!(data_sw_neep == 0)){
          if(!callNeep){
            motor_arm_keep_start(200);
            callNeep = true;
          }
         }else{
          delay(250);
          sw_state_neep();
          delay(200);
          exitloopNeep = true;
         }
      }
      while(!exitloopTop)
      {
         data_sw_top = digitalRead(sw_top);
         if(!(data_sw_top == 1)){
          if(!callTop){
            motor_arm_slide_back(200);
            callTop = true;
          }
         }else{
          delay(100);
          sw_state_top();
           delay(200);
          exitloopTop = true;
         }
      }
      delay(500);
      if(data_sw_neep == LOW && data_sw_top == HIGH){
        box_color = box_3; 
        robot_circle_back(2,3);  
      } 
    }else if(count_sensor == 43){
      delay(100);
      motor_start(0,0);     
      delay(200);
      motor_start(70,70); 
      delay(300);    
      motor_start(0,0); 
      delay(1000); 
      
      bool exitloopNeep = false;
      bool callNeep = false;    
      bool exitloopTop = false;
      bool callTop = false;
  
      while(!exitloopNeep)
      {
         data_sw_neep = digitalRead(sw_neep);
         if(!(data_sw_neep == 0)){
          if(!callNeep){
            motor_arm_keep_start(200);
            callNeep = true;
          }
         }else{
          delay(250);
          sw_state_neep();
          delay(200);
          exitloopNeep = true;
         }
      }
      while(!exitloopTop)
      {
         data_sw_top = digitalRead(sw_top);
         if(!(data_sw_top == 1)){
          if(!callTop){
            motor_arm_slide_back(200);
            callTop = true;
          }
         }else{
          delay(100);
          sw_state_top();
           delay(200);
          exitloopTop = true;
         }
      }
      delay(500);
      if(data_sw_neep == LOW && data_sw_top == HIGH){
        box_color = box_4; 
        robot_circle_back(2,3);  
      } 
    }


    if(count_sensor == 103){
      robot_turn_left(0, 60);
    }else if(count_sensor == 112){
      robot_turn_left(0, 59);
    }else if(count_sensor == 121){
      robot_turn_left(0, 58);
    }else if(count_sensor == 130){
      robot_turn_left(0, 57);
    }

    if(count_sensor == 60){
      returnData = 3;
    }else{
      returnData = 1;
    }  
    return returnData;
  }

  int drop_zone(){
    control_switch();//Read State From Switch
    sw_state(); // Chect Status State of Switch
    
    static int returnData;
    
    condition_motor(130, 130); 
    
    if(count_sensor == drop_line){
      bool exitloop = false;
      bool call = false;
      delay(650);
      motor_start(0,0);
      delay(100);
      control_mLeft(40, 85);
      delay(420);
      motor_start(0,0);
      delay(100);
      while(!exitloop)
      {
        data_sensor_1 = digitalRead(sensor_1);
//        delay(200);
        if(!(data_sensor_1 == 1)){
          if(!call){
            control_mLeft(50, 110);
//            control_mLeft(30, 90);
            call = true;
          }
        }else{
          delay(100);
          motor_start(0,0);      
          count_sensor *=3;
          delay(100);
          exitloop = true;
        }
      }
    }

    if(count_sensor == 151){
      delay(60);
      motor_start(0,0);
      delay(500);
      bool exitloopVang = false;
      bool callVang = false;     
      while(!exitloopVang)
      {
         data_sw_vang = digitalRead(sw_vang);
         if(!(data_sw_vang == 0)){
          if(!callVang){
            motor_arm_keep_back(200);
            callVang = true;
          }
         }else{
          delay(200);
          sw_state_vang();
          delay(200);
          exitloopVang = true;
         }
      }      
      delay(500);
      if(data_sw_vang == LOW){
        motor_back(80,75);      
        delay(500);
        robot_circle_back(2,2);
      }
      
    }else if(count_sensor == 154){
      delay(60);
      motor_start(0,0);
      delay(500);
      bool exitloopVang = false;
      bool callVang = false;     
      while(!exitloopVang)
      {
         data_sw_vang = digitalRead(sw_vang);
         if(!(data_sw_vang == 0)){
          if(!callVang){
            motor_arm_keep_back(200);
            callVang = true;
          }
         }else{
          delay(200);
          sw_state_vang();
          delay(200);
          exitloopVang = true;
         }
      }      
      delay(500);
      if(data_sw_vang == LOW){
        motor_back(80,75);      
        delay(500);
        robot_circle_back(2,2);
      }
    }else if(count_sensor == 157){
      delay(60);
      motor_start(0,0);
      delay(500);
      bool exitloopVang = false;
      bool callVang = false;     
      while(!exitloopVang)
      {
         data_sw_vang = digitalRead(sw_vang);
         if(!(data_sw_vang == 0)){
          if(!callVang){
            motor_arm_keep_back(200);
            callVang = true;
          }
         }else{
          delay(200);
          sw_state_vang();
          delay(200);
          exitloopVang = true;
         }
      }      
      delay(500);
      if(data_sw_vang == LOW){
        motor_back(80,75);      
        delay(500);
        robot_circle_back(2,2);
      }
    }else if(count_sensor == 160){
      delay(60);
      motor_start(0,0);
      delay(500);
      bool exitloopVang = false;
      bool callVang = false;     
      while(!exitloopVang)
      {
         data_sw_vang = digitalRead(sw_vang);
         if(!(data_sw_vang == 0)){
          if(!callVang){
            motor_arm_keep_back(200);
            callVang = true;
          }
         }else{
          delay(200);
          sw_state_vang();
          delay(200);
          exitloopVang = true;
         }
      }      
      delay(500);
      if(data_sw_vang == LOW){
        motor_back(80,75);      
        delay(500);
        robot_circle_back(2,2);
      }
    }

    if(count_sensor == 303){
      robot_turn_right(0, 87);
    }else if(count_sensor == 309){
      robot_turn_right(0, 86);
    }else if(count_sensor == 315){
      robot_turn_right(0, 85);
    }else if(count_sensor == 321){
      robot_turn_right(0, 84);
    }
    if(count_sensor == 88){
    bool exitloop = false;
    bool call = false;
    delay(650);
    motor_start(0,0);
    delay(100);
    while(!exitloop)
    {
      data_sensor_1 = digitalRead(sensor_1);
//      delay(200);
      if(!(data_sensor_1 == 1)){
        if(!call){
          control_mLeft(50, 110);
          call = true;
        }
      }else{
        delay(100);
        motor_start(0,0);
        start_keep_slide = 1;
        count_sensor = 1;
        back_location +=1;
        delay(100);
        exitloop = true;
      }
    }
      returnData = 0;
    } else{
      returnData = 2;
    }
    return returnData;
  }

  int back_zone(){              
    static int returnData;
    if(count_sensor == 60){
      condition_motor(100, 100);
    }else{
      condition_motor(140,140);
    }   
    if(count_sensor == 66){
      robot_turn_right(0, 49);
      count_sensor = 49;
      drop_line = 0;
      drop_line = count_sensor + box_color;
      returnData = 2;
    }else{
      returnData = 3;
    }

    
//    if(count_sensor == 60){
//      condition_motor(140, 140);
//    } 
//    else if(count_sensor == 61){
//      condition_motor(90, 90);
//    }else{
//      condition_motor(140,140);
//    } 

//    if(count_sensor == 67){
//      robot_turn_right(0, 49);
//      count_sensor = 49;
//      drop_line = 0;
//      drop_line = count_sensor + box_color;
//      returnData = 2;
//    }else{
//      returnData = 3;
//    }
    
    return returnData;
  }
 
  void control_Sensor()
  {
    //Line Sensor
    data_sensor_1 = digitalRead(sensor_1);
    data_sensor_2 = digitalRead(sensor_2);
    data_sensor_3 = digitalRead(sensor_3);
    data_sensor_4 = digitalRead(sensor_4);   
    data_sensor_5 = digitalRead(sensor_5);   
  }

  void control_switch()
  {
    //Switch Mode
    data_sw_box_1 = digitalRead(sw_box_1);
    data_sw_box_2 = digitalRead(sw_box_2);
    data_sw_box_3 = digitalRead(sw_box_3);
    data_sw_box_4 = digitalRead(sw_box_4);
    //Switch Stop Arm
    data_sw_top = digitalRead(sw_top);
    data_sw_bottom = digitalRead(sw_bottom);
    data_sw_neep = digitalRead(sw_neep);
    data_sw_vang = digitalRead(sw_vang);
  }

  void robot_turn_right(int s, int count){
    bool exitloop = false;
    bool call = false;
    delay(700);
    motor_start(0,0);
    delay(100);
    while(!exitloop)
    {
      data_sensor_5 = digitalRead(sensor_5);
//      delay(200);
      if(!(data_sensor_5 == 1)){
        if(!call){
          control_mRight(110, 50);
//control_mRight(90, 20);
          call = true;
        }
      }else{
        delay(100);
        motor_start(0,0);
        switch(s){
          case 0:
          count_sensor = count;
          break;
          case 1:
          count_sensor += count;
          break;
          case 2:
          count_sensor *= count;
          break;
        }    
        delay(100);
        exitloop = true;
      }
    }
  }

  void robot_turn_left(int s,int count){
    bool exitloop = false;
    bool call = false;
    delay(650);
    motor_start(0,0);
    delay(100);
    while(!exitloop)
    {
      data_sensor_1 = digitalRead(sensor_1);
//      delay(200);
      if(!(data_sensor_1 == 1)){
        if(!call){
          control_mLeft(50, 110);
//control_mLeft(20, 90);
          call = true;
        }
      }else{
        delay(100);
        motor_start(0,0);
        switch(s){
          case 0:
          count_sensor = count;
          break;
          case 1:
          count_sensor += count;
          break;
          case 2:
          count_sensor *= count;
          break;
        }
        delay(100);
        exitloop = true;
      }
    }
  }

  void robot_circle_back(int s, int count){
    bool exitloop = false;
    bool call = false;
    delay(100);
    motor_start(0,0);
    delay(100);
    while(!exitloop)
    {
      data_sensor_5 = digitalRead(sensor_5);
//      delay(200);
      if(!(data_sensor_5 == 1)){
        if(!call){
          control_mRight(110, 40);
//control_mRight(90, 30);
          call = true;
        }
      }else{
        delay(200);
        motor_start(0,0);
        switch(s){
          case 0:
          count_sensor = count;
          break;
          case 1:
          count_sensor += count;
          break;
          case 2:
          count_sensor *= count;
          break;
        }
        delay(200);
        exitloop = true;
      }
    }
  }

  void robot_circle_back_L(int s, int count){
    bool exitloop = false;
    bool call = false;
    delay(100);
    motor_start(0,0);
    delay(100);
    while(!exitloop)
    {
      data_sensor_1 = digitalRead(sensor_1);
//      delay(200);
      if(!(data_sensor_1 == 1)){
        if(!call){
          control_mLeft(55, 100);
//          control_mLeft(30, 90);
          call = true;
        }
      }else{
        delay(200);
        motor_start(0,0);
        switch(s){
          case 0:
          count_sensor = count;
          break;
          case 1:
          count_sensor += count;
          break;
          case 2:
          count_sensor *= count;
          break;
        }
        delay(200);
        exitloop = true;
      }
    }
  }

  void condition_motor(int pwm_L, int pwm_R)
  {     
      if (data_sensor_2 == HIGH && data_sensor_3 == HIGH && data_sensor_4 == HIGH)
      {
//        motor_start(90,90);
        motor_start(pwm_L,pwm_R); 
      } 
      else if (data_sensor_1==HIGH && data_sensor_2 == HIGH && data_sensor_3 == HIGH && data_sensor_4 == HIGH && data_sensor_5==HIGH) 
      {      
        motor_start(pwm_R,pwm_R);
      }
      else if (data_sensor_2 == LOW && data_sensor_3 == HIGH && data_sensor_4 == HIGH) 
      {    
        //control_mRight(PWM Min, PWM Max);  
        control_mRight(40,60);
      }  
      else if (data_sensor_2 ==LOW && data_sensor_3 == LOW && data_sensor_4 == HIGH) 
      {      
        control_mRight(30,60);
      }    
      else if (data_sensor_1==LOW && data_sensor_2 == LOW && data_sensor_3 == HIGH && data_sensor_4 == HIGH && data_sensor_5==HIGH) 
      {      
        control_mRight(50,70);
      }
      else if(data_sensor_1 == LOW && data_sensor_2 == LOW && data_sensor_3 == LOW && data_sensor_4 == HIGH && data_sensor_5 == HIGH)
      {
        control_mRight(40,70);
      } 
      else if(data_sensor_1 == LOW && data_sensor_2 == LOW && data_sensor_3 == LOW && data_sensor_4 == LOW && data_sensor_5 == HIGH)
      {    
        control_mRight(40,80);
      }
      
      else if (data_sensor_2 == HIGH && data_sensor_3 == HIGH && data_sensor_4 == LOW) 
      {   
        //control_mLeft(PWM Max, PWM Min);   
        control_mLeft(60,40);
      }  
       else if (data_sensor_2 == HIGH && data_sensor_3 == LOW && data_sensor_4 == LOW)
       {     
        control_mLeft(70,30);
      }   
      else if (data_sensor_1 == HIGH && data_sensor_2 == HIGH && data_sensor_3 == HIGH && data_sensor_4 == LOW && data_sensor_5 == LOW) 
      {     
        control_mLeft(70,40);        
      }   
      else if(data_sensor_1 == HIGH && data_sensor_2==HIGH && data_sensor_3 == LOW && data_sensor_4 == LOW && data_sensor_5 == LOW) 
      {
        control_mLeft(70,50);
      }
      else if(data_sensor_1 == HIGH && data_sensor_2 == LOW && data_sensor_3 == LOW && data_sensor_4 == LOW && data_sensor_5 == LOW)
      {
        control_mLeft(80,40);
      }
          
      else 
      {
        motor_start(0,0);
      }
  }//end Run Motor

  void motor_start(int pwm_l, int pwm_r)
  {
    //MOTOR RIGHT
    digitalWrite(dir_1, LOW); // USE PIN 10
    analogWrite(pwm_1, pwm_r); // USE PIN 11
  
    // MOTOR LEFT
    digitalWrite(dir_2, LOW); // USE PIN 12
    analogWrite(pwm_2, pwm_l); // USE PIN 9
  }//end of Motor Start

  void control_mRight(int pwm_l, int pwm_r) 
  {
    // MOTOR RIGHT
    digitalWrite(dir_1, LOW); // USE PIN 10
    analogWrite(pwm_1, pwm_r); // USE PIN 11
  
    // MOTOR LEFT
    digitalWrite(dir_2, HIGH); // USE PIN 12 
    analogWrite(pwm_2, pwm_l); // USE PIN 9
  } // end of control_mRight
  
  void control_mLeft(int pwm_l, int pwm_r) 
  {
    // MOTOR RIGHT
    digitalWrite(dir_1, HIGH); // USE PIN 10
    analogWrite(pwm_1, pwm_r); // USE PIN 11
  
    // MOTOR LEFT
    digitalWrite(dir_2, LOW); // USE PIN 12
    analogWrite(pwm_2, pwm_l); // USE PIN 9
  } //end of control_mLeft

  void motor_back(int pwm_l, int pwm_r){
    //MOTOR RIGHT
    digitalWrite(dir_1, HIGH); // USE PIN 10
    analogWrite(pwm_1, pwm_r); // USE PIN 11
  
    // MOTOR LEFT
    digitalWrite(dir_2, HIGH); // USE PIN 12
    analogWrite(pwm_2, pwm_l); // USE PIN 9
  }//end of Motor Back
  
  void motor_arm_keep_start(int pwm3)
  {
    //MOTOR ARM Keep
    digitalWrite(dir_3, HIGH); // USE PIN 4
    analogWrite(pwm_3, pwm3); // USE PIN 5
  }//end of Motor Arm Keep Start

  void motor_arm_keep_back(int pwm3)
  {
    //MOTOR ARM Keep
    digitalWrite(dir_3, LOW); // USE PIN 4
    analogWrite(pwm_3, pwm3); // USE PIN 5
  }//end of Motor Arm Keep Back

  void motor_arm_slide_start(int pwm4)
  {
    //MOTOR ARM Slide
    digitalWrite(dir_4, HIGH); // USE PIN 6
    analogWrite(pwm_4, pwm4); // USE PIN 7
  }//end of Motor Arm Slide Start

  void motor_arm_slide_back(int pwm4)
  {
    //MOTOR ARM Slide
    digitalWrite(dir_4, LOW); // USE PIN 6
    analogWrite(pwm_4, pwm4); // USE PIN 7   
  }//end of Motor Arm Slide Back

  void count_Line()
  {   
    if(data_sensor_1 != last_state_sensor  && data_sensor_2 != last_state_sensor && data_sensor_3 != last_state_sensor && data_sensor_4 != last_state_sensor && data_sensor_5 != last_state_sensor){
      if(data_sensor_1 == HIGH && data_sensor_2 == HIGH && data_sensor_3 == HIGH && data_sensor_4 == HIGH && data_sensor_5 == HIGH)
      {
        count_sensor++;
        Serial.print("Line: ");
        Serial.println(count_sensor);
        delay(10);
      }
      delay(20);
    }
    last_state_sensor = data_sensor_1 && data_sensor_2 && data_sensor_3 && data_sensor_4 && data_sensor_5;  
  }

  void sw_state_top()
  {
    if(data_sw_top != sw_last_state_top)
    {
      if(data_sw_top == HIGH)
      {
        motor_arm_slide_back(0);          
        Serial.println("SW Top Click: ");       
        delay(10);
      }
      delay(20);     
    }
    sw_last_state_top = data_sw_top;
  }

  void sw_state_bottom()
  {
    if(data_sw_bottom != sw_last_state_bottom)
    {
      if(data_sw_bottom == HIGH)
      {
        motor_arm_slide_start(0);  
        Serial.println("SW Bottom Click: ");
        delay(10);
      }
      delay(20);     
    }
    sw_last_state_bottom = data_sw_bottom;
  }

  void sw_state_neep()
  {
    if(data_sw_neep != sw_last_state_neep)
    {
      if(data_sw_neep == LOW)
      {
        motor_arm_keep_start(0);  
        Serial.println("SW Neep Click: ");
        delay(10);
      }
      delay(20);     
    }
    sw_last_state_neep = data_sw_neep;
  }

  void sw_state_vang()
  {
    if(data_sw_vang != sw_last_state_vang)
    {
      if(data_sw_vang == LOW)
      {
        motor_arm_keep_back(0); 
        Serial.println("SW Vang Click: ");
        delay(10);
      }
      delay(20);     
    }
    sw_last_state_vang = data_sw_vang;
  }

  void sw_state()
  {
    sw_state_top(); sw_state_bottom(); sw_state_neep(); sw_state_vang();   
  }

  void robotArm_keep_slide_box()
  {
      motor_arm_keep_start(200);
      delay(20);
      sw_state_neep();
      delay(1000);       
      motor_arm_slide_back(220);
  }

  void robotArm_vang_box()
  {
      motor_arm_keep_back(200);
      delay(20);
      sw_state_vang();
  }

  void robotArm_keep_slide_setup()
  {
      //Before use this function please set sw_state(); to top this function
      delay(10);
      if(data_sw_vang==HIGH)
      {
        motor_arm_keep_back(200);       
      }
      if(data_sw_bottom==LOW)
      {
        motor_arm_slide_start(200);         
      }
  }

  void sw_box_state()
  {
    box_color_1();box_color_2();box_color_3();box_color_4();
  }
  
  void box_color_1()
  {
    if(data_sw_box_1 != last_state_box_1)
    {
      if(data_sw_box_1 == HIGH)
      {
        box_1++;
        Serial.print("Box_1:: ");
        Serial.println(box_1);
        delay(20);
      }
      delay(50);
    }
    last_state_box_1 = data_sw_box_1;
  }
  
  void box_color_2()
  {
    if(data_sw_box_2 != last_state_box_2)
    {
      if(data_sw_box_2 == HIGH)
      {
        box_2++;
        Serial.print("Box_2:: ");
        Serial.println(box_2);
        delay(20);
      }
      delay(50);
    }
    last_state_box_2 = data_sw_box_2;
  }
  
  void box_color_3()
  {
     if(data_sw_box_3 != last_state_box_3)
     {
      if(data_sw_box_3 == HIGH)
      {
        box_3++;
        Serial.print("Box_3:: ");
        Serial.println(box_3);
        delay(20);
      }
      delay(50);
    }
    last_state_box_3 = data_sw_box_3;
  }
  
  void box_color_4()
  {
    if(data_sw_box_4 != last_state_box_4)
    {
      if(data_sw_box_4 == HIGH)
      {
        box_4++;
        Serial.print("Box_4:: ");
        Serial.println(box_4);
        delay(20);
      }
      delay(50);
    }
    last_state_box_4 = data_sw_box_4;
  }

