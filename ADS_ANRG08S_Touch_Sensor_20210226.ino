// ADS Touch Sensor Test Example Program (IC P/N:ANRG08S )
// Code:
// Date: 2021.02.26  Ver.: 0.0.1
// H/W Target: ARDUINO UNO R3, S/W: Arduino IDE  1.8.13
// Author: Park, Byoungbae (yni2yni@hanmail.net)
// Note: More information? Please ,send to e-mail.
// Uno R3, A4:SDA, A5: SCL, Leonardo 2:SDA,3:SCL
// Register setting values are subject to change without prior notice to improve touch operation.

#include <Wire.h>

#define LF        0x0A //New Line
#define CR        0x0D //Carriage  return
#define SPC       0x20 //Space

#define Ch_enable    0x01 //Touch Channel Enable/Disable
#define i2c_id       0x06 //I2C Address od ANRG08S
#define Clock_ctrl   0x34 //Clock Control Register (init_cal_opt, Reserved, clk_sel, rb_sel)
#define Global_ctrl1 0x36 //Global Option Control Register1
                          //(response_off_ctrl, response_ctrl, bf_mode, Software Reset)
#define State_count  0x37 //Cal_pre_scale
#define Global_ctrl2 0x38 //Global Option Control Register2
                          //(imp_sel,Single/Multi ,Cal_Hold_time,Reserved, ck_off)

// Sensitivity level (threshold, Register Value X 0.025% = (1 Step=0.025%)	
// The lower value of these register ANRG08 has, the higher sensitivity ANRG08 has. 
// And if user wants to set higher sensitivity over 0.9%,						
#define Sensitivity1  0x39 //ch1,Default: 0x1C X 0.025% = 0.70% (threshold)
#define Sensitivity2  0x3A //ch2
#define Sensitivity3  0x3B //ch3
#define Sensitivity4  0x3C //ch4
#define Sensitivity5  0x3D //ch5
#define Sensitivity6  0x3E //ch6
#define Sensitivity7  0x3F //ch7
#define Sensitivity8  0x40 //ch8

#define Cal_speed1 0x41 //Calibration Speed Control at BF mode
#define Cal_speed2 0x42 //Calibration Speed Control at BS mode

#define PWM_ctrl1	0x43 //LED PWM control Register (D2,D1)
#define PWM_ctrl2	0x44 //LED PWM control Register (D4,D3)
#define PWM_ctrl3	0x45 //LED PWM control Register (D6,D5)
#define PWM_ctrl4	0x46 //LED PWM control Register (D8,D7)

#define Port_Mode	0x4F //Select the output port operation mode of each channel.
#define prom_cmd	0x5C //EEPROM Control Register
#define prom_addr	0x5F //EEPROM data address select register

#define Output  0x2A //Touch Output Data Register  (ch = 1bit)

#define ANRG08S_ID  0x24 //0x48 >> 1 (0b0100100 7bit + W/R Bit1,  7bit=0x24, 8bit=0x48)

void  Init_ANRG08S(void); //Initialize ANRG08S

#define RESET_PIN 7 //Reset pin
#define EN_PIN    6 //I2C Enable Pin

void Register_Dump()
{
   byte read_data[1] = {0};

   for (int i = 0; i < 256; i += 16)
   {
      for (int j = 0; j <= 15; j++)
      {
         Wire.beginTransmission(ANRG08S_ID);   // sned ic slave address
         Wire.write((i + j));                 // sends register address
         Wire.endTransmission();              // stop transmitting
         Wire.requestFrom(ANRG08S_ID, 1);      // key data read (2 byte)
         read_data[0] = Wire.read();          //Key 1~8
         Wire.endTransmission();
         print2hex(read_data, 1); //
      }
      Serial.write(LF);
      Serial.write(CR);
   }
   delay(500);
}

void print2hex(byte *data, byte length) //Print Hex code
{
   Serial.print("0x");
   for (int i = 0; i < length; i++)
   {
      if (data[i] < 0x10)
      {
         Serial.print("0");
      }
      Serial.print(data[i], HEX);
      Serial.write(SPC);
   }
}

