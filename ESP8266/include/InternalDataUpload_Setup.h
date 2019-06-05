#include <Arduino.h>
#include "StorageManager.h"

void InternalDataUpload_Main()
{
    StorageTemplate* Template = new StorageTemplate();

	StorageManager::Begin<StorageTemplate>();
	StorageManager::RestoreData<StorageTemplate>(Template);
	StorageManager::End();
}