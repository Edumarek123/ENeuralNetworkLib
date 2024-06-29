#include "src/eneural_network_lib/eneural_network_lib.hpp"

void setup()
{
  //SERIAL
  Serial.begin(115200);
    int hn[] = {1};
    ENeuralNetwork(2, 1, hn, 1, 1);

  
}

void loop() 
{

}
