//éviter d'inclure le fichier deux fois
#ifndef AX12T_SEEN
	#define AX12T_SEEN
	#include <glib.h>
    
    int ax12_getRegister(int port, int motorID, int reg);
    void ax12_setPosition(int port, int motorID, int position);						//deplace le moteur motorID jusqu'a position
    void ax12_setServoSpeed(int port, int motorID, int speed);						//si le moteur est en mode rotation continue, definie la vitesse du moteur
    void ax12_setServoSpeedPos(int port, int motorID, int speed, int pos);						//si le moteur est en mode rotation continue, definie la vitesse du moteur
    void ax12_setAngleLimit(int port, int motorID, int lowLimit, int highLimit);	//defini la limite angulaire du servo; (0,0) pour passer en rotation continue
    gboolean ax12_isMoving(int port, int motorID);										//renvoie vrai si le servo est en train de tourner 
    void ax12_servoLed(int port, int motorID, gboolean on);								//allume ou eteint la led sur le servo
    void ax12_torqueOn(int port, int motorID, gboolean on);								//couple on/off
    int ax12_init(char *port, int speed, int dirPin);								//connection au port serie
#endif 
