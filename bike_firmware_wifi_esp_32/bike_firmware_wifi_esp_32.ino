#include <WiFi.h>           // Use this for WiFi instead of Ethernet.h
#include <HTTPClient.h>
#include <ezButton.h> //антидребезг
#include <ESP32Servo.h> // подключение библиотеки

String serverName = "http://koshelevzz.ru/bike/index.php";
// WiFi card example
char ssid[] = "Kotov";         // your SSID
char pass[] = "11155515";     // your SSID Password
WiFiClient client;                 // Use this for WiFi instead of EthernetClient
String payload;        // переменная для складывания данных с сервера
uint8_t Ary[10];       //для парсинга
uint8_t i = 0, j = 0;  //для парсинга


const int back_servo_pin = 5;    //D1
const int front_servo_pin = 2;   //для примера, не знаю какой пока
int back_servo_feedbackPin = 36;
int front_servo_feedbackPin = 39;



Servo back_servo;         // создание объекта для сервопривода
Servo front_servo;        // создание объекта для сервопривода



const int button_up_gear = 34;   //D0
const int button_down_gear = 35;  //D2
bool button_up_flag = false;
bool button_down_flag = false;
bool two_buttons_pressed_flag = false;
ezButton button_up(button_up_gear);      // create ezButton object for pin D7
ezButton button_down(button_down_gear);  // create ezButton object for pin D7




int bike_gear;        // there should be value readed from eeprom or analog. Its current gear
int max_gear_amount = 9;  // начиная с 0. Итого 10

int back_gears_servo_position_array[] = { 5, 30, 42, 54, 66, 78, 90 };
int front_gears_servo_position_array[] = { 10, 20, 30 };

int back_gears_servo_position;// = back_gears_servo_position_array[2];    // значение передающееся на сервопривод
int front_gears_servo_position;// = front_gears_servo_position_array[2];  // значение передающееся на сервопривод




int get_nearest_angle(int x, int angle_array[])
{
  int idx = 0; // by default near first element
  int distance = abs(angle_array[idx] - x);
  for (int i=1; i<18; i++)
  {
    int d = abs(angle_array[i] - x);
    if (d < distance)
    {
      idx = i;
      distance = d;
    }
  }
  ///return idx;
  return angle_array[idx];
}




void get_values_from_server() {

  // Begin WiFi section
  Serial.printf("\nConnecting to %s", ssid);
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  // print out info about the connection:
  Serial.println("\nConnected to network");
  Serial.print("My IP address is: ");
  Serial.println(WiFi.localIP());


HTTPClient http;
http.begin(serverName.c_str());
      // Send HTTP GET request
      int httpResponseCode = http.GET();
      
      if (httpResponseCode>0) {
        payload = http.getString();
        //Serial.println(payload);

               
      }
      else {
        Serial.print("Error code: ");
        Serial.println(httpResponseCode);
      }
      // Free resources    
      http.end();




   while ( j < payload.length())
  {
    if (payload.charAt(j) == ',')
    {
      i++;
    }
    else
    {
      Ary[i] = Ary[i] * 10;
      if ((payload.charAt(j) < '0') || (payload.charAt(j) > '9'))
      {
        Ary[i] = 0;
      }
      else
      {
        Ary[i] = Ary[i] + (payload.charAt(j) - '0');
      }
    }
    j++;
  }
  for (int i = 0; i < 10; i++)
  {
    //Serial.print(Ary[i]); //shows 1 2 3 4 5
    Serial.print(' ');
    
  }    
  Serial.println("");

  for (int i = 0; i < 7; i++)
  {
    back_gears_servo_position_array[i]=Ary[i];
  }    
  for (int i = 0; i < 7; i++)
  {
    Serial.print(back_gears_servo_position_array[i]); //shows 1 2 3 4 5
    Serial.print(' ');
  }    
  Serial.println("");



   j=0;
   for (int i = 7; i < 10; i++)
  {
    front_gears_servo_position_array[j]=Ary[i];
    j++;
  }  
  
  for (int i = 0; i < 3; i++)
  {
    Serial.print(front_gears_servo_position_array[i]); //shows 1 2 3 4 5
    Serial.print(' ');
  }    
  Serial.println("");

    WiFi.disconnect();
     WiFi.mode(WIFI_OFF);
}





