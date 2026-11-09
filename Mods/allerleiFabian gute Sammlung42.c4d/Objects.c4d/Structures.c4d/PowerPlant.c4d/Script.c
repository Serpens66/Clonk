/*-- Kraftwerk --*/

#strict 3

#include DOOR
#include BAS4

local pLight;


public func IsWaste(){ return(20);}

/* Initialisierung */

protected func Initialize()
{
  // Netterweise gibt's zu Beginn einmal Kohle
  CreateContents(COAL);
}

/* Eingangssteuerung */

private func SoundOpenDoor()
{
  Sound("GateOpen");
}

private func SoundCloseDoor()
{
  Sound("GateClose");
}
  
func Collection(obj) // wenn etwas betritt
{
    if(Or(Equal(GetID(obj), WOOD), Equal(GetID(obj), COAL))) obj->~DoCon(-1);  // damit es beim automatischen buaen nicht rausgenommen wird
    Sound("Clonk");
    return(_inherited(obj));
}

func Ejection(obj) // wenn etwas verlässt
{
    if(Or(Equal(GetID(obj), WOOD), Equal(GetID(obj), COAL))) obj->~DoCon(100);
    return(_inherited(obj));
}

/* Produktion */

private func Burning()
{
  if(!pLight) {pLight=CreateObject(LGFL,nil,nil,-1);pLight->~Init(this,nil,-6,300,RGB(75,60,10),true);}  // Licht, wenn burning
  // Rauch
  Smoke(-15,-20,12);
  Smoke(-1,-23,8);
  // Energieerzeugung
  DoEnergy(+50);
  // Weiter
  if((GetActTime() ?? 0) < (GetBurnTime() ?? 0)) return(1);
  // Fertig
  SetAction("Idle");
}

/* Inhaltsüberprüfung */

private func ContentsCheck()
{
  if(FindObject(ENNN))
      Message(Format("%v",GetEnergy()),this());
  // Noch aktiv
  if(!ActIdle()) return(1);
  if(pLight) {RemoveObject(pLight);}  // Licht weg, wenn idle
  var i = -1, obj, def;
  while(obj = Contents(S42MigrationStep(i, 1)) )
  {
    if(obj->~IsLorry() )
    {
      SetCommand(obj, "Exit");
    }
  }

  // Erstes Inhaltsobjekt prüfen
  obj = Contents(nil);
  def = GetID(obj);
  
  // Alles außer Brennmaterial raus
  if(And(And(And(!Equal(def, COAL), !Equal(def, OBRL)), !Equal(def, WOOD)), !Equal(def, LB9I)))
    SetCommand(obj, "Exit");
    
  // Wird Energie benötigt
  if(!CheckEnergyNeedChain() ) return(1);
  
  // Holz?
  if((ContentsCount(WOOD) ?? 0) > 1)
    return(BurnWood() );
    
  // Öl?
  if(ContentsCount(OBRL) )
    return(BurnOil() );
    
  // Kohle
  if(!(obj = FindContents(COAL)) ) return(1);
  RemoveObject(obj);
if(!Random(2))
{
var verbrannt;
verbrannt= CreateObject(VBWS);
verbrannt->Verbrannt(GetID(),IsWaste());
};
  SetAction("Burning");
}

private func BurnWood()
{
  // Zwei Holz löschen
  for(var i = nil; (i ?? 0) < 2; S42MigrationStep( i, 1))
    RemoveObject(FindContents(WOOD) );

if(!Random(2))
{
var verbrannt;
verbrannt= CreateObject(VBWS);
verbrannt->Verbrannt(GetID(),IsWaste());
};
  // Los gehts
  SetAction("Burning");
}

private func BurnOil()
{
  // Fass entleeren
  var barrel = FindContents(OBRL);
  if(!barrel) return(nil);
  ChangeDef(BARL, barrel);
  
  // Los geth.
if(!Random(2))
{
var verbrannt;
verbrannt= CreateObject(VBWS);
verbrannt->Verbrannt(GetID(),IsWaste());
};
  SetAction("Burning");
}

/* Brenndauer */
private func GetBurnTime() { return(2000); }
