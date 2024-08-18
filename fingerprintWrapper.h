#pragma once
#include "fingerprint.h"
#include "Keyboard.h"

#define MAX_FINGERS 20

enum FbStatus
{
    ERROR_SENSOR_NOT_FOUND = -2,
    SUCCESS = 0,
    ERROR_NO_FINGER = -1
};

class FingerBoard
{
public:
    FingerBoard(HardwareSerial* hs);

    FbStatus Begin();
    FbStatus Begin(Serial_* hs);

    bool CmdCheck();

    int GetFingerID();
    bool AddFinger(unsigned int id);
    void DeleteAllFingers();

    void InputPassword(String pswd);
    void TypeString(String s, bool enter);
    void PressKey(uint8_t k);
    void ReleaseAll();

private:
    Serial_* debug = NULL;
    HardwareSerial* sensorSerial = NULL;
    Fingerprint sensor = NULL;

    char comdata[256], data_p; // Serial port cache data
    long time;
    long INTERVAL = 200; // ms

    bool isTouching = false;

    String passwords[MAX_FINGERS];

    volatile unsigned char FPMXX_RECEIVE_BUFFER[64];

    unsigned char FPMXX_Pack_Head[6] = { 0xEF, 0x01, 0xFF, 0xFF, 0xFF, 0xFF }; // Protocol packet header
    unsigned char FPMXX_Get_Img[6] = { 0x01, 0x00, 0x03, 0x01, 0x0, 0x05 }; // Get fingerprint image
    unsigned char FPMXX_Save_Finger[9] = { 0x01, 0x00, 0x06, 0x06, 0x01, 0x00, 0x0B, 0x00, 0x19 }; // Save fingerprint from BUFFER1 to specified location
    unsigned char FPMXX_Search[11] = { 0x01, 0x0, 0x08, 0x04, 0x01, 0x0, 0x0, 0x03, 0xE7, 0x0, 0xF8 }; // Search for fingerprint in range 0 - 999, using features from BUFFER1
    unsigned char FPMXX_Get_Device[6] = { 0x01, 0x00, 0x03, 0x16, 0x00, 0x1A };
    unsigned char FPMXX_Get_Templete_Count[6] = { 0x01, 0x00, 0x03, 0x1D, 0x00, 0x21 }; // Get total number of templates
    unsigned char FPMXX_Img_To_Buffer1[7] = { 0x01, 0x0, 0x04, 0x02, 0x01, 0x0, 0x08 }; // Place image into BUFFER1
    unsigned char FPMXX_Img_To_Buffer2[7] = { 0x01, 0x0, 0x04, 0x02, 0x02, 0x0, 0x09 }; // Place image into BUFFER2
    unsigned char FPMXX_Reg_Model[6] = { 0x01, 0x0, 0x03, 0x05, 0x0, 0x09 }; // Combine BUFFER1 and BUFFER2 into a feature template
    unsigned char FPMXX_Delete_All_Model[6] = { 0x01, 0x0, 0x03, 0x0d, 0x00, 0x11 }; // Delete all templates from fingerprint module

    void FPMXX_Cmd_Save_Finger(unsigned int storeID);
    void FPMXX_Send_Cmd(unsigned char length, unsigned char* address, unsigned char returnLength);
    void FPMXX_Cmd_Receive_Data(unsigned int r_size);
    void FPMXX_Cmd_StoreTemplate(unsigned int ID);
    bool FPMXX_Add_Fingerprint(unsigned int writeID);
    void FPMXX_Delete_All_Fingerprint();
};
