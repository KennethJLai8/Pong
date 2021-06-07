/*	Author: klai022
 *  Partner(s) Name: 
 *	Lab Section:
 *	Assignment: Lab #  Exercise #
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#include <stdio.h>
#include <stdlib.h>
#include "keypad.h"
#include "timer.h"
#include "scheduler.h"
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif







enum PaddleStates{PStart, PWait, Up, UpWait, Down, DownWait}PaddleState;//can't hold to move have to press
unsigned char PaddleShift[5] = {0xFE, 0xFD, 0xFB, 0xF7, 0xEF};//up to down
unsigned char TopPaddle = 2;
unsigned char BotPaddle = 3;





int PaddleSM(int state)
{
    unsigned char tmpA = ~PINA;
    switch(PaddleState)
    {
        case PStart:
            PaddleState = PWait;
            PORTC = 0x80;
            PORTD = PaddleShift[TopPaddle] & PaddleShift[BotPaddle];//position 3, 4
            break;
            
        case PWait:
            if(tmpA == 0x01)
            {
		    PORTC = 0x80;
                PORTD = PaddleShift[TopPaddle] & PaddleShift[BotPaddle];
                PaddleState = Up;
                break;
            }
            else if(tmpA == 0x02)
            {
		    PORTC = 0x80;
                PORTD = PaddleShift[TopPaddle] & PaddleShift[BotPaddle];
                PaddleState = Down;
                break;
            }
            else
            {   PORTC = 0x80;
                PORTD = PaddleShift[TopPaddle] & PaddleShift[BotPaddle];
                PaddleState = PWait;
                break;
            }

	    case Up:
                        /* PORTD = 0xFE;
                         PORTC = 0x02;
                         */

                        PORTD = PaddleShift[TopPaddle] & PaddleShift[BotPaddle];
			PORTC = 0x80;

                     
                         if(TopPaddle > 0 && tmpA == 0x01)
                         {       //i = 0;//reset i
                                 //BallState = TopOff;
                           //      PORTC = 0x00;
                           //      PORTD = 0xFF;
			    PORTD = PaddleShift[TopPaddle] & PaddleShift[BotPaddle];
                        PORTC = 0x80;

                                 TopPaddle--;
                                 BotPaddle--;
                                 PaddleState = UpWait;
                                 break;
                         }
			 else 
			 {
				 PaddleState = UpWait;
				 break;
			 }

                     
        case UpWait:
            if(tmpA == 0x01)
            {

		     PORTD = PaddleShift[TopPaddle] & PaddleShift[BotPaddle];
                        PORTC = 0x80;

                PaddleState = UpWait;
                break;
            }
            
            else
            {
		     PORTD = PaddleShift[TopPaddle] & PaddleShift[BotPaddle];
                        PORTC = 0x80;

                PaddleState = PWait;
                break;
            }
            
        case Down:
            PORTD = PaddleShift[TopPaddle] & PaddleShift[BotPaddle];
                        PORTC = 0x80;


                         if(BotPaddle < 4 && tmpA == 0x02)
                         {       //i = 0;//reset i
                                 //BallState = TopOff;
                           //      PORTC = 0x00;
                           //      PORTD = 0xFF;
                            PORTD = PaddleShift[TopPaddle] & PaddleShift[BotPaddle];
                        PORTC = 0x80;

                                 TopPaddle++;
                                 BotPaddle++;
                                 PaddleState = DownWait;
                                 break;
                         }
                         else
                         {
                                 PaddleState = DownWait;
                                 break;
                         }

        case DownWait:
           
            if(tmpA == 0x02)
            {
		    PORTD = PaddleShift[TopPaddle] & PaddleShift[BotPaddle];
                        PORTC = 0x80;

                PaddleState = DownWait;
                break;
            }
            else
            {
		    PORTD = PaddleShift[TopPaddle] & PaddleShift[BotPaddle];
                        PORTC = 0x80;

                PaddleState = PWait;
                break;
            }
    }
    return state;
}



