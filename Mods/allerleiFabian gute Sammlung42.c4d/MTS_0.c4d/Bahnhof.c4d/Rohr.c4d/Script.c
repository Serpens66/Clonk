/*-- Bahnhofsweiche --*/

#strict
#include BOB2
local rad, gleis1, gleis2, gleis3, gleis4,cont, leitx, leity, zgleis,
      zeit1, zeit2, zeit3, zeit4, power, manager, bahnhof, grad1,
      grad2, grad3, grad4, halt1, halt2, halt3, halt4,laden1, laden2, laden3,laden4,
      loeschen1, loeschen2, loeschen3, loeschen4,wechsel1, wechsel2, wechsel3,wechsel4,
      change1, change2, change3, change4,kugel;

func Initialize() 
{
  //Weiche erstellen
  rad = CreateObject(ST_2,0,0,GetOwner());
  ObjectSetAction(rad,"Attaching",this());
  SetR(90,rad);
  gleis1 = 90;
  gleis2 = 90;
  gleis3 = 90;
  gleis4 = 90;
  grad1 = Format("Leitwinkel %d°", gleis1);
  grad2 = Format("Leitwinkel %d°", gleis2);
  grad3 = Format("Leitwinkel %d°", gleis3);
  grad4 = Format("Leitwinkel %d°", gleis4);
  zeit1 = 3030;
  zeit2 = 3030;
  zeit3 = 3030;
  zeit4 = 3030;
  halt1 = (",|Unbegrenzter Aufenthalt");
  halt2 = (",|Unbegrenzter Aufenthalt");
  halt3 = (",|Unbegrenzter Aufenthalt");
  halt4 = (",|Unbegrenzter Aufenthalt");
  wechsel1 = 1;
  wechsel2 = 2;
  wechsel3 = 3;
  wechsel4 = 4;
  return(1);
}


func SetErzeuger(x) 
{
  bahnhof = x;
  return(1);
}

//WERTE IN WORTE
func GetAusdruck(x) 
{
  if(x == 1 ) return("wird beladen");
  if(x == 2 ) return("wird entladen");
  if(x == 0 ) return("wird nicht be/entladen");
}

//STRECKENSTATUS
func GetRichtung1()
{
  if (gleis1 > 180) gleis1 = (gleis1 - 180) * -1;
  if (gleis1 == 360) gleis1 = (0);
  loeschen1 = Format(",|%s",GetAusdruck(laden1));
  grad1 = Format("Leitwinkel %d°", gleis1);
  halt1 = Format(",|%d Frames Aufenthalt.", zeit1);
  if (zeit1 == 3030) halt1 = (",|Unbegrenzter Aufenthalt.");
  if (wechsel1 != 1) return(Message("<c fbfbfb>Strecke 1: Wechsel auf Strecke  %d.", bahnhof, wechsel1));
  Message("<c fbfbfb>Strecke 1:|%s%s%s%s",bahnhof,grad1,halt1,loeschen1,change1);
  
  return(1);
}
func GetRichtung2()
{
  if (gleis2 > 180) gleis2 = (gleis2 - 180) * -1;
  if (gleis2 == 360) gleis2 = (0);
  loeschen2 = Format(",|%s",GetAusdruck(laden2));
  grad2 = Format("Leitwinkel %d°", gleis2);
  halt2 = Format(",|%d Frames Aufenthalt.", zeit2);
  if (zeit2 == 3030) halt2 = (",|Unbegrenzter Aufenthalt.");
  if (wechsel2 != 2) return(Message("<c fbfbfb>Strecke 2: Wechsel auf Strecke  %d.", bahnhof, wechsel2));
  
  Message("<c fbfbfb>Strecke 2:|%s%s%s%s",bahnhof,grad2,halt2,loeschen2,change2);

  return(1);
}
func GetRichtung3()
{
  if (gleis3 > 180) gleis3 = (gleis3 - 180) * -1;
  if (gleis3 == 360) gleis3 = (0);
  loeschen3 = Format(",|%s",GetAusdruck(laden3));
  grad3 = Format("Leitwinkel %d°", gleis3);
  halt3 = Format(",|%d Frames Aufenthalt.", zeit3);
  if (zeit3 == 3030) halt3 = (",|Unbegrenzter Aufenthalt.");
  if (wechsel3 != 3) return(Message("<c fbfbfb>Strecke 3: Wechsel auf Strecke  %d.", bahnhof, wechsel3));
  
  Message("<c fbfbfb>Strecke 3:|%s%s%s%s",bahnhof,grad3,halt3,loeschen3,change3);
  
  return(1);
}

func GetRichtung4()
{
  if (gleis4 > 180) gleis4 = (gleis4 - 180) * -1;
  if (gleis4 == 360) gleis4 = (0);
  loeschen4 = Format(",|%s",GetAusdruck(laden4));
  grad4 = Format("Leitwinkel %d°", gleis4);
  halt4 = Format(",|%d Frames Aufenthalt.", zeit4);
  if (zeit4 == 3030) halt4 = (",|Unbegrenzter Aufenthalt.");
  if (wechsel4 != 4) return(Message("<c fbfbfb>Strecke 4: Wechsel auf Strecke  %d.", bahnhof, wechsel4));
  
  Message("<c fbfbfb>Strecke 4:|%s%s%s%s",bahnhof,grad4,halt4,loeschen4,change4);
  
  return(1);
}


