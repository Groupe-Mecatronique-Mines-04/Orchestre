#include "mbed.h"
#include <string>

Serial pc(USBTX, USBRX); // tx, rx
PwmOut led1(LED1);
PwmOut led2(LED2);
PwmOut led3(LED3);
PwmOut led4(LED4);

char buffer_read;
string commande;
int position=0;
int vitesse=0;

void changerTension(int tension, int vitesse)
{
    led1=1;
}
void changerPlectre(int position, int vitesse)
{
    led2=1;
}
void requeteTension()
{
}
void requetePlectre()
{
}
void requeteMicro()
{
}


int main()
{
    commande.clear();
    pc.printf("Waiting...");
    while(1) {
        while(pc.readable()) {
            buffer_read = pc.getc();
            commande += buffer_read;
            if (buffer_read == '$') {
                commande.clear();
                position = 0;
                vitesse = 0;
              /* led1=0;
                led2=0;
                led3=0;
                led4=0;*/
                pc.printf("reset\n");
            }
        }
        if (buffer_read == '#') {
            for(int i=2; i<6; i++) {
                if (commande[i] >= '0' && commande[i] <= '9') {
                    position = position*16 + (commande[i]-48);
                } else if (commande[i] >= 'A' && commande[i] <= 'F') {
                    position = position*16 + (commande[i]-55);
                } else if (commande[i] >= 'a' && commande[i] <= 'f') {
                    position = position*16 + (commande[i]-87);
                }
            }
            for(int i=6; i<10; i++) {
                if (commande[i] >= '0' && commande[i] <= '9') {
                    vitesse = vitesse*16 + (commande[i]-48);
                } else if (commande[i] >= 'A' && commande[i] <= 'F') {
                    vitesse = vitesse*16 + (commande[i]-55);
                } else if (commande[i] >= 'a' && commande[i] <= 'f') {
                    vitesse = vitesse*16 + (commande[i]-87);
                }
            }
            if (commande[0] == '0') {
                if (commande[1] == '0') {
                    changerTension(position,vitesse);
                }
                if (commande[1] == '1') {
                    changerPlectre(position,vitesse);
                }
            } else if (commande[0] == '1') {
                if (commande[1] == '0') {
                    requeteTension();
                }
                if (commande[1] == '1') {
                    requetePlectre();
                }
                if (commande[1] == '2') {
                    requeteMicro();
                }
            } else {
            }
            buffer_read = 0;
        }
    }
}