void setup(){
  delay(100); //wait for 100[msec]
  
  Wire.begin();        // join i2c bus (address optional for master)
  Wire.setClock(200000); // 200Khz
  Serial.begin(115200);  // start serial for output (Spped)
  // put your setup code here, to run once:
 
  pinMode(RESET_PIN, OUTPUT);
  pinMode(EN_PIN, OUTPUT);

//---------- Initialize ANRG08S --------------------------------------------------
  delay(500);    //please waiting 500[msec] after ic power on.
  Init_ANRG08S(); //Initialize ANRG08S
  delay(10); //wait for 10[msec]
//---------------------- End --------------------------------------------------

}
void loop() {

   byte read_data[2] = {0};

   Wire.beginTransmission(ANRG08S_ID); // sned ic slave address
   Wire.write(byte(Output));          // sends register address
   Wire.endTransmission();            // stop transmitting
   Wire.requestFrom(ANRG08S_ID, 1);    // key data read (1 byte)
   while (Wire.available())
   {
      read_data[0] = Wire.read();
   }
  
   Serial.write(10);
   Serial.print("-------Touch Sensor Output Data  ---- > "); // Test Code

   print2hex(read_data, 1);
   // Serial.print(read_data, HEX);
   //Serial.write(SPC);
   //Serial.write(LF);
   //Serial.write(CR);

   //Register_Dump();
   delay(50);   
}

