#ifdef  UPLOAD_INTERNAL_DATA
#include "InternalDataUpload_Setup.h"

void setup()
{
	Serial.begin(74880);
	InternalDataUpload_Main();
}

void loop() 
{
	//InternalDataUpload_Loop();
}
#else
#include "DefaultUpload_Setup.h"
void setup()
{
	Serial.begin(115200);
	DefaultUpload_Setup();
}

void loop() 
{
	DefaultUpload_Loop();
}
#endif