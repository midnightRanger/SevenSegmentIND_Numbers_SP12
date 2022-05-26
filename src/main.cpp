#include <Arduino.h>

#define latchPin 4
#define clockPin 7
#define dataPin 8
#define COUNTDOWN_DELAY 1000

//коды для цифр от 0 до 9 под соответствующими индексами
int digitArray[] = {~B11111100, ~B01100000, ~B11011010, ~B11110010, ~B01100110, ~B10110110, ~B10111110, ~B11100000, ~B11111110, ~B11110110, ~B00000000};
//коды для каждого разряда
int classArray[] = {0xF1, 0xF2, 0xF4, 0xF8};

//массив с цифрами числа, которое ввел пользователь
int decade_array[4];

//число, которое ввел пользователь
int number;

void setup()
{
  Serial.begin(9600);
  Serial.println("Начните ввод цифр (от 0 до 9999");
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
}

void separate_number(int num)
{
  for (int i = 3; i >= 0; i--)
  {
    decade_array[i] = num % 10;
    num /= 10;
  }

  if (decade_array[0] == 0) decade_array[0] = 10;

  for (int i = 0; i < 4; i++)
  {
    if (decade_array[i] == 10 && decade_array[i + 1] == 0)
        decade_array[i + 1] = 10;
  }
}

void set_number(int num)
{
  if (num < 0 || num > 9999)
  {
    Serial.println("Введите цифру от 0 до 9999");
    return;
  }

  long displayingMillis;
  if (millis() - displayingMillis > 16)
  {
    for (int i = 0; i < 4; i++)
    {
      digitalWrite(latchPin, LOW);
      shiftOut(dataPin, clockPin, LSBFIRST, digitArray[decade_array[i]]);
      shiftOut(dataPin, clockPin, MSBFIRST, classArray[i]);
      digitalWrite(latchPin, HIGH);
    }
  }
}

void command_handle()
{
  String input_number;
  if (Serial.available() > 0)
  {
    input_number = Serial.readString();
    number = input_number.toInt();
  }
  else
    return;
  Serial.print("Было введено значение: ");
  Serial.println(number);
  separate_number(number);
}

void loop()
{
  command_handle();
  set_number(number);
}