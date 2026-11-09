/*-- Fackel --*/

#strict 3
local light, reicht,genug,mlight;

public func Initialize()
{
  Incincinerate();
  SetPosition(GetX(),(GetY() ?? 0)-2); // ein kleines stück nach oben verschieben, damits nicht in erde feststeckt.

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
  if(!!SEqual(GetAction(), "Burning")) {if (!FindObject(NELI)) if(Random(3)) CreateContents(WOOD)->~DoCon(-1);return(Extinguishsh());} // erstelle ein neues holz, denn bei erneutem anzünden wird direkt eines verbraucht.
if(Equal(genug, 1))  {Incincinerate();
  return(this());};
if(Equal(genug, nil)) {PlayerMessage(GetOwner(),"Nicht genug Feuerholz",this());return;}
}

public func ControlDigDouble()
{
  if(!!SEqual(GetAction(), "Burning")) Extinguishsh();
  CreateObject(WOOD);
  RemoveObject();
  return(this() );
}

public func Incincinerate()
{
  if(!!SEqual(GetAction(), "Burning")) return nil;
  SetAction("Burning");  BurnWood();
  light=CreateObject(LGFL,nil,nil,-1);
    if(light)
    {
        light->~Init(this(),nil,-6,300,RGB(75,60,10),true);
    }
 SetPlrViewRange(FogViewRange(),nil,1);
  return(1);
}

func FogViewRange() // wird vom Teamview objekt aufgerufen, um anderen nicht-feindlichen Spielern ebenfalls diesen sichtradius mithilfe eines hilfsobjekts zu geben
{
    if(!!SEqual(GetAction(), "Burning")) return(170); // wenn es brennt
    else return(nil);
}

public func Extinguishsh()
{
  SetAction("Idle");
  Sound("Pshshsh");
    if(light)
      RemoveObject(light);
TurnLightOff();
 SetPlrViewRange(FogViewRange(),nil,1);
  return(1);
}

public func FireEffect()
{
  Smoke(Random(3), Random(3), Random(8));
  if(!Random(2))
    CreateParticle("Fire" ,RandomX(-2,2),-6,(RandomX(-1,1) ?? 0)+(GetWind() ?? 0)/80,-6,50,RGB(255,255,255));
  else
    CreateParticle("Fire2",RandomX(-2,2),-6,(RandomX(-1,1) ?? 0)+(GetWind() ?? 0)/80,-6,50,RGB(255,255,255));
}


public func CheckContents()
{
 if(!!SEqual(GetAction(), "Burning"))
  {if(InLiquid())
    Extinguishsh();};
 if (!FindObject(NELI))  // wenn energie verbrucht wird
 {
     if(And((GetActTime() ?? 0)>9000, Equal(ContentsCount(WOOD), nil)))
     { Extinguishsh();}

    if (And(Equal(ContentsCount(WOOD), nil), (reicht ?? 0) <=10) ) {PlayerMessage(GetOwner(),"das letzte Stück Holz wird verbrannt",this()); S42MigrationStep(reicht, 1) ;};
    if ((ContentsCount(WOOD) ?? 0)>=1) {reicht=nil;};
    if (Equal(ContentsCount(WOOD), nil)) genug=nil;
    if ((ContentsCount(WOOD) ?? 0)>=1) genug=1;
    if(And((GetActTime() ?? 0)>9000, (ContentsCount(WOOD) ?? 0)>=0)) {BurnWood();SetAction("Idle");SetAction("Burning");}
 }
 else genug=1;

  if(!!SEqual(GetAction(), "Burning"))
  {  if (And(!mlight, LightNeeded))  TurnLightOn();
  if (And(mlight, !LightNeeded))  TurnLightOff();
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
protected func RejectCollect(def,pObj) { if(!Equal(def, WOOD)) return(1); }



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
