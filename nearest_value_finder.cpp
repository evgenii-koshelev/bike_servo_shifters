
int back_gears_servo_position_array[] = { 5, 30, 42, 54, 66, 78, 90 };


void setup() 
{
  Serial.begin(115200);
  Serial.println("Start ");
  int idx = nearest(4,back_gears_servo_position_array);
  Serial.println(back_gears_servo_position_array[idx]);
  
}


int nearest(int x, int angle_array[])
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
  return idx;
}



void loop() 
{
  if (Serial.available())
  {
    Serial.read();
    int x = random(0, 90);
    int idx = nearest(x,back_gears_servo_position_array);

    Serial.print(x);
    Serial.print("\t");
    Serial.print(idx);
    Serial.print("\t");
    Serial.println(back_gears_servo_position_array[idx]);
  }
}