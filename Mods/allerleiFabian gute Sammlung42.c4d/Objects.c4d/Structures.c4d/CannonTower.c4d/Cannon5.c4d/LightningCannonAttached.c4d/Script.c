/*--- Blitzgeschütz (angeschraubt) ---*/

#strict 3

local energy;

#include CTW2

protected func RotationSpeed() { return(2); }
protected func CannonMobileID() { return(ACT3); }

private func EnergyPerShot() { return(350); } // Energiebedarf für einen Schuss

/* Turm weg */

public func ComEnter(object pObj)
{
  // Batterien
  if(Equal(GetID(pObj), ADM3)) return(ConvertBattery(pObj) );
  // Kristalle
  if(Equal(GetID(pObj), CRYS))
  {
    Sound("Electric");
    Enter(this(), pObj);
  }
}

private func EnergyTransfer()
{
  if((GetEnergy(GetActionTarget()) ?? 0) > 9) DrainEnergy(GetActionTarget(), 10);
}

private func DrainEnergy(object pTower, int iLevel)
{
  // Energie vom Turm abziehen
  iLevel ??= 0;
  DoEnergy(-(iLevel ?? 0), pTower);
  S42MigrationAssign(energy, "+=", (iLevel ?? 0));
  if((energy ?? 0) > (EnergyPerShot() ?? 0) )
    ConvertEnergy();
}

private func ConvertEnergy()
{
  Sound("LightningLoad");
  CreateContents(CRYS);
  S42MigrationAssign(energy, "-=", (EnergyPerShot() ?? 0));
}
   
private func ConvertBattery(object pBattery)
{
  // Sounds
  Sound("Connect");
  Sound("LightningLoad");
  for(var i = nil; (i ?? 0) < 2; S42MigrationStep( i, 1))
    CreateContents(CRYS);
  if(Equal(GetEnergy(pBattery), 100)) // wenn vol augeladen, noch einen extra Crystall
    CreateContents(CRYS);
  RemoveObject(pBattery);
  // while(GetEnergy(pBattery)>=33) // zu einfach wieder nachzuladen, wenn Batterie nicht verbraucht wird, daher Ladung nun egal und Batterie weiterhin verbrauchen
  // {
      // CreateContents(CRYS);
      // DoEnergy(-33,pBattery); // 3 Aufladungen aus einer aufgeladenen Batterie
  // }
    
}

/* Blitzumwandlung */

public func LightningStrike()
{
  // Nur wenn fertig gebaut
  if((GetCon() ?? 0) < 100) return(nil);
  // Energiegewinnung
  CreateContents(CRYS);
  // Sound
  Sound("Electric");
}

public func ComFire(object pClonk)
{
  // SetRDir(0)
  SetRDir(nil);
  SetAction("Attaching",GetActionTarget());
  
  // Pruefen, ob genug Kristalle zur Verfuegung stehen
  var power;
  var i = nil, obj;
  
  while(obj = Contents(S42MigrationStep(i , 1, true)) )
    if(Equal(GetID(obj), CRYS))
    {
      S42MigrationAssign(power, "+=", ((GetCon(obj) ?? 0) - 50));
      if((power ?? 0) >= 50)
        break;
    }
  if((power ?? 0) < 50) return(Sound("Error") );
  
  // Kristalle verbrauchen. Beim Pruefen konnten die Kristalle
  // nicht gleichzeitig verbraucht werden, da, wenn die Energie
  // nicht ausreicht, eventuell bereits Kristalle geloescht worden waeren.
  power = nil;
  // Von oben nach unten zaehlen, da Objekte geloescht werden.
  i = ContentsCount();
  while(obj = Contents(S42MigrationStep( i, -1)) )
    if(Equal(GetID(obj), CRYS))
    {
      // Kraft des Kristalls zur Gesamtkraft hinzufeugen
      S42MigrationAssign(power, "+=", ((GetCon(obj) ?? 0) - 50));
      // Mehr als noetig?
      if((power ?? 0) > 50)
      {
        // Nur das was noetig ist dem Kristall abziehen
        var con = ((GetCon(obj) ) ?? 0) - (power ?? 0);
        DoCon(-(con ?? 0), obj);
      }
      else
        // Reicht noch nicht? Kristall ganz verbrauchen.
        RemoveObject(obj);
        
      // Genug power? Ok.
      if((power ?? 0) >= 50)
        break;
    }

  Sound("LightningCannon");
  
  var lgt1=CreateObject(ADM4, nil, nil, -1);
  lgt1->SetController(pClonk->GetController());
  lgt1->ADM4::Activate(GetX(), GetY(), Sin(GetR(), 18), 4, -(Cos(GetR(), 18) ?? 0), 4);
  var lgt2=CreateObject(ADM4, nil, nil, -1);
  lgt2->SetController(pClonk->GetController());
  lgt2->ADM4::Activate((GetX() ?? 0) + (Sin((GetR() ?? 0) + 90, 5) ?? 0), (GetY() ?? 0) + (Cos((GetR() ?? 0) + 90, 5) ?? 0), Sin(GetR(), 18), 4, -(Cos(GetR(), 18) ?? 0), 4);
  
  CastParticles("Spark", (Random(8) ?? 0) + 4, Random(35), Sin(GetR(), 18), -(Cos(GetR(), 18) ?? 0) );
}