void set_speed() {

  switch (bike_gear) {
    case 0:
      front_gears_servo_position = front_gears_servo_position_array[0];
      back_gears_servo_position = back_gears_servo_position_array[6];
      break;
    case 1:
      front_gears_servo_position = front_gears_servo_position_array[0];
      back_gears_servo_position = back_gears_servo_position_array[5];
      break;
    case 2:
      front_gears_servo_position = front_gears_servo_position_array[0];
      back_gears_servo_position = back_gears_servo_position_array[4];
      break;
    case 3:
      front_gears_servo_position = front_gears_servo_position_array[1];
      back_gears_servo_position = back_gears_servo_position_array[5];
      break;

    case 4:
      front_gears_servo_position = front_gears_servo_position_array[1];
      back_gears_servo_position = back_gears_servo_position_array[4];
      break;


    case 5:
      front_gears_servo_position = front_gears_servo_position_array[1];
      back_gears_servo_position = back_gears_servo_position_array[3];
      break;

    case 6:
      front_gears_servo_position = front_gears_servo_position_array[1];
      back_gears_servo_position = back_gears_servo_position_array[2];
      break;

    case 7:
      front_gears_servo_position = front_gears_servo_position_array[2];
      back_gears_servo_position = back_gears_servo_position_array[2];
      break;


    case 8:
      front_gears_servo_position = front_gears_servo_position_array[2];
      back_gears_servo_position = back_gears_servo_position_array[1];
      break;


    case 9:
      front_gears_servo_position = front_gears_servo_position_array[2];
      back_gears_servo_position = back_gears_servo_position_array[0];
      break;
  }

  Serial.println();
  Serial.print("back_gears_servo_position");
  Serial.print(" ");
  Serial.println(back_gears_servo_position);

  Serial.print("front_gears_servo_position");
  Serial.print(" ");
  Serial.println(front_gears_servo_position);
  Serial.println();
  
  
}






int get_speed_at_startup(int current_back_speed, int current_front_speed ) {
  int curr_speed;
  if ((current_front_speed == front_gears_servo_position_array[0])&&(current_back_speed == back_gears_servo_position_array[6])){
      curr_speed = 0; 
  }

   if ((current_front_speed == front_gears_servo_position_array[0])&&(current_back_speed == back_gears_servo_position_array[5])){
      curr_speed = 1; 
  }

   if ((current_front_speed == front_gears_servo_position_array[0])&&(current_back_speed == back_gears_servo_position_array[4])){
      curr_speed = 2; 
  }

   if ((current_front_speed == front_gears_servo_position_array[1])&&(current_back_speed == back_gears_servo_position_array[5])){
      curr_speed = 3; 
  }

   if ((current_front_speed == front_gears_servo_position_array[1])&&(current_back_speed == back_gears_servo_position_array[4])){
      curr_speed = 4; 
  }


   if ((current_front_speed == front_gears_servo_position_array[1])&&(current_back_speed == back_gears_servo_position_array[3])){
      curr_speed = 5; 
  }
      

   if ((current_front_speed == front_gears_servo_position_array[1])&&(current_back_speed == back_gears_servo_position_array[2])){
      curr_speed = 6; 
  }


   if ((current_front_speed == front_gears_servo_position_array[2])&&(current_back_speed == back_gears_servo_position_array[2])){
      curr_speed = 7; 
  }


   if ((current_front_speed == front_gears_servo_position_array[2])&&(current_back_speed == back_gears_servo_position_array[1])){
      curr_speed = 8; 
  }


   if ((current_front_speed == front_gears_servo_position_array[2])&&(current_back_speed == back_gears_servo_position_array[0])){
      curr_speed = 9; 
  }
  
  Serial.println();
  Serial.print("current speed is ");
  Serial.println(curr_speed);
  return curr_speed;
  ///delete curr_speed;
  
}










