void processCommand(uint8_t theChoice);
uint8_t buildADDLINKADD(char *cmdAddr, uint16_t cmdMaxLen);
uint8_t buildADDLINKNAME(char *cmdAddr, uint16_t cmdMaxLen);
uint8_t getSerial_chars(void);
uint8_t getSerial_uint8_t(void);
void testEcho(void);
void sendBlueCmds(const char *cmdArray[], const uint8_t numCmds);
void sendBlueCmd(char *cmd);
unsigned int reportBlueCom(void);
unsigned int loopReportBlueCom(void);
