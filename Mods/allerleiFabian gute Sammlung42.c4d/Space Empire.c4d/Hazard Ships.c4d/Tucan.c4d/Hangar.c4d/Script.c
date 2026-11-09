/*-- Hangar --*/

#strict 3

func Initialize()
{
  local pilot, vessel, predx, bay, pred1, pred2, pred3, sd;
  sd=nil;
  bay=1;
  predx=nil;
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
  if(pilot=FindObject(nil, nil,nil,-1,-1, OCF_CrewMember,nil,nil,this())) return(1);
  return(nil);
}


func Explosion()
{
  pilot=FindObject(nil, nil,nil,-1,-1, OCF_CrewMember,nil,nil,this());
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
  return(nil);
}


//Steuerung


func ContainedDownDouble()
{
  pilot=FindObject(nil, nil,nil,-1,-1, OCF_CrewMember,nil,nil,this());
  Enter(vessel,pilot);
  Sound("Airlock1");
  return(1);
}

func ContainedDig(pByObj)
{
   pilot=pByObj;
   CreateMenu(TCN_,Par(nil),nil,nil,nil,nil);
   if(pred1)
   {
   if(Equal(GetDamage(pred1), nil)) AddMenuItem("%s","Predbay1",T16I,Par(nil)); 	//Bay1
   if((GetDamage(pred1) ?? 0)>0) AddMenuItem("%s","Predbay1",T16H,Par(nil));
   }
   if(!pred1) 		   AddMenuItem("%s","Predbay1",T16G,Par(nil));

   if(pred2)
   {
   if(Equal(GetDamage(pred2), nil)) AddMenuItem("%s","Predbay2",T17I,Par(nil)); 	//Bay2
   if((GetDamage(pred2) ?? 0)>0) AddMenuItem("%s","Predbay2",T17H,Par(nil));
   }
   if(!pred2) 		   AddMenuItem("%s","Predbay2",T17G,Par(nil));

   if(pred3)
   {
   if(Equal(GetDamage(pred3), nil)) AddMenuItem("%s","Predbay3",T18I,Par(nil)); 	//Bay3
   if((GetDamage(pred3) ?? 0)>0) AddMenuItem("%s","Predbay3",T18H,Par(nil));
   }
   if(!pred3) 		   AddMenuItem("%s","Predbay3",T18G,Par(nil));

   if(Equal(sd, nil))AddMenuItem("%s","Startrichtung",T19G,Par(nil));  	//Startrichtung
   if(!Equal(sd, nil))AddMenuItem("%s","Startrichtung",T19H,Par(nil));
   
   return(1);
}


func Asemblierung()
{
  if(Equal(bay, 1)) predx=pred1;
  if(Equal(bay, 2)) predx=pred2;
  if(Equal(bay, 3)) predx=pred3;
  if(predx)
  {
  Message("Andockstelle besetzt.",this());
  return(1);
  } 
  if(And((GetWealth(GetController(pilot)) ?? 0) < 250,  !ObjectCount(UNFI)))
  {
  Message("Nicht genug Geld",this());
  return(nil);
  }
  if(!ObjectCount(UNFI)) SetWealth( GetController(pilot), Sub(GetWealth(GetController(pilot)),250) );
  predx=CreateObject(PRL_,nil,nil,GetOwner(pilot));
  Enter(this(),predx);
  if(Equal(bay, 1)) pred1=predx;
  if(Equal(bay, 2)) pred2=predx;
  if(Equal(bay, 3)) pred3=predx;
  predx=nil;
  Message("Predator Asembliert in Bay %d.",this(),bay);
  if(!Random(2)) Sound("PRReparatur1");
  if(!Random(1)) Sound("PRReparatur2");
  Sound("PRReparatur3");
  return(1);
}

func Startrichtung()
{
   if(Equal(sd, nil))
   {
   sd=1;
   return(1);
   }
   sd=nil;
   return(1);
}


func Predbay1()
{
    Message("Predatorbay 1 gewählt",this());
    bay=1;
    CreateMenu(A56B,pilot,nil,nil,nil,nil);
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
    CreateMenu(A56B,pilot,nil,nil,nil,nil);
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
    CreateMenu(A56B,pilot,nil,nil,nil,nil);
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
  if(Equal(bay, 1)) predx=pred1;
  if(Equal(bay, 2)) predx=pred2;
  if(Equal(bay, 3)) predx=pred3;
  if(!predx)
  {
  Message("Andockstelle frei.",this());
  return(1);
  }
  Message("Predator gedockt in Bay %d. Schaden:%d%.",this(),bay,GetDamage(predx));
  predx=nil;
  return(1);
}

func Starten()
{
  if(Equal(bay, 1)) predx=pred1;
  if(Equal(bay, 2)) predx=pred2;
  if(Equal(bay, 3)) predx=pred3;
  if(!predx)
  {
  Message("Andockstelle leer.",this());
  return(1);
  }
  SetR(nil,predx);
  Exit(predx,nil,30);
  Enter(predx,pilot);
  if(Equal(sd, nil)) SetR(90,predx);
  if(Equal(sd, 1)) SetR(-90,predx);
  ObjectCall(predx,"Fahrgestell");
  ObjectSetAction(predx,"Fliegen",this());
  predx=nil;
  if(Equal(bay, 1)) pred1=nil;
  if(Equal(bay, 2)) pred2=nil;
  if(Equal(bay, 3)) pred3=nil;
  return(1);
}

func Brainstarten()
{
  if(Equal(bay, 1)) predx=pred1;
  if(Equal(bay, 2)) predx=pred2;
  if(Equal(bay, 3)) predx=pred3;
  if(!predx)
  {
  Message("Andockstelle leer.",this());
  return(1);
  }
  if((GetWealth(GetController(pilot)) ?? 0) < 50)
  {
  Message("Nicht genug Geld",this());
  return(nil);
  }
  SetWealth( GetController(pilot), Sub(GetWealth(GetController(pilot)),50) );
  SetR(nil,predx);
  Exit(predx,40-(sd ?? 0)*80,30);
  if(Equal(sd, nil)) SetR(90,predx);
  if(Equal(sd, 1)) SetR(-90,predx);
  ObjectCall(predx,"Fahrgestell");
  SetOwner(GetOwner(),predx);
  CreateContents(BRA_,predx);
  predx=nil;
  if(Equal(bay, 1)) pred1=nil;
  if(Equal(bay, 2)) pred2=nil;
  if(Equal(bay, 3)) pred3=nil;
  return(1);
}

func Einladen()
{
  if(Equal(bay, 1)) predx=pred1;
  if(Equal(bay, 2)) predx=pred2;
  if(Equal(bay, 3)) predx=pred3;
  if(predx)
  {
  Message("Andockstelle besetzt.",this());
  return(1);
  } 
  if(predx=FindObject(PRL_,-50,-20,100,40,nil,nil,nil,NoContainer()))
  {
  ObjectCall(predx,"Tucan",vessel);
  Enter(this(),predx);
  if(Equal(bay, 1)) pred1=predx;
  if(Equal(bay, 2)) pred2=predx;
  if(Equal(bay, 3)) pred3=predx;
  predx=nil;
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
  if(Equal(predx, nil)) return(1);
  DoDamage(-3,predx);
  if(!Random(2)) Sound("PRReparatur1");
  if(!Random(1)) Sound("PRReparatur2");
  Sound("PRReparatur3");
  predx=nil;
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




















