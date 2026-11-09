/*-- Bahnhofsweiche --*/

#strict 3
#include BOB2
local rad, gleis1, gleis2, gleis3, gleis4,cont, leitx, leity, zgleis,
      zeit1, zeit2, zeit3, zeit4, power, manager, bahnhof, grad1,
      grad2, grad3, grad4, halt1, halt2, halt3, halt4,laden1, laden2, laden3,laden4,
      loeschen1, loeschen2, loeschen3, loeschen4,wechsel1, wechsel2, wechsel3,wechsel4,
      change1, change2, change3, change4,kugel;

func Initialize() 
{
  //Weiche erstellen
  rad = CreateObject(ST_2,nil,nil,GetOwner());
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
  if(Equal(x, 1) ) return("wird beladen");
  if(Equal(x, 2) ) return("wird entladen");
  if(Equal(x, nil) ) return("wird nicht be/entladen");
}

//STRECKENSTATUS
func GetRichtung1()
{
  if ((gleis1 ?? 0) > 180) gleis1 = ((gleis1 ?? 0) - 180) * -1;
  if (Equal(gleis1, 360)) gleis1 = (nil);
  loeschen1 = Format(",|%s",GetAusdruck(laden1));
  grad1 = Format("Leitwinkel %d°", gleis1);
  halt1 = Format(",|%d Frames Aufenthalt.", zeit1);
  if (Equal(zeit1, 3030)) halt1 = (",|Unbegrenzter Aufenthalt.");
  if (!Equal(wechsel1, 1)) return(Message("<c fbfbfb>Strecke 1: Wechsel auf Strecke  %d.", bahnhof, wechsel1));
  Message("<c fbfbfb>Strecke 1:|%s%s%s%s",bahnhof,grad1,halt1,loeschen1,change1);
  
  return(1);
}
func GetRichtung2()
{
  if ((gleis2 ?? 0) > 180) gleis2 = ((gleis2 ?? 0) - 180) * -1;
  if (Equal(gleis2, 360)) gleis2 = (nil);
  loeschen2 = Format(",|%s",GetAusdruck(laden2));
  grad2 = Format("Leitwinkel %d°", gleis2);
  halt2 = Format(",|%d Frames Aufenthalt.", zeit2);
  if (Equal(zeit2, 3030)) halt2 = (",|Unbegrenzter Aufenthalt.");
  if (!Equal(wechsel2, 2)) return(Message("<c fbfbfb>Strecke 2: Wechsel auf Strecke  %d.", bahnhof, wechsel2));
  
  Message("<c fbfbfb>Strecke 2:|%s%s%s%s",bahnhof,grad2,halt2,loeschen2,change2);

  return(1);
}
func GetRichtung3()
{
  if ((gleis3 ?? 0) > 180) gleis3 = ((gleis3 ?? 0) - 180) * -1;
  if (Equal(gleis3, 360)) gleis3 = (nil);
  loeschen3 = Format(",|%s",GetAusdruck(laden3));
  grad3 = Format("Leitwinkel %d°", gleis3);
  halt3 = Format(",|%d Frames Aufenthalt.", zeit3);
  if (Equal(zeit3, 3030)) halt3 = (",|Unbegrenzter Aufenthalt.");
  if (!Equal(wechsel3, 3)) return(Message("<c fbfbfb>Strecke 3: Wechsel auf Strecke  %d.", bahnhof, wechsel3));
  
  Message("<c fbfbfb>Strecke 3:|%s%s%s%s",bahnhof,grad3,halt3,loeschen3,change3);
  
  return(1);
}

func GetRichtung4()
{
  if ((gleis4 ?? 0) > 180) gleis4 = ((gleis4 ?? 0) - 180) * -1;
  if (Equal(gleis4, 360)) gleis4 = (nil);
  loeschen4 = Format(",|%s",GetAusdruck(laden4));
  grad4 = Format("Leitwinkel %d°", gleis4);
  halt4 = Format(",|%d Frames Aufenthalt.", zeit4);
  if (Equal(zeit4, 3030)) halt4 = (",|Unbegrenzter Aufenthalt.");
  if (!Equal(wechsel4, 4)) return(Message("<c fbfbfb>Strecke 4: Wechsel auf Strecke  %d.", bahnhof, wechsel4));
  
  Message("<c fbfbfb>Strecke 4:|%s%s%s%s",bahnhof,grad4,halt4,loeschen4,change4);
  
  return(1);
}


//WINKELMENÜ (WEICHE WIRD AUF DIE ENTSPRECHENDE POSITION GEDREHT)
func SetStrecke() 
{
  var x;
  //Strecke auswählen
  if (Equal(Par(1), 1)) {SetR(gleis1,rad); x = grad1;}
  if (Equal(Par(1), 2)) {SetR(gleis2,rad); x = grad2;}
  if (Equal(Par(1), 3)) {SetR(gleis3,rad); x = grad3;}
  if (Equal(Par(1), 4)) {SetR(gleis4,rad); x = grad4;}
  zgleis = Par(1);
  return(Message("<c fbfbfb>%s",bahnhof,x));
}


