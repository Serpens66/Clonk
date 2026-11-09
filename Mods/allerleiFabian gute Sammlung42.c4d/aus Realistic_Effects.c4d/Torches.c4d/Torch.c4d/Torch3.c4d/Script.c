/*-- Fackel --*/

#strict
local light, reicht,genug,mlight;

public func Initialize()
{
  Incincinerate();
  SetPosition(GetX(),GetY()-2); // ein kleines stück nach oben verschieben, damits nicht in erde feststeckt. 

 // NELI sorgt dafür, dass kein Holz fürs brennen benötigt wird
if (!FindObject(NELI))  CreateContents(WOOD)->~DoCon(-1);  // DoCon damit es nicht als Ziel fürs automatische Bauen usw verwendet wird
return(1);
}

func Collection(obj) // wenn etwas die fackel betritt
{
    obj->~DoCon(-1);  // damit es beim automatischen buaen nicht rausgenommen wird
    return(_inherited(obj));
}

func Ejection(obj) // wenn etwas die Fackel verlässt
{
    obj->~DoCon(100);
    return(_inherited(obj));
}

func Destruction()
{
    if(light)
      RemoveObject(light);
_inherited();
}


public func ControlDig()
{
  if(GetAction() S= "Burning") {if (!FindObject(NELI)) if(Random(3)) CreateContents(WOOD)->~DoCon(-1);return(Extinguishsh());} // erstelle ein neues holz, denn bei erneutem anzünden wird direkt eines verbraucht.
if(genug==1)  {Incincinerate();
  return(this());};
if(genug==0) {PlayerMessage(GetOwner(),"Nicht genug Feuerholz",this());return();}
}

public func ControlDigDouble()
{
  if(GetAction() S= "Burning") Extinguishsh();
  CreateObject(WOOD);
  RemoveObject();
  return(this() );
}

public func Incincinerate()
{
  if(GetAction() S= "Burning") return 0;
  SetAction("Burning");  BurnWood();
  light=CreateObject(LGFL,0,0,-1);
    if(light)
    {
        light->~Init(this(),0,-6,300,RGB(75,60,10),true);
    }
 SetPlrViewRange(FogViewRange(),0,1);
  return(1);
}

func FogViewRange() // wird vom Teamview objekt aufgerufen, um anderen nicht-feindlichen Spielern ebenfalls diesen sichtradius mithilfe eines hilfsobjekts zu geben
{
    if(GetAction() S= "Burning") return(170); // wenn es brennt
    else return(0);
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

public func FireEffect()
{
  Smoke(Random(3), Random(3), Random(8));
  if(!Random(2))
    CreateParticle("Fire" ,RandomX(-2,2),-6,RandomX(-1,1)+GetWind()/80,-6,50,RGB(255,255,255));
  else
    CreateParticle("Fire2",RandomX(-2,2),-6,RandomX(-1,1)+GetWind()/80,-6,50,RGB(255,255,255));
}


public func CheckContents()
{
 if(GetAction() S= "Burning")
  {if(InLiquid())
    Extinguishsh();};
 if (!FindObject(NELI))  // wenn energie verbrucht wird
 {
     if(GetActTime()>9000 && ContentsCount(WOOD)==0)
     { Extinguishsh();}

    if (ContentsCount(WOOD)==0 && reicht <=10 ) {PlayerMessage(GetOwner(),"das letzte Stück Holz wird verbrannt",this()); ++reicht ;};
    if (ContentsCount(WOOD)>=1) {reicht=0;};
    if (ContentsCount(WOOD)==0) genug=0;
    if (ContentsCount(WOOD)>=1) genug=1;
    if(GetActTime()>9000 && ContentsCount(WOOD)>=0) {BurnWood();SetAction("Idle");SetAction("Burning");}
 }
 else genug=1;

  if(GetAction() S= "Burning") 
  {  if (!mlight && LightNeeded)  TurnLightOn();
  if (mlight && !LightNeeded)  TurnLightOff();
  }
}  

private func BurnWood() 
{
  if (!FindObject(NELI))
  {
      var pWood;
      if(pWood=FindContents(WOOD)) RemoveObject(pWood);
      Sound("Inflame");
  }
}

// außer Holz nichts aufnehmen
protected func RejectCollect(def,pObj) { if(def!=WOOD) return(1); }



// nicht das hauptlicht, sondern ein zweites größeres Licht für die größere umgebung.
private func TurnLightOn()
{
  var iColor;
  iColor = RGBa(235,150,10,50);
  mlight = AddLight(1500, iColor, this());
  return(1);
}

private func TurnLightOff()
{
  if (mlight) RemoveObject(mlight);
  return(1);
}