//enum Multrows{0xFE, 0xFD, 0xFB, 0xF7, 0xEF}
unsigned char horizontalBall[8] = {0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01};//left to right
unsigned char vertBall[5] = {0xFE, 0xFD, 0xFB, 0xF7, 0xEF};//up to down
int i = 0;
unsigned char j = 0;//start at row 1
unsigned char k = 7;//start at column 2

enum BallStates{BStart, Top, TopOff, DiagLeftDown, DiagLeftUp, DiagRightUp, DiagRightDown} BallState;//using code from TimeMultSM
int BallSM(int state)
{
	switch(BallState)
	{
		case BStart:
			BallState = DiagLeftDown;
			break;

		case DiagLeftDown:
			/* PORTD = 0xFE;
                	 PORTC = 0x02;
			 */

			PORTD = vertBall[j];
			PORTC = horizontalBall[k];

			 if(i < 300)
			 {
				 BallState = DiagLeftDown;
					 i++;
					 break;
			 }
			 else if(j < 4 && k > 0)
			 {       i = 0;//reset i
				 //BallState = TopOff;
				 PORTC = 0x00;
				 PORTD = 0xFF;
				 j++;
				 k--;
				 BallState = DiagLeftDown;
				 break;
			 }

			 if(j == 4)
			 {
				 PORTC = 0x00;
                                 PORTD = 0xFF;
				 i = 0;

				 BallState = DiagLeftUp;
				 break;
			 }
			 else if(k == 0)
			 {
				 PORTC = 0x00;
                                 PORTD = 0xFF;
                                 i = 0;

                                 BallState = DiagRightDown;
                                 break;

			 }

		case DiagLeftUp:
			  PORTD = vertBall[j];
                        PORTC = horizontalBall[k];

			 if(i < 300)
			 {
				 BallState = DiagLeftUp;
				 i++;
				 break;
			 }
			 else if(k > 0 && j > 0)//remember array indexing
			 {
				 i = 0;//reset i
                                 //BallState = TopOff;
                                 PORTC = 0x00;
                                 PORTD = 0xFF;
                                 j--;
                                 k--;
                                 BallState = DiagLeftUp;
                                 break;

			 }
			 if(k == 0)
			 {
				 PORTC = 0x00;
                                 PORTD = 0xFF;
                                 i = 0;

                                 BallState = DiagRightUp;
                                 break;


			 }
			 else if(j == 0)
			 {
				 PORTC = 0x00;
                                 PORTD = 0xFF;
                                 i = 0;

                                 BallState = DiagLeftDown;
                                 break;

			 }

		   case DiagRightUp:
                          PORTD = vertBall[j];
                        PORTC = horizontalBall[k];

                         if(i < 300)
                         {
                                 BallState = DiagRightUp;
                                 i++;
                                 break;
                         }
                         else if(j > 0 && k < 7)//remember array indexing
                         {
                                 i = 0;//reset i
                                 //BallState = TopOff;
                                 PORTC = 0x00;
                                 PORTD = 0xFF;
                                 j--;
                                 k++;
                                 BallState = DiagRightUp;
                                 break;

                         }
                         if(j == 0)
                         {
                                 PORTC = 0x00;
                                 PORTD = 0xFF;
                                 i = 0;

                                 BallState = DiagRightDown;
                                 break;


                         }
			 else if(k == 7)
			 {
				 PORTC = 0x00;
                                 PORTD = 0xFF;
                                 i = 0;

                                 BallState = DiagLeftUp;
                                 break;

			 }

		case DiagRightDown:
                          PORTD = vertBall[j];
                        PORTC = horizontalBall[k];

                         if(i < 300)
                         {
                                 BallState = DiagRightDown;
                                 i++;
                                 break;
                         }
                         else if(k < 7 && j < 4)//remember array indexing
                         {
                                 i = 0;//reset i
                                 //BallState = TopOff;
                                 PORTC = 0x00;
                                 PORTD = 0xFF;
                                 j++;
                                 k++;
                                 BallState = DiagRightDown;
                                 break;

                         }

                         if(j == 4)
                         {
                                 PORTC = 0x00;
                                 PORTD = 0xFF;
                                 i = 0;

                                 BallState = DiagRightUp;
                                 break;


                         }
			 else if(k == 7)
			 {
				  PORTC = 0x00;
                                 PORTD = 0xFF;
                                 i = 0;

                                 BallState = DiagLeftDown;
                                 break;

			 }






	
	}

	return state;
}


