//driver pour les Dynamixels AX12
#include "Ax12driver.h"

//communication par port serie
#include <termios.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>

gchar *dir;
//fonctions internes
//renvoie la valeur des registres du moteur motorID, partant de reg et jusqu'a reg + lenght
int ax12_getRegister(int port, int motorID, int reg)
{
	unsigned char message[8];
	message[0] = 0xFF;
	message[1] = 0xFF;
	message[2] = motorID;
	message[3] = 4;
	message[4] = 2;
	message[5] = reg;
	message[6] = 1;
	message[7] = ~((motorID + 7 + reg)%256);
	write(port, message,8);
	
	usleep(520);
//	char *reponse = malloc(6*sizeof(unsigned char));
	//if(read(port, reponse, 6) > 0) 
		//return reponse[5];
	
	return -1;
}

//definie la valeur du registre reg a data
void ax12SetRegister(int port, int motorID, int reg, int data)
{
	FILE *f = fopen(dir, "w");
	fprintf(f,"0");
	fclose(f);
	
    unsigned char message[8];
    message[0] = 0xFF;
	message[1] = 0xFF;
	message[2] = motorID;
	message[3] = 4;
	message[4] = 3;
	message[5] = reg;
	message[6] = data&0xFF;	//on ne prend que l'octet de poids le plus faible
	message[7] = ~((motorID + 7 + reg + message[6])%256);
	write(port, message,8);
	
	f = fopen(dir, "w");
	fprintf(f,"1");
	fclose(f);
}

//definie la valeur du registre (reg, reg+1) a data
void ax12SetRegister2(int port, int motorID, int reg, int value)
{
	FILE *f = fopen(dir, "w");
	fprintf(f,"0");
	fclose(f);
	
    unsigned char message[9];
    message[0] = 0xFF;
	message[1] = 0xFF;
	message[2] = motorID;
	message[3] = 5;
	message[4] = 3;
	message[5] = reg;
	message[6] = value%256;	//on ne prend que l'octet de poids le plus faible
	message[7] = ((value/256) % 256 );
	message[8] = ~((motorID + 8 + reg + message[6] + message[7])%256);
	write(port, message,9);
	f = fopen(dir, "w");
	fprintf(f,"1");
	fclose(f);
	
	//~ usleep(100000);
	
	//~ //read available data
	//~ int ndata = 0;
	//~ ioctl(port, FIONREAD, &ndata);
	//~ printf("%d\n", ndata);
	
	//~ char data[ndata];
	//~ read(port, data, ndata);
	//~ for(int x = 0; x < ndata; x++)
		//~ printf("%d ", data[x]);
	//~ printf("\n");
}

void ax12SetRegister4(int port, int motorID, int reg, int data, int data2)
{
	
	FILE *f = fopen(dir, "w");
	fprintf(f,"0");
	fclose(f);
	
    unsigned char message[11];
    message[0] = 0xFF;
	message[1] = 0xFF;
	message[2] = motorID;
	message[3] = 7;
	message[4] = 3;
	message[5] = reg;
	message[6] = data%256;	//on ne prend que l'octet de poids le plus faible
	message[7] = ((data/256) % 256 );
	message[8] = data2%256;	//on ne prend que l'octet de poids le plus faible
	message[9] = ((data2/256) % 256 );
	message[10] = ~((motorID + 10 + reg + message[6] + message[7] + message[8] + message[9])%256);
	write(port, message,11);
	
	f = fopen(dir, "w");
	fprintf(f,"1");
	fclose(f);
}

//doc: voir Ax12driver.h
void ax12_setPosition(int port, int motorID, int position)
{
	ax12SetRegister2(port, motorID, 0x1e, position);
}

void ax12_setServoSpeedPos(int port, int motorID, int speed, int pos)
{
	ax12SetRegister4(port, motorID, 0x1e, speed, pos);
}


void ax12_setServoSpeed(int port, int motorID, int speed)
{
	ax12SetRegister2(port, motorID, 0x20, speed);
}

void ax12_setAngleLimit(int port, int motorID, int lowLimit, int highLimit)
{
	if(lowLimit > highLimit)
		return;
	ax12SetRegister2(port, motorID, 0x06, lowLimit);
	usleep(50000);
	ax12SetRegister2(port, motorID, 0x08, highLimit);
}

gboolean ax12_isMoving(int port, int motorID)
{
	if(ax12_getRegister(port, motorID, 0x2E) == 1)
		return TRUE;
	return FALSE;
}

void ax12_servoLed(int port, int motorID, gboolean on)
{
	ax12SetRegister(port, motorID, 0x19, on);
}

void ax12_torqueOn(int port, int motorID, gboolean on)
{
	ax12SetRegister(port, motorID, 0x18, on);
}

//open serial port for servo
int ax12_init(char *port, int speed, int dirPin) {
	int fd;
	struct termios options;

	fd = open(port, O_RDWR | O_NOCTTY | O_NDELAY);

	if (fd == -1) {
	  printf("Erreur: impossible d'ouvrir %s\n", port);
	  return -1;
	}
	else
	fcntl(fd, F_SETFL, FNDELAY);	//ne pas attendre a la lecture

	//option de communication
	tcgetattr(fd, &options);

	//vitesse
	cfsetispeed(&options, speed);
	cfsetospeed(&options, speed);

	options.c_cflag |= (CLOCAL | CREAD);
	//8 bit, 1 stop, no parity
	options.c_cflag &= ~PARENB;
	options.c_cflag &= ~CSTOPB;
	options.c_cflag &= ~CSIZE;
	options.c_cflag |= CS8;
	tcsetattr(fd, TCSANOW, &options);
	
	
	//setup direction pin
	FILE *f = fopen("/sys/class/gpio/export", "a");
	dir= g_strdup_printf("%d", dirPin);
	fprintf(f,dir);
	fclose(f);
	g_free(dir);
	
	dir = g_strdup_printf("/sys/class/gpio/gpio%d/direction", dirPin);
	f = fopen(dir, "w");
	g_free(dir);
	fprintf(f, "out");
	fclose(f);
	
	dir = g_strdup_printf("/sys/class/gpio/gpio%d/value", dirPin);
	
	f = fopen(dir, "w");
	fprintf(f,"1");
	fclose(f);
	
  return (fd);
}
