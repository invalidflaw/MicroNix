#include "nixieInterrupt.h"


/* ******************************************************************** */
void nixieISR()
/* ******************************************************************** */
{
  static byte tubeIndex = 0;
  if (gnixieActive)
  {
    switch (tubeIndex)
    {     
      case 0:
        clearNixie(3); // shut off tube 4
        if(ghourZero || gleftHour != 0)
        {
          setNixie(0, gleftHour);
        }
        tubeIndex++;
        break;
      case 1:
        clearNixie(0);
        setNixie(1, grightHour);
        tubeIndex++;
        break;
      case 2:
        clearNixie(1);
        if(gminZero || gleftHour != 0)
        {
          setNixie(2, gleftMin);
        }
        tubeIndex++;
        break;
      case 3:
        clearNixie(2);
        setNixie(3, grightMin);
        tubeIndex++;
        break;
      default:
        tubeIndex = 0;
    }
    setNixiePeriod(tubeIndex, gperiods[tubeIndex]);
  }
  
}
// NIXIE CONTROL SUPPORT FUNCTIONS

// SN74141 : Truth Table
//D C B A #
//L,L,L,L 0
//L,L,L,H 1
//L,L,H,L 2
//L,L,H,H 3
//L,H,L,L 4
//L,H,L,H 5
//L,H,H,L 6
//L,H,H,H 7
//H,L,L,L 8
//H,L,L,H 9


/* ******************************************************************** */
void setNixie(byte tubeSelect, byte value)
/* ******************************************************************** */
{
  byte a,b,c,d;
  a = 0; b = 0; c = 0; d = 0;
  switch( value )
  {
    case 0: a=0;b=0;c=0;d=0;break;
    case 1: a=1;b=0;c=0;d=0;break;
    case 2: a=0;b=1;c=0;d=0;break;
    case 3: a=1;b=1;c=0;d=0;break;
    case 4: a=0;b=0;c=1;d=0;break;
    case 5: a=1;b=0;c=1;d=0;break;
    case 6: a=0;b=1;c=1;d=0;break;
    case 7: a=1;b=1;c=1;d=0;break;
    case 8: a=0;b=0;c=0;d=1;break;
    case 9: a=1;b=0;c=0;d=1;break;
    default: a=1;b=1;c=1;d=1;break;
  }
  
   // Write the number binary
   digitalWrite(tubePin_a, a);
   digitalWrite(tubePin_b, b);
   digitalWrite(tubePin_c, c);
   digitalWrite(tubePin_d, d);

   // Activate the correct anode
   if(value > 9)
   {
    digitalWrite(anodes[tubeSelect], LOW);
   }
   else
   {
    digitalWrite(anodes[tubeSelect], HIGH);
   }
   
}

void setNixiePeriod(byte tubeSelect, bool state)
{
  digitalWrite(periodPin, state);
  digitalWrite(anodes[tubeSelect], HIGH);
}

void clearNixie(byte tubeSelect)
{
  digitalWrite(tubePin_a, LOW);
  digitalWrite(tubePin_b, LOW);
  digitalWrite(tubePin_c, LOW);
  digitalWrite(tubePin_d, LOW);
  digitalWrite(periodPin, LOW);
  digitalWrite(anodes[tubeSelect], LOW);
}