//NORMALSTRECKE -
func Links() 
{
  if (Equal(zgleis, 1)) gleis1 = -90;
  if (Equal(zgleis, 2)) gleis2 = -90;
  if (Equal(zgleis, 3)) gleis3 = -90;
  if (Equal(zgleis, 4)) gleis4 = -90;
  
  SetR(-90,rad);
  
  Message("<c fbfbfb>Strecke %d: %d°",bahnhof,zgleis,GetR(rad));

  return(1);
}


//NORMALSTRECKE +
func Rechts() 
{
  if (Equal(zgleis, 1)) gleis1 = 90;
  if (Equal(zgleis, 2)) gleis2 = 90;
  if (Equal(zgleis, 3)) gleis3 = 90;
  if (Equal(zgleis, 4)) gleis4 = 90;
  
  SetR(90,rad);
  
  Message("<c fbfbfb>Strecke %d: %d°",bahnhof,zgleis,GetR(rad));

  return(1);
}

//ZEIT ERFASSEN
func GetZeit() 
{
  var x;
  if (Equal(Par(1), 1)) x = halt1;
  if (Equal(Par(1), 2)) x = halt2;
  if (Equal(Par(1), 3)) x = halt3;
  if (Equal(Par(1), 4)) x = halt4;
  
  Message("<c fbfbfb>Strecke %d: %s",bahnhof,Par(1),x);

  return(1);
}
//ZEIT Festlegen
func Zeit(x,y) 
{
  var z;
  
  if (Equal(x, 1))  {halt1 = Format(", %d Frames Aufenthalt.", y); zeit1 = y;}
  if (Equal(x, 2))  {halt2 = Format(", %d Frames Aufenthalt.", y); zeit2 = y;}
  if (Equal(x, 3))  {halt3 = Format(", %d Frames Aufenthalt.", y); zeit3 = y;}
  if (Equal(x, 4))  {halt4 = Format(", %d Frames Aufenthalt.", y); zeit4 = y;}
  
  if (And(Equal(x, 1), Equal(y, 3030))) halt1 = (", Unbegrenzter Aufenthalt.");
  if (And(Equal(x, 2), Equal(y, 3030))) halt2 = (", Unbegrenzter Aufenthalt.");
  if (And(Equal(x, 3), Equal(y, 3030))) halt3 = (", Unbegrenzter Aufenthalt.");
  if (And(Equal(x, 4), Equal(y, 3030))) halt4 = (", Unbegrenzter Aufenthalt.");
  
  if (Equal(x, 1)) z = halt1;
  if (Equal(x, 2)) z = halt2;
  if (Equal(x, 3)) z = halt3;
  if (Equal(x, 4)) z = halt4;
  
  Message("<c fbfbfb>Strecke %d: %s",bahnhof,x,z);

  return(1);
}

