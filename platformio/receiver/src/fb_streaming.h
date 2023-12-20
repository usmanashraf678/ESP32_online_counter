
#pragma once
#include "global_variables.h"

void streaming_setup()
{

  //In setup(), set the stream callback function to handle data
  //streamCallback is the function that called when database data changes or updates occurred
  //streamTimeoutCallback is the function that called when the connection between the server
  //and client was timeout during HTTP stream

  Firebase.setStreamCallback(fbdo, streamCallback, streamTimeoutCallback);

  //In setup(), set the streaming path to "/test/data" and begin stream connection

  if (!Firebase.beginStream(fbdo, "/dr_umair_counter"))
  {
    //Could not begin stream connection, then print out the error detail
    Serial.println(fbdo.errorReason());
  }
}

//Global function that handles stream data
void streamCallback(StreamData data)
{

  //Print out all information

  Serial.println("Stream Data...");
  Serial.println(data.streamPath());
  Serial.println(data.dataPath());
  Serial.println(data.dataType());

  //Print out the value
  //Stream data can be many types which can be determined from function dataType

  if (data.dataTypeEnum() == fb_esp_rtdb_data_type_integer)
    Serial.println(data.to<int>());
  else if (data.dataTypeEnum() == fb_esp_rtdb_data_type_string)
    Serial.println(data.to<String>());
}

//Global function that notifies when stream connection lost
//The library will resume the stream connection automatically
void streamTimeoutCallback(bool timeout)
{
  if (timeout)
  {
    //Stream timeout occurred
    Serial.println("Stream timeout, resume streaming...");
  }
}

void streaming_loop()
{
  if (!Firebase.readStream(fbdo))
  {
    Serial.println(fbdo.errorReason());
  }

  if (fbdo.streamTimeout())
  {
    Serial.println("Stream timeout, resume streaming...");
    Serial.println();
  }

  if (fbdo.streamAvailable())
  {

    if (fbdo.dataTypeEnum() == fb_esp_rtdb_data_type_integer)
      Serial.println(fbdo.to<int>());
    else if (fbdo.dataTypeEnum() == fb_esp_rtdb_data_type_string)
      Serial.println(fbdo.to<String>());
  }
}