//WINKELMENÜ (WEICHE WIRD AUF DIE ENTSPRECHENDE POSITION GEDREHT)
func SetStrecke() 
{
  var x;
  //Strecke auswählen
  if (Par(1) == 1) {SetR(gleis1,rad); x = grad1;}
  if (Par(1) == 2) {SetR(gleis2,rad); x = grad2;}
  if (Par(1) == 3) {SetR(gleis3,rad); x = grad3;}
  if (Par(1) == 4) {SetR(gleis4,rad); x = grad4;}
  zgleis = Par(1);
  return(Message("<c fbfbfb>%s",bahnhof,x));
}


//NORMALSTRECKE -
func Links() 
{
  if (zgleis == 1) gleis1 = -90;
  if (zgleis == 2) gleis2 = -90;
  if (zgleis == 3) gleis3 = -90;
  if (zgleis == 4) gleis4 = -90;
  
  SetR(-90,rad);
  
  Message("<c fbfbfb>Strecke %d: %d°",bahnhof,zgleis,GetR(rad));

  return(1);
}


//NORMALSTRECKE +
func Rechts() 
{
  if (zgleis == 1) gleis1 = 90;
  if (zgleis == 2) gleis2 = 90;
  if (zgleis == 3) gleis3 = 90;
  if (zgleis == 4) gleis4 = 90;
  
  SetR(90,rad);
  
  Message("<c fbfbfb>Strecke %d: %d°",bahnhof,zgleis,GetR(rad));

  return(1);
}

//ZEIT ERFASSEN
func GetZeit() 
{
  var x;
  if (Par(1) == 1) x = halt1;
  if (Par(1) == 2) x = halt2;
  if (Par(1) == 3) x = halt3;
  if (Par(1) == 4) x = halt4;
  
  Message("<c fbfbfb>Strecke %d: %s",bahnhof,Par(1),x);

  return(1);
}
//ZEIT Festlegen
func Zeit(x,y) 
{
  var z;
  
  if (x == 1)  {halt1 = Format(", %d Frames Aufenthalt.", y); zeit1 = y;}
  if (x == 2)  {halt2 = Format(", %d Frames Aufenthalt.", y); zeit2 = y;}
  if (x == 3)  {halt3 = Format(", %d Frames Aufenthalt.", y); zeit3 = y;}
  if (x == 4)  {halt4 = Format(", %d Frames Aufenthalt.", y); zeit4 = y;}
  
  if (x == 1 && y == 3030) halt1 = (", Unbegrenzter Aufenthalt.");
  if (x == 2 && y == 3030) halt2 = (", Unbegrenzter Aufenthalt.");
  if (x == 3 && y == 3030) halt3 = (", Unbegrenzter Aufenthalt.");
  if (x == 4 && y == 3030) halt4 = (", Unbegrenzter Aufenthalt.");
  
  if (x == 1) z = halt1;
  if (x == 2) z = halt2;
  if (x == 3) z = halt3;
  if (x == 4) z = halt4;
  
  Message("<c fbfbfb>Strecke %d: %s",bahnhof,x,z);

  return(1);
}