enum RicochetStates{RStart, RWait, HitUpperPaddle, HitLowerPaddle}RicochetState;


RicochetSM(int state)
{
     switch(RicochetState)
     {  
	case RStart:
		RicochetState = RWait;
		break;

	case RWait:
		if((TopPaddle == (j + 1)) && k == 1)//diagdownleft hits the upper paddle
		{
			BallState = DiagRightDown;
			RicochetState = RWait;
			break;
		}
		else if((TopPaddle == (j - 1)) && k == 1)//diagupleft hits the upper paddle
                {
                        BallState = DiagRightDown;
                        RicochetState = RWait;
                        break;
                }
		else if((BotPaddle == (j + 1)) && k == 1)//diagdownleft hits the upper paddle
                {
                        BallState = DiagRightUp;
                        RicochetState = RWait;
                        break;
                }
		else if((BotPaddle == (j + 1)) && k == 1)//diagdownleft hits the upper paddle
                {
                        BallState = DiagRightUp;
                        RicochetState = RWait;
                        break;
                }





      }
}





enum MultRowsStates{MStart, Row1, Row2, Row3, Row4, Row5} MultRowsState;
unsigned char stayInState;
unsigned char incUntilNextState;


/*
int TimeMultSM(int state)//row by row
{
    switch(MultRowsState)
    {
        case MStart:
		
            MultRowsState = Row1;
            break;
            
        case Row1://rename to ball movement?

            //PORTD = PORTD << 1;
	    //PORTC = PORTC << 1;
	  //  PORTD = 0xFE;
            //    PORTC = 0x02;
	    
            MultRowsState = Row2;
            break;
            
        case Row2://paddle
            PORTD = 0xFD;
		PORTC = 0x80;
            MultRowsState = Row3;

            break;
            
        case Row3://paddle
            PORTD = 0xFB;
	    PORTC = 0x80;
            MultRowsState = Row4;
            break;
            
        case Row4://ball
            //PORTD = 0xF7;
	//    PORTD = 0xFD;
	//    PORTC = 0x04;
            MultRowsState = Row5;
            break;
            
        case Row5:
            //PORTD = 0xEF;
            MultRowsState = Row1;
            break;
            
    }
    return state;
}

*/






typedef struct _task{

	// Tasks should have members that include: state, period,

	//a measurement of elapsed time, and a function pointer.

	signed 	 char state; 		//Task's current state

	unsigned long period; 		//Task period

	unsigned long elapsedTime; 	//Time elapsed since last task tick

	int (*TickFct)(int); 		//Task tick function


} task;





int main(void)
{

    DDRA = 0x00; PORTA = 0xFF;
    DDRC = 0xFF; PORTC = 0x00;
	DDRD = 0xFF; PORTD = 0x00;
    
    static task task1, task2, task3, task4;
    task *tasks[] = {&task1, &task2, &task3};
    const unsigned short numTasks = sizeof(tasks)/sizeof(task*);
    
    const char start = -1;
    
    task1.state = BStart;
    task1.period = 1;//was 100
    task1.elapsedTime = task1.period;
    task1.TickFct = &BallSM;
    
    task2.state = RStart;
    task2.period = 1;
    task2.elapsedTime = task2.period;
    task2.TickFct = &RicochetSM;
    
    task3.state = PStart;
    task3.period = 2;
    task3.elapsedTime = task3.period;
    task3.TickFct = &PaddleSM;
    
   
    
    TimerSet(1);/*GCD*/
    TimerOn();
    
    unsigned short i;
    
    

    while(1)
    {

	for(i = 0; i < numTasks; i++)
        {
            if(tasks[i]->elapsedTime == tasks[i]->period)
            {
                tasks[i]->state = tasks[i]->TickFct(tasks[i]->state);
                tasks[i]->elapsedTime = 0;
            }
            tasks[i]->elapsedTime += 1; /*GCD*/
        }
        
        while(!TimerFlag);
        TimerFlag = 0;

    }
}
