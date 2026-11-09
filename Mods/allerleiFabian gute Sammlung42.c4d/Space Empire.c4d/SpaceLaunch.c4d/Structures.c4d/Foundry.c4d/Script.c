/*-- Hochofen --*/

#strict 3
#include BOB1
#include XMTR

local basement, manager;
local sign, furnace;


/* Fundament und Ofen */

protected func Initialize(...) {
  manager = CreateObject(BMAN,nil,nil,GetOwner());
  manager->Init(this(), 1);
  manager->InitLight(-56,-28);
  furnace = CreateObject(SFNC,-37,21,GetOwner());
  furnace->Init(this());
  sign = CreateObject(SFDS,-2,21,GetOwner());
  sign->Init(this(),furnace);
  return(1);
}

protected func Construction(...) {
  basement = CreateObject(BS19,nil,+8,GetOwner());
  return(1);
}
  
protected func Destruction(...) {
  PreDestruction();
  if (basement) RemoveObject(basement);
  return(1);
}

public func PreDestruction(...) {
  if (manager) RemoveObject(manager);
  if (furnace) RemoveObject(furnace);
  if (sign)    RemoveObject(sign);
  return(inherited());
}


/* Sound, wenn etwas reingeworfen wird */

protected func Collection(obj) 
{ 
    if(Or(Equal(GetID(obj), WOOD), Equal(GetID(obj), COAL))) obj->~DoCon(-1);  // damit es beim automatischen buaen nicht rausgenommen wird
    Sound("Clonk");
    return(_inherited(obj));
}

func Ejection(obj) // wenn etwas die verlässt
{
    if(Or(Equal(GetID(obj), WOOD), Equal(GetID(obj), COAL))) obj->~DoCon(100);
    return(_inherited(obj));
}

/* Energie-/Metallerzeugung */

public func DoSmoke(...) {
  // Aufruf vom Schmelzofen-Objekt
  if (Random(2)) Smoke(-30,-29, (Random(11) ?? 0)+8);
  if (Random(2)) Smoke(  nil,-29, (Random(11) ?? 0)+8);
  return(1);
}

public func Production(ID)
{
  // Aufruf vom Schmelzofen-Objekt
  Sound("Pshshsh");
  Exit(CreateContents(ID), -18,12,nil, -1);
  if (!Random(10)) Exit(CreateContents(SHLK), -18,12,nil, -1);
  if (Equal(ID, SHLK)) {Exit(CreateContents(SHLK,nil,9), -18,12,nil, -1);Exit(CreateContents(ANML), -18,12,nil, -1);};
  return(1);
}



/* Auffüllen per Leitung */

public func FillWithPipe(mat, value)
{
  if (Equal(mat, -1)) return(nil);
  if (!Equal(mat, Material("Oil"))) return(nil);
  // versuchen, aufzufüllen
  return(furnace->DoFill(value));
}


/* Inhalt von Loren übernehmen */

private func DoorClosed()
{
  var num = -1;
  var obj;
  while (obj = Contents(S42MigrationStep(num, 1)))
    if (obj->~IsLorry())
    {
      GrabContents(obj);
      SetCommand(obj,"Exit");
    }
}


/* Steuerung von innen */

protected func ContainedUp(...) {
  [Anzeige|Image=SFNC]
  furnace->ShowBurnInfo();
  return(1);
}

protected func ContainedDig(...) {
  [Modus wechseln|Image=SFNC]
  furnace->SwitchMode();
  return(1);
}


/* Anzeigentext */

private func InfoBarText(barID, fill, maxf)
{
  if (Equal(barID, IBRE))
    return(Format("@Brennenergie:  %d", fill));
  if (Equal(barID, IBRP))
    return(Format("@Metall:  %d%", (fill ?? 0)*100/(maxf ?? 0)));
  return(nil);
}


/* Energie */

public func TransmitEnergy()
{
  // nichts tun, wenn nicht fertig gebaut
  if ((GetCon() ?? 0) < 100) return(nil);
  // nur dann Energie versenden, wenn im Energiemodus
  if (!furnace->GetMode())
    inherited();
}

public func EnoughEnergy()
{
  if (furnace->EnoughEnergy()) return(1);
  if ((GetEnergy() ?? 0) > 10)        return(1);
  return(nil);
}

public func IsEnergyConsumer(...) {
  return(LocalN("mode",furnace));
}


/* Eigenschaften */

public func InfoBarSize(...) { return(4);
}
public func GetMaxDamage(...) { return(110);
}

