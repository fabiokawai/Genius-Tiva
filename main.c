#include <tm4c123gh6pm.h> //biblioteca da placa

#include <inttypes.h> //para usar uint32_t

#include <stdlib.h> //para usar Rand

//Valores da porta E dos Leds
#define led1 2
#define led2 4
#define led3 8
#define led4 16
#define led1e4 18
#define led1234 30
#define ledNULL 0
#define releasedOn 300

//Nível máximo do jogo
#define maxLVL 20

//Valores da porta F dos botões pressionados
#define btnNULL 30
#define btn1 28
#define btn2 26
#define btn3 22
#define btn4 14

uint32_t btnValue;
uint32_t level = 1;
uint32_t sequence[maxLVL];
uint32_t playing = 1;

//LED PORT E
//BOTAO PORT F

int main(void) {

    //Habilita PORTE e PORTF
    SYSCTL_RCGCGPIO_R = 48;
    //zera PORTE e PORTF
    GPIO_PORTE_DATA_R = 0;
    GPIO_PORTF_DATA_R = 0;
    //Habilita saida pinos 1, 2, 3 e 4 no PORTE
    GPIO_PORTE_DIR_R = 30;
    //habilita PullUp no pino 1, 2, 3 e 4 no PORTF
    GPIO_PORTF_PUR_R = 30;
    //Digital Enable
    GPIO_PORTE_DEN_R = 30;
    GPIO_PORTF_DEN_R = 30;

    //Aguarda pressionar um botão, e em seguida gera a sequência de jogadas
    createSequence();

    //Executa o loop enquanto o jogador não vencer ou não errar
    while (playing == 1) {
        showSequence();
        if (playing == 1){
            readSequence();
        }
        level++;
        if (level > maxLVL && playing == 1) {
            playing = 2;
        }
    }

    // Se o jogador errou a sequência
    if (playing == 0) {
        gameOver();
    }
    //Se o jogador acertou as sequências até o último nível
    else if (playing == 2) {
        victory();
    }

    return 0;
}

//Animação de derrota
void gameOver() {
    while (1) {
        blinkLED(led1e4);
        delay(300000);
        blinkLED(ledNULL);
        delay(500000);
    }
}

//Animação de vitória
void victory() {
    while (1) {
        blinkLED(led1);
        delay(500000);
        blinkLED(led2);
        delay(500000);
        blinkLED(led3);
        delay(500000);
        blinkLED(led4);
        delay(500000);
        blinkLED(led3);
        delay(500000);
        blinkLED(led2);
        delay(500000);
        blinkLED(led1);
        delay(500000);
        blinkLED(ledNULL);
        delay(500000);
        blinkLED(led1234);
        delay(500000);
        blinkLED(ledNULL);
        delay(500000);
        blinkLED(led1234);
        delay(500000);
        blinkLED(ledNULL);
        delay(500000);

    }
}

//Pisca a sequencia do nível atual
void showSequence() {
    uint32_t xShow;
    uint32_t contAntiCitti;
    blinkLED(ledNULL);
    delay(1000000);
    for (xShow = 0; xShow < level; xShow++) {
        switch (sequence[xShow]) {
        case 1:
            blinkLED(led1);
            break;
        case 2:
            blinkLED(led2);
            break;
        case 3:
            blinkLED(led3);
            break;
        case 4:
            blinkLED(led4);
            break;
        }
        delay(1000000);
        blinkLED(ledNULL);
        delay(500000);
    }
    btnValue = GPIO_PORTF_DATA_R;
    contAntiCitti = 0;
    while(btnValue != 30 && contAntiCitti<500000){
        btnValue = GPIO_PORTF_DATA_R;
        contAntiCitti++;
        if(contAntiCitti>=4000000){
            gameOver();
        }

    }
}

