/*-- Hangar --*/

#strict

func Initialize()
{
  local pilot, vessel, predx, bay, pred1, pred2, pred3, sd;
  sd=0;
  bay=1;
  predx=0;
 // pred1=CreateObject(PRL_,0,0,GetOwner());
 // Enter(this(),pred1);
 // pred2=CreateObject(PRL_,0,0,GetOwner());
 // Enter(this(),pred2);
}

func Bezug(_vessel)
{
  vessel=_vessel;
  return(1);
}


func Besetzt()
{
  if(pilot=FindObject(0, 0,0,-1,-1, OCF_CrewMember(),0,0,this())) return(1);
  return(0);
}


func Explosion()
{
  pilot=FindObject(0, 0,0,-1,-1, OCF_CrewMember(),0,0,this()); 
  Exit(pilot);
  if(pred1) RemoveObject(pred1);
  if(pred2) RemoveObject(pred2);
  if(pred3) RemoveObject(pred3);
  RemoveObject();
  return(1);
}


//Nichts Aufnehmen
protected func RejectCollect()
{
  return(0);
}


//Steuerung


func ContainedDownDouble()
{
  pilot=FindObject(0, 0,0,-1,-1, OCF_CrewMember(),0,0,this());
  Enter(vessel,pilot);
  Sound("Airlock1");
  return(1);
}

func ContainedDig(pByObj)
{
   pilot=pByObj;
   CreateMenu(TCN_,Par(0),0,0,0,0);
   if(pred1)
   {
   if(GetDamage(pred1)==0) AddMenuItem("%s","Predbay1",T16I,Par(0)); 	//Bay1
   if(GetDamage(pred1)>0) AddMenuItem("%s","Predbay1",T16H,Par(0));
   }
   if(!pred1) 		   AddMenuItem("%s","Predbay1",T16G,Par(0));

   if(pred2)
   {
   if(GetDamage(pred2)==0) AddMenuItem("%s","Predbay2",T17I,Par(0)); 	//Bay2
   if(GetDamage(pred2)>0) AddMenuItem("%s","Predbay2",T17H,Par(0));
   }
   if(!pred2) 		   AddMenuItem("%s","Predbay2",T17G,Par(0));

   if(pred3)
   {
   if(GetDamage(pred3)==0) AddMenuItem("%s","Predbay3",T18I,Par(0)); 	//Bay3
   if(GetDamage(pred3)>0) AddMenuItem("%s","Predbay3",T18H,Par(0));
   }
   if(!pred3) 		   AddMenuItem("%s","Predbay3",T18G,Par(0));

   if(sd==0)AddMenuItem("%s","Startrichtung",T19G,Par(0));  	//Startrichtung
   if(sd!=0)AddMenuItem("%s","Startrichtung",T19H,Par(0));  	
   
   return(1);
}


func Asemblierung()
{
  if(bay==1) predx=pred1;
  if(bay==2) predx=pred2;
  if(bay==3) predx=pred3;
  if(predx)
  {
  Message("Andockstelle besetzt.",this());
  return(1);
  } 
  if(GetWealth(GetController(pilot)) < 250 &&  !ObjectCount(UNFI))
  {
  Message("Nicht genug Geld",this());
  return(0);
  }
  if(!ObjectCount(UNFI)) SetWealth( GetController(pilot), Sub(GetWealth(GetController(pilot)),250) );
  predx=CreateObject(PRL_,0,0,GetOwner(pilot));
  Enter(this(),predx);
  if(bay==1) pred1=predx;
  if(bay==2) pred2=predx;
  if(bay==3) pred3=predx;
  predx=0;
  Message("Predator Asembliert in Bay %d.",this(),bay);
  if(!Random(2)) Sound("PRReparatur1");
  if(!Random(1)) Sound("PRReparatur2");
  Sound("PRReparatur3");
  return(1);
}

func Startrichtung()
{
   if(sd==0)
   {
   sd=1;
   return(1);
   }
   sd=0;
   return(1);
}


func Predbay1()
{
    Message("Predatorbay 1 gewählt",this());
    bay=1;
    CreateMenu(A56B,pilot,0,0,0,0);
    AddMenuItem("%s","Statuskontrolle",T13G,pilot); 	//Anzeige Status
    AddMenuItem("%s","Starten",T12G,pilot);   		//Starten
    AddMenuItem("%s","Einladen",T14G,pilot);   		//Einladen
	AddMenuItem("%s","Brainstarten",T23H,pilot);   //Starten mit Brain an Bord
	AddMenuItem("%s","Asemblierung",T22H,pilot); //Asemblierung
    return(1);
}