//SPHÄRE ANDOCKEN
func Andocken(pkugel,pnummer,ptor) 
  {
  var inhalt,stelle;
  stelle = 0;
  kugel = pkugel;
  
  //Genug Energie?
  if (power == 0) return(Message("Zu wenig Energie!",bahnhof),ObjectSetAction(pkugel,"Halt",this()), LocalN("blockiert",pkugel) = 70, LocalN("treibstoff",pkugel) = 0);
  //Wenn belegt zurückschicken
  if (FindObject(ST_3,-10,-10,20,20,0,"Bahnhof") || FindObject(ST_3,-10,-10,20,20,0,"Auf") || FindObject(ST_3,-10,-10,20,20,0,"Istauf")|| FindObject(ST_3,-10,-10,20,20,0,"Zu"))
   {
    Message("<c fbfbfb>Bahnhof belegt, Sphäre in Warteschleife",pkugel); 
    LocalN("treibstoff",pkugel) = 800;
    LocalN("blockiert",pkugel) = 35;
    LocalN("nexthalt",pkugel)=LocalN("lasthalt",pkugel);
    LocalN("lasthalt",pkugel)=this();
    return(LocalN("leitx",pkugel) = -LocalN("leitx",pkugel), LocalN("leity",pkugel) = -LocalN("leity",pkugel));
   }
  
  Sound("mts_halt");
  LocalN("treibstoff",pkugel) = 800; 
  if (pnummer == 0)
    return(Message("Magnetsphäre hat keine Strecke zugewiesen!",pkugel));   
  if (pnummer == 1)
   {
    if(wechsel1 != 1) return(ObjectCall(pkugel,"SetStrecke",0,wechsel1), SetAction("Schweben",pkugel),ObjectCall(this(),"Andocken",pkugel,LocalN("gleis",pkugel)));
    SetR(gleis1,rad);
    ObjectCall(pkugel,"SetHaltezeit",zeit1);
    
    if (laden1 == 1 && LocalN("ladbar",pkugel) == 1) Einladen();
    if (laden1 == 2 && LocalN("ladbar",pkugel) == 1) Ausladen();    
   }
   
  if (pnummer == 2 )
   {
    if(wechsel2 != 2) return(ObjectCall(pkugel,"SetStrecke",0,wechsel2), SetAction("Schweben",pkugel),ObjectCall(this(),"Andocken",pkugel,LocalN("gleis",pkugel)));
    SetR(gleis2,rad);
    ObjectCall(pkugel,"SetHaltezeit",zeit2);
    
    if (laden2 == 1 && LocalN("ladbar",pkugel) == 1) Einladen();
    if (laden2 == 2 && LocalN("ladbar",pkugel) == 1) Ausladen();
   }
   
  if (pnummer == 3)
   {
    if(wechsel3 != 3) return(ObjectCall(pkugel,"SetStrecke",0,wechsel3), SetAction("Schweben",pkugel),ObjectCall(this(),"Andocken",pkugel,LocalN("gleis",pkugel)));
    SetR(gleis3,rad);
    ObjectCall(pkugel,"SetHaltezeit",zeit3);
 
    if (laden3 == 1 && LocalN("ladbar",pkugel) == 1) Einladen();
    if (laden3 == 2 && LocalN("ladbar",pkugel) == 1) Ausladen();
   }
   
   if (pnummer == 4)
   {
    if(wechsel4 != 4) return(ObjectCall(pkugel,"SetStrecke",0,wechsel4), SetAction("Schweben",pkugel),ObjectCall(this(),"Andocken",pkugel,LocalN("gleis",pkugel)));
    SetR(gleis4,rad);
    ObjectCall(pkugel,"SetHaltezeit",zeit4);
 
    if (laden4 == 1 && LocalN("ladbar",pkugel) == 1) Einladen();
    if (laden4 == 2 && LocalN("ladbar",pkugel) == 1) Ausladen();
   }
    
  leitx = Sin(GetR(rad),2);
  leity = -Cos(GetR(rad),2);
  ObjectCall(pkugel,"Kurs",leitx,leity);
  ObjectSetAction(pkugel,"Bahnhof",this());
  LocalN("lasthalt",pkugel)=this();
  //ObjectCall(this(),"Andocken",pkugel,LocalN("gleis",pkugel));
  return(1);
  }
  
 //KUGEL BELADEN
 func Einladen()
   {
   var i0, i1, i2, i3, i4, i5, i6, i7, i8, i9, i10, i11, i12, i13, i14, i15, z,lauf;
   z=0;
   
   while (z <= 15)
     { 
      //Inhaltsindex an Variable übermitteln
      VarN(Format("i%d",z)) = Contents(z,bahnhof);

      //Wenn kein Clonk, dann ausladen
      if (GetID(VarN(Format("i%d",z))) != HZCK)
        { 
         if (GetID(VarN(Format("i%d",z))) != ST_4)
           {
            if (GetID(VarN(Format("i%d",z))) != ST_5)
              {
               if (GetID(VarN(Format("i%d",z))) != ST_2) 
                 {
                  if (GetID(VarN(Format("i%d",z))) != LORY)
                    { 
                     if (GetID(VarN(Format("i%d",z))) != ST_3) 
                       {
                        if (GetID(VarN(Format("i%d",z))) != SHTL) 
                          {
                           if (GetID(VarN(Format("i%d",z))) != SHT2) 
                             {
                              if (ContentsCount(0,kugel) < 16) ObjectCall(kugel,"Giver",VarN(Format("i%d",z)),this());
                             }
                          }
                       }
                    }
                 }  
              }
           }
        }
      z ++ ;  
     } 
     lauf ++;
     if(FindOtherContents(HZCK,bahnhof) && FindOtherContents(ST_4,bahnhof) 
        && FindOtherContents(ST_5,bahnhof) && FindOtherContents(ST_2,bahnhof) 
        && ContentsCount(0,kugel) < 16 && lauf < 16) return(Einladen());
     return(lauf = 0,Message("<c fbfbfb>Sphäre beladen.",bahnhof));
   }

//KUGEL ENTLADEN
 func Ausladen()
   {
   var i0, i1, i2, i3, i4, i5, i6, i7, i8, i9, i10, i11, i12, i13, i14, i15, z;
   z=0;
   
   while (z <= 15)
     { 
      //Inhaltsindex an Variable übermitteln
      VarN(Format("i%d",z)) = Contents(z,kugel);

      //Wenn kein Clonk, dann ausladen
      if (GetID(VarN(Format("i%d",z))) != HZCK) ObjectCall(bahnhof,"Grabber",VarN(Format("i%d",z)));
      z ++ ;  
     }  
     if(FindOtherContents(HZCK,kugel)) Ausladen();   
     return(Message("<c fbfbfb>Sphäre entladen.",bahnhof));
   }
 