void button_processing() {



  /////////up button processing//////////////
  if (button_up.isPressed() && !button_down.isPressed() && button_up_flag == false) {
    if (bike_gear <= (max_gear_amount - 1)) {
      bike_gear++;
      button_up_flag = true;
      Serial.println("increasing");
      Serial.println(bike_gear);
      set_speed();
      
      back_servo.attach(back_servo_pin, 544, 2444);    // указываем управляющий пин, параметр min и max // servo attach D3 pin of arduino
      front_servo.attach(front_servo_pin, 544, 2444);  // указываем управляющий пин, параметр min и max // servo attach D3 pin of arduino
      
      back_servo.write(back_gears_servo_position);    // set servo pos
      front_servo.write(front_gears_servo_position);  // set servo pos

      back_servo.detach(); 
      front_servo.detach(); 
    }
  }
  if (button_up.isReleased() && button_up_flag == true) {
    button_up_flag = false;
  }





  //////////down button processing//////////////
  if (button_down.isPressed() && !button_up.isPressed() && button_down_flag == false) {
    if (bike_gear > 0) {
      bike_gear--;
      button_down_flag = true;
      Serial.println("decreasing");
      Serial.println(bike_gear);
      set_speed();
      
      back_servo.attach(back_servo_pin, 544, 2444);    // указываем управляющий пин, параметр min и max // servo attach D3 pin of arduino
      front_servo.attach(front_servo_pin, 544, 2444);  // указываем управляющий пин, параметр min и max // servo attach D3 pin of arduino
      
      back_servo.write(back_gears_servo_position);    // set servo pos
      front_servo.write(front_gears_servo_position);  // set servo pos

      back_servo.detach(); 
      front_servo.detach(); 
    }
  }

  if (button_down.isReleased() && button_down_flag == true) {
    button_down_flag = false;
  }



  //////////2 buttons pressed processing//////////////


  if (digitalRead(button_up_gear) && digitalRead(button_down_gear) and two_buttons_pressed_flag == false) {

    Serial.println("Down and UP is pressed");
    two_buttons_pressed_flag = true;
  }

  if (!digitalRead(button_up_gear) && !digitalRead(button_down_gear) && two_buttons_pressed_flag == true) {
    Serial.println("starting to reseive values");
    get_values_from_server();
    two_buttons_pressed_flag = false;
  }
}











void setup() {

  //  analogWriteResolution(16);  //увеличиваем разрядность ШИМ до 16 бит
  //back_servo.attach(back_servo_pin, 544, 2444);    // указываем управляющий пин, параметр min и max // servo attach D3 pin of arduino
  //front_servo.attach(front_servo_pin, 544, 2444);  // указываем управляющий пин, параметр min и max // servo attach D3 pin of arduino
  
  pinMode(button_up_gear, INPUT);
  pinMode(button_down_gear, INPUT);  
  button_up.setDebounceTime(20);    // set debounce time to 50 milliseconds
  button_down.setDebounceTime(20);  // set debounce time to 50 milliseconds
 
  Serial.begin(115200);

   Serial.print("back_servo_feedback_value: ")
   Serial.println(analogRead(back_servo_feedbackPin));

   Serial.print("front_servo_feedback_value: ")
   Serial.println(analogRead(front_servo_feedbackPin));
   Serial.println()
  //int val = map(analogRead(back_servo_feedbackPin), 0, 1023, 0, 179);     //  reads the value of the potentiometer (value between 0 and 1023) and scale it to use it with the servo (value between 0 and 180) 
  back_gears_servo_position = get_nearest_angle(map(analogRead(back_servo_feedbackPin), 0, 1023, 0, 179), back_gears_servo_position_array ); //get servo angle preset
  front_gears_servo_position = get_nearest_angle(map(analogRead(front_servo_feedbackPin), 0, 1023, 0, 179), front_gears_servo_position_array );
  bike_gear = get_speed_at_startup(back_gears_servo_position, front_gears_servo_position); //get current program speed


  Serial.print("back_gears_servo_position: ");
  Serial.println(back_gears_servo_position);
  
  Serial.print("front_gears_servo_position: ");
  Serial.println(front_gears_servo_position);
  Serial.println()
   
}



int r = 0;
void loop() {

  button_up.loop();    // MUST call the loop() function first
  button_down.loop();  // MUST call the loop() function first
  button_processing();
  /* Testing of receiving values from server
    if (r ==0){
      get_values_from_server();
      r++;
    }
  */
}