//Executa as leituras e compara com a sequencia do nível atual
void readSequence() {
    uint32_t xRead;
    uint32_t contReleased = 0;
    uint32_t contTimeOut = 0;

    for (xRead = 0; xRead < level; xRead++) {
        blinkLED(ledNULL);
        contTimeOut = 0;
        btnValue = GPIO_PORTF_DATA_R;
        while (btnValue == 30 && contTimeOut<3000000) {
            btnValue = GPIO_PORTF_DATA_R;
            contTimeOut++;
        }
        if(contTimeOut>=3000000){
            gameOver();
        }
        contReleased = 0;
        contTimeOut = 0;
        switch (btnValue) {
        case btn1:
            blinkLED(led1);
            if (sequence[xRead] != 1) {
                gameOver();
            }
            while (btnValue == btn1) {
                btnValue = GPIO_PORTF_DATA_R;
                contTimeOut++;
                if ((btnValue != btnNULL && btnValue != btn1) || contTimeOut > 3000000){
                    gameOver();
                }
            }
            while (contReleased < releasedOn) {
                btnValue = GPIO_PORTF_DATA_R;
                contTimeOut++;
                if (btnValue == btnNULL) {
                    contReleased++;
                } else {
                    contReleased = 0;
                }
                if ((btnValue != btnNULL && btnValue != btn1) || contTimeOut > 3000000){
                    gameOver();
                }
            }
            break;
        case btn2:
            blinkLED(led2);
            if (sequence[xRead] != 2) {
                gameOver();
            }
             while (btnValue == btn2) {
                btnValue = GPIO_PORTF_DATA_R;
                contTimeOut++;
                if ((btnValue != btnNULL && btnValue != btn2) || contTimeOut > 3000000){
                    gameOver();
                }
            }
            while (contReleased < releasedOn) {
                btnValue = GPIO_PORTF_DATA_R;
                contTimeOut++;
                if (btnValue == btnNULL) {
                    contReleased++;
                } else {
                    contReleased = 0;
                }
                if ((btnValue != btnNULL && btnValue != btn2) || contTimeOut > 3000000){
                    gameOver();
                }
            }
            break;
        case btn3:
            blinkLED(led3);
            if (sequence[xRead] != 3) {
                gameOver();
            }
            while (btnValue == btn3) {
                btnValue = GPIO_PORTF_DATA_R;
                contTimeOut++;
                if ((btnValue != btnNULL && btnValue != btn3) || contTimeOut > 3000000){
                    gameOver();
                }
            }
                while (contReleased < releasedOn) {
                    btnValue = GPIO_PORTF_DATA_R;
                    contTimeOut++;
                    if (btnValue == btnNULL) {
                        contReleased++;
                    } else {
                        contReleased = 0;
                    }
                    if((btnValue != btnNULL && btnValue != btn3) || contTimeOut > 3000000){
                        gameOver();
                    }
                }
            break;
        case btn4:
            blinkLED(led4);
            if (sequence[xRead] != 4) {
                gameOver();
            }
            while (btnValue == btn4) {
                btnValue = GPIO_PORTF_DATA_R;
                contTimeOut++;
                if ((btnValue != btnNULL && btnValue != btn4) || contTimeOut > 3000000){
                    gameOver();
                }
            }
                while (contReleased < releasedOn) {
                    btnValue = GPIO_PORTF_DATA_R;
                    contTimeOut++;
                    if (btnValue == btnNULL) {
                        contReleased++;
                    } else {
                        contReleased = 0;
                    }
                    if ((btnValue != btnNULL && btnValue != btn4) || contTimeOut > 3000000){
                        gameOver();
                    }
                }
            break;
        }
    }
}

//Acender Led
void blinkLED(int led) {
    GPIO_PORTE_DATA_R = led;
}
//Gera Sequência aleatória e armazena
void createSequence() {
    uint32_t contSequence = 0;
    btnValue = 30;
    while (btnValue == 30) {
        contSequence++;
        btnValue = GPIO_PORTF_DATA_R;
        if (contSequence % 250000 == 0) {
            GPIO_PORTE_DATA_R ^= 30;
        }
        if (contSequence > 2000000) {
            contSequence = 0;
        }
    }
    while(btnValue!= 30){
        btnValue = GPIO_PORTF_DATA_R;
    }
    srand(contSequence);
    uint32_t x;
    for (x = 0; x < maxLVL; x++) {
        sequence[x] = rand() % 4 + 1;
    }
}

//delay
void delay(uint32_t timerMax) {
    uint32_t timer = 0;
    for (timer = 0; timer < timerMax; timer++) {
    }
}