void  Init_ANRG08S(void)
{
   //------------------ Software Reset Enable (Set)----------------------
   Wire.beginTransmission(ANRG08S_ID); // 
   Wire.write(byte(Global_ctrl1)); // 
   Wire.write(byte(0x4D)); // 
   Wire.endTransmission(); // 
   
// --------------- Hidden Register Start ---------------------------------
// user does not change the register. please contact us to change.
// -----------------------------------------------------------------------
   Wire.beginTransmission(ANRG08S_ID);// 
   Wire.write(byte(0x05)); // address
   Wire.write(byte(0x80)); // data
   Wire.endTransmission(); //   

   Wire.beginTransmission(ANRG08S_ID);// 
   Wire.write(byte(0x08)); // address
   Wire.write(byte(0x11)); //0x08h
   Wire.write(byte(0x11)); //0x09h
   Wire.write(byte(0x11)); //0x0Ah
   Wire.write(byte(0x11)); //0x0Bh
   Wire.write(byte(0x11)); //0x0Ch
   Wire.write(byte(0x11)); //0x0Dh
   Wire.write(byte(0xA1)); //0x0Eh
   Wire.write(byte(0x10)); //0x0Fh   
   Wire.endTransmission(); //   

   Wire.beginTransmission(ANRG08S_ID);// 
   Wire.write(byte(0x10)); // address
   Wire.write(byte(0xCE)); //0x10h  // IC Reset Value: 0xF5 (-0.55%) ->0xCE (-2.55%)
   Wire.write(byte(0x20)); //0x11h
   Wire.write(byte(0xFF)); //0x12h
//Wire.write(byte(0x88));  //0x12h
//Change the value of the register 0x12h when a problem occurs due to voltage drop
   Wire.write(byte(0x92)); //0x13h
   //Wire.write(byte(0x83)); //0x14h  //IC Reset Value: 0x83 (3ch) -> 0x87(7ch)
   Wire.write(byte(0x87)); //0x14h  //IC Reset Value: 0x83 (3ch) -> 0x87(7ch)
   Wire.write(byte(0x73)); //0x15h
   Wire.write(byte(0x64)); //0x16h
   Wire.write(byte(0xFF)); //0x17h 
   Wire.write(byte(0x2B)); //0x18h  
   Wire.write(byte(0x11)); //0x19h
   Wire.write(byte(0x03)); //0x1Ah  // IC Reset Value: 0x01 ->0x03
   Wire.write(byte(0xFF)); //0x1Bh  
   Wire.write(byte(0x10)); //0x1Ch 
   Wire.write(byte(0xFF)); //0x1Dh 
   Wire.write(byte(0xFF)); //0x1Eh 
   Wire.write(byte(0xFD)); //0x1Fh 
   Wire.write(byte(0x7F)); //0x20h 
   Wire.write(byte(0x00)); //0x21h
   Wire.write(byte(0xC0)); //0x22h
   Wire.write(byte(0x00)); //0x23h
   Wire.write(byte(0xC0)); //0x24h
   Wire.write(byte(0x00)); //0x25h
   Wire.write(byte(0xFF)); //0x26h
   Wire.write(byte(0xFF)); //0x27h   
   Wire.write(byte(0xFF)); //0x28h   
   Wire.write(byte(0xFF)); //0x29h   
   Wire.endTransmission(); //  

   Wire.beginTransmission(ANRG08S_ID);// 
   Wire.write(byte(0x2B)); // address 
   Wire.write(byte(0x00)); // data
   Wire.endTransmission(); // 

   Wire.beginTransmission(ANRG08S_ID);// 
   Wire.write(byte(0x2C)); // address 
   Wire.write(byte(0x00)); // data
   Wire.endTransmission(); // 
      
   Wire.beginTransmission(ANRG08S_ID);//
   Wire.write(byte(0x35));// address
   //Wire.write(byte(0xC0)); //
   //IC Reset Value = 0xC0 (Sensing Frequency Low) -> 0x40 (Sensing Frequency High), Bit 7
   Wire.write(byte(0x85)); // -% Cal. Hole time Fast Recovery (bit6 ~ bit0 = Recovery time)
   Wire.endTransmission(); //

   Wire.beginTransmission(ANRG08S_ID);// 
   Wire.write(byte(0x47)); // address
   Wire.write(byte(0x0D)); // data
   Wire.endTransmission(); //   
   
   Wire.beginTransmission(ANRG08S_ID);// 
   Wire.write(byte(0x48)); // address
   Wire.write(byte(0x00)); // data
   Wire.endTransmission(); //   
   
   Wire.beginTransmission(ANRG08S_ID);// 
   Wire.write(byte(0x49)); // address
   Wire.write(byte(0x80)); // data
   Wire.endTransmission(); //    

   Wire.beginTransmission(ANRG08S_ID);// 
   Wire.write(byte(0x4A)); // address
   Wire.write(byte(0x08)); // data
   Wire.endTransmission(); //    
   
   Wire.beginTransmission(ANRG08S_ID);// 
   Wire.write(byte(0x59)); // address
   Wire.write(byte(0x00)); // data
   Wire.endTransmission(); //    
   
   Wire.beginTransmission(ANRG08S_ID);// 
   Wire.write(byte(0x5A)); // address
   Wire.write(byte(0x00)); // data
   Wire.endTransmission(); //    
   
   Wire.beginTransmission(ANRG08S_ID);// 
   Wire.write(byte(0x5F)); // address
   Wire.write(byte(0x00)); // data
   Wire.endTransmission(); //    

   Wire.beginTransmission(ANRG08S_ID);// 
   Wire.write(byte(0x60)); // address
   Wire.write(byte(0x00)); // data
   Wire.endTransmission(); //    

   Wire.beginTransmission(ANRG08S_ID);// 
   Wire.write(byte(0x61)); // address
   Wire.write(byte(0x00)); // data
   Wire.endTransmission(); // 

   Wire.beginTransmission(ANRG08S_ID);// 
   Wire.write(byte(0x7F)); // address
   Wire.write(byte(0x00)); // data
   Wire.endTransmission(); // 
// --------------- Hidden Register End-------------------------------

// ---------------- user code ---------------------//   
   Wire.beginTransmission(ANRG08S_ID);// 
   Wire.write(byte(Ch_enable)); // 0x01h
   Wire.write(0xFF); // 0:Touch Key Disable, 1: Touch Key Enable
   Wire.endTransmission(); // 

   Wire.beginTransmission(ANRG08S_ID);// 
   Wire.write(byte(Clock_ctrl)); //  0x34h
   Wire.write(0x65);       // //IC Reset value 0x05 -> 0x65
   Wire.endTransmission(); // 

   Wire.beginTransmission(ANRG08S_ID);// 
   Wire.write(byte(Global_ctrl1)); // 0x36h 
   Wire.write(0x4D); // 
   Wire.endTransmission(); // 

   Wire.beginTransmission(ANRG08S_ID);// 
   Wire.write(byte(State_count)); // 0x37
   Wire.write(0xE6);       // //IC Reset value 0xFF -> 0xE6
   Wire.endTransmission(); //   
   
   Wire.beginTransmission(ANRG08S_ID);// 
   Wire.write(byte(Global_ctrl2)); // 0x38h
   Wire.write(0xFC); // Low imp. Multi, touch out expire time enable
   // Wire.write(0xC0); // touch out expire time disable
   Wire.endTransmission(); //  
   
//------------ Sensitivity control  -----------------------------------
   Wire.beginTransmission(ANRG08S_ID);// 
   Wire.write(byte(Sensitivity1)); // 0x39h
   Wire.write(0x24); // HEX x 0.025 = 0.90%
   Wire.endTransmission(); //      

   Wire.beginTransmission(ANRG08S_ID);// 
   Wire.write(byte(Sensitivity2)); // 0x3Ah
   Wire.write(0x24); // HEX x 0.025 = 0.90%
   Wire.endTransmission(); //   
   
   Wire.beginTransmission(ANRG08S_ID);// 
   Wire.write(byte(Sensitivity3)); // 0x3Bh
   Wire.write(0x24); // HEX x 0.025 = 0.90%
   Wire.endTransmission(); //   
   
   Wire.beginTransmission(ANRG08S_ID);// 
   Wire.write(byte(Sensitivity4)); // 0x3Ch
   Wire.write(0x24); // HEX x 0.025 = 0.90%
   Wire.endTransmission(); //   
   
   Wire.beginTransmission(ANRG08S_ID);// 
   Wire.write(byte(Sensitivity5)); // 0x3Dh
   Wire.write(0x24); // HEX x 0.025 = 0.90%
   Wire.endTransmission(); //   
   
   Wire.beginTransmission(ANRG08S_ID);// 
   Wire.write(byte(Sensitivity6)); // 0x3Eh
   Wire.write(0x24); // HEX x 0.025 = 0.90%
   Wire.endTransmission(); //   
   
   Wire.beginTransmission(ANRG08S_ID);// 
   Wire.write(byte(Sensitivity7)); // 0x3Fh
   Wire.write(0x24); // HEX x 0.025 = 0.90%
   Wire.endTransmission(); //   
   
   Wire.beginTransmission(ANRG08S_ID);// 
   Wire.write(byte(Sensitivity8)); // 0x40h
   Wire.write(0x24); // HEX x 0.025 = 0.90%
   Wire.endTransmission(); //  
 
// ------------ Calibration Speed Control ------------------------ 
   Wire.beginTransmission(ANRG08S_ID);// 
   Wire.write(byte(Cal_speed1)); // 0x41h
   //Wire.write(0x66);             // default value
   Wire.write(0x67);
   Wire.endTransmission(); //   
   
   Wire.beginTransmission(ANRG08S_ID);// 
   Wire.write(byte(Cal_speed2)); // 0x42h
   //Wire.write(0x66);             // default value
   Wire.write(0x67);
   Wire.endTransmission(); //   
// -------------------- LED PWM Control -----------------------------
   Wire.beginTransmission(ANRG08S_ID);// 
   Wire.write(byte(PWM_ctrl1)); // 0x43h
   Wire.write(0x00); // D2,D1
   Wire.endTransmission(); //

   Wire.beginTransmission(ANRG08S_ID);// 
   Wire.write(byte(PWM_ctrl2)); // 0x44h
   Wire.write(0x00); // D4,D3
   Wire.endTransmission(); //

   Wire.beginTransmission(ANRG08S_ID);// 
   Wire.write(byte(PWM_ctrl3)); // 0x45h
   Wire.write(0x00); // D6,D5
   Wire.endTransmission(); //

   Wire.beginTransmission(ANRG08S_ID);// 
   Wire.write(byte(PWM_ctrl4)); // 0x46h
   Wire.write(0x00); // D8,D7
   Wire.endTransmission(); //
// ---------------- Port Mode Control Register --------------------------
   Wire.beginTransmission(ANRG08S_ID);// 
   Wire.write(byte(Port_Mode)); // 0x4Fh
   Wire.write(0x00); // 0: Parallel output mode, 1: LED Drive mode
   Wire.endTransmission(); //

//------------------ Software Reset Disable (Clear) ---------------------
   Wire.beginTransmission(ANRG08S_ID); // 
   Wire.write(byte(Global_ctrl1)); // 
   Wire.write(byte(0x4C)); // Respone Off =3, Response ON = 4, Normal Mode
   //Wire.write(byte(0x28)); // Respone Off =2, Response ON = 3, Normal Mode
   Wire.endTransmission(); // 
    
   }
// End

