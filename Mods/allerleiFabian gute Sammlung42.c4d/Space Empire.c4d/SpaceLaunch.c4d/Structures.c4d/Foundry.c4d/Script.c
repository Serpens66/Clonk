/*-- Hochofen --*/

#strict
#include BOB1
#include XMTR

local basement, manager;
local sign, furnace;


/* Fundament und Ofen */

protected Initialize:
  manager = CreateObject(BMAN,0,0,GetOwner());
  manager->Init(this(), 1);
  manager->InitLight(-56,-28);
  furnace = CreateObject(SFNC,-37,21,GetOwner());
  furnace->Init(this());
  sign = CreateObject(SFDS,-2,21,GetOwner());
  sign->Init(this(),furnace);
  return(1);

protected Construction:
  basement = CreateObject(BS19,0,+8,GetOwner());
  return(1);
  
protected Destruction:
  PreDestruction();
  if (basement) RemoveObject(basement);
  return(1);

public PreDestruction:
  if (manager) RemoveObject(manager);
  if (furnace) RemoveObject(furnace);
  if (sign)    RemoveObject(sign);
  return(inherited());


/* Sound, wenn etwas reingeworfen wird */

protected func Collection(obj) 
{ 
    if(GetID(obj)==WOOD || GetID(obj)==COAL) obj->~DoCon(-1);  // damit es beim automatischen buaen nicht rausgenommen wird
    Sound("Clonk");
    return(_inherited(obj));
}

func Ejection(obj) // wenn etwas die verlässt
{
    if(GetID(obj)==WOOD || GetID(obj)==COAL) obj->~DoCon(100);
    return(_inherited(obj));
}

/* Energie-/Metallerzeugung */

public DoSmoke:
  // Aufruf vom Schmelzofen-Objekt
  if (Random(2)) Smoke(-30,-29, Random(11)+8);
  if (Random(2)) Smoke(  0,-29, Random(11)+8);
  return(1);

public func Production(ID)
{
  // Aufruf vom Schmelzofen-Objekt
  Sound("Pshshsh");
  Exit(CreateContents(ID), -18,12,0, -1);
  if (!Random(10)) Exit(CreateContents(SHLK), -18,12,0, -1);
  if (ID==SHLK) {Exit(CreateContents(SHLK,0,9), -18,12,0, -1);Exit(CreateContents(ANML), -18,12,0, -1);};
  return(1);
}



/* Auffüllen per Leitung */

public func FillWithPipe(mat, value)
{
  if (mat == -1) return(0);
  if (mat != Material("Oil")) return(0);
  // versuchen, aufzufüllen
  return(furnace->DoFill(value));
}


/* Inhalt von Loren übernehmen */

private func DoorClosed()
{
  var num = -1;
  var obj;
  while (obj = Contents(++num))
    if (obj->~IsLorry())
    {
      GrabContents(obj);
      SetCommand(obj,"Exit");
    }
}


/* Steuerung von innen */

protected ContainedUp:
  [Anzeige|Image=SFNC]
  furnace->ShowBurnInfo();
  return(1);

protected ContainedDig:
  [Modus wechseln|Image=SFNC]
  furnace->SwitchMode();
  return(1);


/* Anzeigentext */

private func InfoBarText(barID, fill, maxf)
{
  if (barID == IBRE)
    return(Format("@Brennenergie:  %d", fill));
  if (barID == IBRP)
    return(Format("@Metall:  %d%", fill*100/maxf));
  return(0);
}


/* Energie */

public func TransmitEnergy()
{
  // nichts tun, wenn nicht fertig gebaut
  if (GetCon() < 100) return(0);
  // nur dann Energie versenden, wenn im Energiemodus
  if (!furnace->GetMode())
    inherited();
}

public func EnoughEnergy()
{
  if (furnace->EnoughEnergy()) return(1);
  if (GetEnergy() > 10)        return(1);
  return(0);
}

public IsEnergyConsumer:
  return(LocalN("mode",furnace));


/* Eigenschaften */

public InfoBarSize: return(4);
public GetMaxDamage: return(110);