func Predbay2()
{
    Message("Predatorbay 2 gewählt",this());
    bay=2;
    CreateMenu(A56B,pilot,0,0,0,0);
    AddMenuItem("%s","Statuskontrolle",T13G,pilot); 	//Anzeige Status
    AddMenuItem("%s","Starten",T12G,pilot);   		//Starten
    AddMenuItem("%s","Einladen",T14G,pilot);   		//Einladen
    AddMenuItem("%s","Brainstarten",T23H,pilot);   //Starten mit Brain an Bord
	AddMenuItem("%s","Asemblierung",T22H,pilot); //Asemblierung
    return(1);
}


func Predbay3()
{
    Message("Predatorbay 3 gewählt",this());
    bay=3;
    CreateMenu(A56B,pilot,0,0,0,0);
    AddMenuItem("%s","Statuskontrolle",T13G,pilot); 	//Anzeige Status
    AddMenuItem("%s","Starten",T12G,pilot);   		//Starten
    AddMenuItem("%s","Einladen",T14G,pilot);   		//Einladen
	AddMenuItem("%s","Brainstarten",T23H,pilot);   //Starten mit Brain an Bord
	AddMenuItem("%s","Asemblierung",T22H,pilot); //Asemblierung
    return(1);
}



func ContainedThrow()
{
  return(1);
}


func Statuskontrolle()
{
  if(bay==1) predx=pred1;
  if(bay==2) predx=pred2;
  if(bay==3) predx=pred3;
  if(!predx)
  {
  Message("Andockstelle frei.",this());
  return(1);
  }
  Message("Predator gedockt in Bay %d. Schaden:%d%.",this(),bay,GetDamage(predx));
  predx=0;
  return(1);
}

func Starten()
{
  if(bay==1) predx=pred1;
  if(bay==2) predx=pred2;
  if(bay==3) predx=pred3;
  if(!predx)
  {
  Message("Andockstelle leer.",this());
  return(1);
  }
  SetR(0,predx);
  Exit(predx,0,30);
  Enter(predx,pilot);
  if(sd==0) SetR(90,predx);
  if(sd==1) SetR(-90,predx);
  ObjectCall(predx,"Fahrgestell");
  ObjectSetAction(predx,"Fliegen",this());
  predx=0;
  if(bay==1) pred1=0;
  if(bay==2) pred2=0;
  if(bay==3) pred3=0;
  return(1);
}

func Brainstarten()
{
  if(bay==1) predx=pred1;
  if(bay==2) predx=pred2;
  if(bay==3) predx=pred3;
  if(!predx)
  {
  Message("Andockstelle leer.",this());
  return(1);
  }
  if(GetWealth(GetController(pilot)) < 50)
  {
  Message("Nicht genug Geld",this());
  return(0);
  }
  SetWealth( GetController(pilot), Sub(GetWealth(GetController(pilot)),50) );
  SetR(0,predx);
  Exit(predx,40-sd*80,30);
  if(sd==0) SetR(90,predx);
  if(sd==1) SetR(-90,predx);
  ObjectCall(predx,"Fahrgestell");
  SetOwner(GetOwner(),predx);
  CreateContents(BRA_,predx);
  predx=0;
  if(bay==1) pred1=0;
  if(bay==2) pred2=0;
  if(bay==3) pred3=0;
  return(1);
}

func Einladen()
{
  if(bay==1) predx=pred1;
  if(bay==2) predx=pred2;
  if(bay==3) predx=pred3;
  if(predx)
  {
  Message("Andockstelle besetzt.",this());
  return(1);
  } 
  if(predx=FindObject(PRL_,-50,-20,100,40,0,0,0,NoContainer()))
  {
  ObjectCall(predx,"Tucan",vessel);
  Enter(this(),predx);
  if(bay==1) pred1=predx;
  if(bay==2) pred2=predx;
  if(bay==3) pred3=predx;
  predx=0;
  Sound("Airlock1");
  Message("Predator gedockt in Bay %d.",this(),bay);
  return(1);
  }
  Message("Kein Predator gelandet.",this());
  return(1);
}


func Repair()
{
  if(!Random(100))
  {
  if(GetDamage(pred3)) predx=pred3;
  if(GetDamage(pred2)) predx=pred2;
  if(GetDamage(pred1)) predx=pred1;
  if(predx==0) return(1);
  DoDamage(-3,predx);
  if(!Random(2)) Sound("PRReparatur1");
  if(!Random(1)) Sound("PRReparatur2");
  Sound("PRReparatur3");
  predx=0;
  }
  return(1);
}


func ContainedUp()
{
    return(1);
}

func ContainedDown()
{
    return(1);
}


func ContainedThrow()
{
  return(1);
}


func ContainedLeft()
{
    return(1);
}


func ContainedRight()
{
    return(1);
}




















