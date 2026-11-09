/*-- Brennende Tonne :D --*/

#strict

local light;
local mlight, mlight2, reicht,genug,brennstoff;  //"reicht" nur für anzeigedauer der nachricht

public func BrennLicht() {
  if (!mlight) return(1); 
  if(Local(2) == 2) { Local(1)++; Local(1)++; Local(1)++;  }
    else { Local(1)--; Local(1)--; Local(1)--; Local(1)--; Local(1)--;  Local(1)--; }
  mlight -> ChangeSize(200 + Local(1));
  if(Local(1) > 15) Local(2) = 1;
  if(Local(1) < -5) Local(2) =2;
  return(1); }
  

func Initialize()
{
}


protected func Completion()  
{
  SetCategory(C4D_Vehicle()); // vehicle reicht, ohne foreground und ist dennoch noch abreißbar wegen construction

  Incincinerate();
  if (!FindObject(NELI))
  {
      CreateContents(WOOD)->~DoCon(-1);
      CreateContents(WOOD)->~DoCon(-1);
      CreateContents(WOOD)->~DoCon(-1);
  }
  Schedule("BrennLicht()", 2,100000000000000);
  Local(2) = 2;
  return(1);
}

func Collection(obj) // wenn etwas betritt
{
    obj->~DoCon(-1);  // damit es beim automatischen buaen nicht rausgenommen wird
    return(_inherited(obj));
}

func Ejection(obj) // wenn etwas die verlässt
{
    obj->~DoCon(100);
    return(_inherited(obj));
}

private func Smoking() 
{
  Smoke(Random(3), Random(3), Random(5) + 3);
  if(!Random(2))
    CreateParticle("Fire" ,RandomX(-2,2),-15,RandomX(-1,1)+GetWind()/80,-6,50,RGB(255,255,255));
  else
    CreateParticle("Fire2",RandomX(-2,2),-15,RandomX(-1,1)+GetWind()/80,-6,50,RGB(255,255,255));
}


public func ControlDig()
{
  if(GetAction() S= "Burn") {if (!FindObject(NELI)) if(Random(3)) CreateContents(WOOD)->~DoCon(-1);return(Extinguishsh());} // erstelle ein neues holz, denn bei erneutem anzünden wird direkt eines verbraucht.
if(genug==1)  {Incincinerate();
  return(this());};
if(genug==0) {PlayerMessage(GetOwner(),"Nicht genug Feuerholz",this());return();}
}


public func Incincinerate()
{
  if(GetAction() S= "Burn") return 0;
 
  light=CreateObject(LGFL,0,0,-1);   // Lichtschein aus RdW
    if(light)
      light->~Init(this(),0,-6,500,RGB(75,60,10),true);

    SetAction("Burn"); 
    SetPlrViewRange(FogViewRange(),0,1);    
    if(FindContents(WOOD)){
    BurnWood();return(1);}
    if(FindContents(COAL))
    BurnCoal();
      return(1);
}

func FogViewRange() // wird vom Teamview objekt aufgerufen, um anderen nicht-feindlichen Spielern ebenfalls diesen sichtradius mithilfe eines hilfsobjekts zu geben
{
    if(GetAction() S= "Burn") return(200); // wenn es brennt
    else return(0);
}

func Destruction()
{
    if(light)
      RemoveObject(light);
return(_inherited());
}


protected func CheckContents() 
{
if (!(GetOCF(this()) & OCF_Construct))  // nur wenn fertig gebaut
 {

if(GetAction() S= "Burn")
  {if(InLiquid())
    Extinguishsh();};

 if (!FindObject(NELI))  // wenn energie verbrucht wird
  {
     if (ContentsCount(WOOD)==0 && ContentsCount(COAL)==0 && reicht <=10 ) {PlayerMessage(GetOwner(),"Das letzte Stueck wird verbrannt",this()); ++reicht ;};
    if (ContentsCount(WOOD)>=1 || ContentsCount(COAL)>=1) {reicht=0;};

    if(brennstoff==WOOD)
      if(GetActTime()>12000 && ContentsCount(WOOD)==0 && ContentsCount(COAL)==0)
          Extinguishsh();
       // { SetAction("Idle");
      // Sound("Pshshsh");
        // if(light)
          // RemoveObject(light);TurnLightOff();}

    if(brennstoff==COAL)
      if(GetActTime()>20000 && ContentsCount(COAL)==0 && ContentsCount(WOOD)==0)
          Extinguishsh();
       // { SetAction("Idle");
      // Sound("Pshshsh");
        // if(light)
          // RemoveObject(light);TurnLightOff();}

      if (ContentsCount(WOOD)==0 && ContentsCount(COAL)==0) genug=0;
      if (ContentsCount(WOOD)>=1 || ContentsCount(COAL)>=1) genug=1;
   }
 else genug=1;

  if (mlight && !LightNeeded)  TurnLightOff();
  if(GetAction() S= "Burn") {  if (!mlight && LightNeeded)  TurnLightOn();}
  
 if (!FindObject(NELI))  // wenn energie verbrucht wird
  {
        if(brennstoff==WOOD)
    if(GetActTime()>12000 && ContentsCount(WOOD)>=0) {BurnWood();SetAction("Idle");SetAction("Burn"); return();} // -> erst alles Holz verbrennen, dann die Kohle
    
    if(brennstoff==COAL)
    if(GetActTime()>20000 && ContentsCount(COAL)>=0) {BurnCoal();SetAction("Idle");SetAction("Burn"); return();}
  }
 }
}





public func Extinguishsh()
{
  SetAction("Idle");
  Sound("Pshshsh");
    if(light)
      RemoveObject(light);
TurnLightOff();
SetPlrViewRange(FogViewRange(),0,1);
  return(1);
}

private func BurnCoal()
{
  if (!FindObject(NELI))  // wenn energie verbrucht wird
  {
      var pWood;
      if(pWood=FindContents(COAL)) RemoveObject(pWood);
      Sound("Inflame");
      SetAction("Burn");
      brennstoff=COAL;
  }  
}


private func BurnWood() 
{
  if (!FindObject(NELI))  // wenn energie verbrucht wird
  {
      var pWood;
      if(pWood=FindContents(WOOD)) RemoveObject(pWood);
      Sound("Inflame");
      SetAction("Burn");
      brennstoff=WOOD;
  }  
}


protected func RejectCollect(def,pObj) { if(def!=WOOD && def!=COAL) return(1); }// außer Holz nichts aufnehmen

// private func CheckNight()
// {
 // if (!mlight && LightNeeded)  {TurnLightOn();}
 // if (mlight && !LightNeeded)  {TurnLightOff();}
 // if (!mlight2 && !LightNeeded)  {TurnLightOn2();}
 // if (mlight2 && LightNeeded)  {TurnLightOff2();}
// }


private func TurnLightOn()
{
  var iColor;
  iColor = RGBa(235,150,10,50);
  mlight = AddLight(2200, iColor, this());
  return(1);
}

private func TurnLightOn2()
{
  var iColor;
  iColor = RGBa(235,150,10,100);
  mlight2 = AddLight(700, iColor, this());
  return(1);
}


private func TurnLightOff2()
{
  if (mlight2) RemoveObject(mlight2);
  return(1);
}


private func TurnLightOff()
{
  if (mlight) RemoveObject(mlight);
  return(1);
}
  