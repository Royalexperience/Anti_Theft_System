# Anti_Theft_System
Anti Theft System it's a simple project made to explain the simple basics of the IOT, using an Esp32.

# What do i need for a replica ?
 The whole project is made by :
- Esp32 / any other arduino with wifi integrated 
- n Wires 
- Buzzer 
- 3 different leds  
- Ultrasonic sensor

# How it works 
The working system it's really simple.
The Ultra sonic device detects a movemeent in it's range , if the movements it's beetween / over a certain distance then it lights a led, if the movement it's very close a red led will be on and the buzzer will play.

All the distance will be saved into a server , but the critical distance movements will trigger a telegram bot that will send you a message on your phone