//SPHÄRE ANDOCKEN
func Andocken(pkugel,pnummer,ptor) 
  {
  var inhalt,stelle;
  stelle = nil;
  kugel = pkugel;
  
  //Genug Energie?
  if (Equal(power, nil)) return(S42MigrationFirst(Message("Zu wenig Energie!",bahnhof),ObjectSetAction(pkugel,"Halt",this()), LocalN("blockiert",pkugel) = 70, LocalN("treibstoff",pkugel) = nil));
  //Wenn belegt zurückschicken
  if (Or(Or(Or(FindObject(ST_3,-10,-10,20,20,nil,"Bahnhof"), FindObject(ST_3,-10,-10,20,20,nil,"Auf")), FindObject(ST_3,-10,-10,20,20,nil,"Istauf")), FindObject(ST_3,-10,-10,20,20,nil,"Zu")))
   {
    Message("<c fbfbfb>Bahnhof belegt, Sphäre in Warteschleife",pkugel); 
    LocalN("treibstoff",pkugel) = 800;
    LocalN("blockiert",pkugel) = 35;
    LocalN("nexthalt",pkugel)=LocalN("lasthalt",pkugel);
    LocalN("lasthalt",pkugel)=this();
    return(S42MigrationFirst(LocalN("leitx",pkugel) = -(LocalN("leitx",pkugel) ?? 0), LocalN("leity",pkugel) = -(LocalN("leity",pkugel) ?? 0)));
   }
  
  Sound("mts_halt");
  LocalN("treibstoff",pkugel) = 800; 
  if (Equal(pnummer, nil))
    return(Message("Magnetsphäre hat keine Strecke zugewiesen!",pkugel));   
  if (Equal(pnummer, 1))
   {
    if(!Equal(wechsel1, 1)) return(S42MigrationFirst(ObjectCall(pkugel,"SetStrecke",nil,wechsel1), SetAction("Schweben",pkugel),ObjectCall(this(),"Andocken",pkugel,LocalN("gleis",pkugel))));
    SetR(gleis1,rad);
    ObjectCall(pkugel,"SetHaltezeit",zeit1);
    
    if (And(Equal(laden1, 1), Equal(LocalN("ladbar",pkugel), 1))) Einladen();
    if (And(Equal(laden1, 2), Equal(LocalN("ladbar",pkugel), 1))) Ausladen();
   }
   
  if (Equal(pnummer, 2) )
   {
    if(!Equal(wechsel2, 2)) return(S42MigrationFirst(ObjectCall(pkugel,"SetStrecke",nil,wechsel2), SetAction("Schweben",pkugel),ObjectCall(this(),"Andocken",pkugel,LocalN("gleis",pkugel))));
    SetR(gleis2,rad);
    ObjectCall(pkugel,"SetHaltezeit",zeit2);
    
    if (And(Equal(laden2, 1), Equal(LocalN("ladbar",pkugel), 1))) Einladen();
    if (And(Equal(laden2, 2), Equal(LocalN("ladbar",pkugel), 1))) Ausladen();
   }
   
  if (Equal(pnummer, 3))
   {
    if(!Equal(wechsel3, 3)) return(S42MigrationFirst(ObjectCall(pkugel,"SetStrecke",nil,wechsel3), SetAction("Schweben",pkugel),ObjectCall(this(),"Andocken",pkugel,LocalN("gleis",pkugel))));
    SetR(gleis3,rad);
    ObjectCall(pkugel,"SetHaltezeit",zeit3);
 
    if (And(Equal(laden3, 1), Equal(LocalN("ladbar",pkugel), 1))) Einladen();
    if (And(Equal(laden3, 2), Equal(LocalN("ladbar",pkugel), 1))) Ausladen();
   }
   
   if (Equal(pnummer, 4))
   {
    if(!Equal(wechsel4, 4)) return(S42MigrationFirst(ObjectCall(pkugel,"SetStrecke",nil,wechsel4), SetAction("Schweben",pkugel),ObjectCall(this(),"Andocken",pkugel,LocalN("gleis",pkugel))));
    SetR(gleis4,rad);
    ObjectCall(pkugel,"SetHaltezeit",zeit4);
 
    if (And(Equal(laden4, 1), Equal(LocalN("ladbar",pkugel), 1))) Einladen();
    if (And(Equal(laden4, 2), Equal(LocalN("ladbar",pkugel), 1))) Ausladen();
   }
    
  leitx = Sin(GetR(rad),2);
  leity = -(Cos(GetR(rad),2) ?? 0);
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
   z=nil;
   
   while ((z ?? 0) <= 15)
     { 
      //Inhaltsindex an Variable übermitteln
      VarN(Format("i%d",z)) = Contents(z,bahnhof);

      //Wenn kein Clonk, dann ausladen
      if (!Equal(GetID(VarN(Format("i%d",z))), HZCK))
        { 
         if (!Equal(GetID(VarN(Format("i%d",z))), ST_4))
           {
            if (!Equal(GetID(VarN(Format("i%d",z))), ST_5))
              {
               if (!Equal(GetID(VarN(Format("i%d",z))), ST_2))
                 {
                  if (!Equal(GetID(VarN(Format("i%d",z))), LORY))
                    { 
                     if (!Equal(GetID(VarN(Format("i%d",z))), ST_3))
                       {
                        if (!Equal(GetID(VarN(Format("i%d",z))), SHTL))
                          {
                           if (!Equal(GetID(VarN(Format("i%d",z))), SHT2))
                             {
                              if ((ContentsCount(nil,kugel) ?? 0) < 16) ObjectCall(kugel,"Giver",VarN(Format("i%d",z)),this());
                             }
                          }
                       }
                    }
                 }  
              }
           }
        }
      S42MigrationStep(z , 1, true) ;
     } 
     S42MigrationStep(lauf , 1, true);
     if(And(And(And(And(And(FindOtherContents(HZCK,bahnhof), FindOtherContents(ST_4,bahnhof))
        , FindOtherContents(ST_5,bahnhof)), FindOtherContents(ST_2,bahnhof))
        , (ContentsCount(nil,kugel) ?? 0) < 16), (lauf ?? 0) < 16)) return(Einladen());
     return(S42MigrationFirst(lauf = nil,Message("<c fbfbfb>Sphäre beladen.",bahnhof)));
   }

//KUGEL ENTLADEN
 func Ausladen()
   {
   var i0, i1, i2, i3, i4, i5, i6, i7, i8, i9, i10, i11, i12, i13, i14, i15, z;
   z=nil;
   
   while ((z ?? 0) <= 15)
     { 
      //Inhaltsindex an Variable übermitteln
      VarN(Format("i%d",z)) = Contents(z,kugel);

      //Wenn kein Clonk, dann ausladen
      if (!Equal(GetID(VarN(Format("i%d",z))), HZCK)) ObjectCall(bahnhof,"Grabber",VarN(Format("i%d",z)));
      S42MigrationStep(z , 1, true) ;
     }  
     if(FindOtherContents(HZCK,kugel)) Ausladen();   
     return(Message("<c fbfbfb>Sphäre entladen.",bahnhof));
   }
 



