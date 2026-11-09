/* Magier */

#strict 3

#include MCLK

/* Nach dem Zaubern... */  
  
protected func AfterMagic()
{
  // Callback nach Magie-Animation: Nach weiteren Kristallen suchen
  CheckContents();
  // Ursprungs-Funktion aufrufen, wenn vorhanden
  return(_inherited() );
}

protected func AfterMagicJump()
{
  // Geth nicht mit inherited weil sonst nicht obiges AfterMagic aufgerufen wird
  if(And(idCurSpell, !!SEqual(GetAction(), "Walk")))
  {
    SetAction("Magic");
    SetPhase(iCurPhase);
  }
  else AfterMagic();
}

/* Energiegewinnung */

// Dummies
public func GetManaGain() {}
public func IsMagicCrystal() {}


// Dummies
public func GetManaGain() {}
public func IsMagicCrystal() {}

private func ConvertCrystal(object pObj)
  {
  if (!IsConvertReady()) return;
  if (!pObj->~Apply(this()))
    {
    var iManaGain = pObj->~GetManaGain(this());
    if (!iManaGain) iManaGain = 10;
    if (!DoMagicEnergy(iManaGain)) return;
    RemoveObject(pObj);
    }
  SetComDir(COMD_Stop);
  SetMagicAction();
  Sound("Electric");
  PlayerMessage(GetOwner(), "", this());
  }

protected func CheckContents()
  {
  // Walk-Startcall vom MagiClonk
  ReAim();
  // Alle Kristalle konvertieren
  var i=ContentsCount(), o;
  while (S42MigrationStep(i, -1, true)) if (o=Contents(i)) if (Or(Equal(GetID(o), CRYS), o->~IsMagicCrystal(this())))
    ConvertCrystal(o);
  }
  
/* Aufnahme */

protected func Collection2(object pObj)
  {
  return(CheckContents());
  }


/* Während des Reitens... */

private func Riding()
{
  CheckContents();
  return(_inherited());
}

/* Vom Blitz getroffen */

public func LightningStrike()
{
  // Wenn der Clonk tot ist, zerstört ihn der erste Blitz
  if (!!SEqual(GetAction(), "Dead"))
    {
    CastObjects(SPRK,5,20);
    CreateParticle("MSpark", nil,nil, 1000, RGBa(50,50,255,150));
    if (!GetAlive()) RemoveObject();
    return(true);
  }
  // Clonk soll anhalten
  SetComDir(COMD_Stop);
  // Energiegewinnung
  for(var i = nil; (i ?? 0) < 10; S42MigrationStep( i, 1)) DoMagicEnergy(1);
  // Sound
  Sound("Electric");
  // Effekt-Aktion (wenn's gerade passt)
  if (IsConvertReady())
    SetMagicAction();
  // Fertig
  return(1);
}

public func IsConvertReady()
{
  // Im Laufen und Reiten können Kristalle umgewandelt werden
  return(Or(Or(!!SEqual(GetAction(), "Walk"), !!SEqual(GetAction(), "Ride")), !!SEqual(GetAction(), "RideStill")));
}  


/* Zur Sicherheit, damit Überladungen keinen Fehler beim Starten geben */

protected func Scaling()
  {
  var szDesiredAction;
  if ((GetYDir() ?? 0)>0) szDesiredAction = "ScaleDown"; else szDesiredAction = "Scale";
  if (!SEqual(GetAction(), szDesiredAction)) SetAction(szDesiredAction);
  return(1);   
  }

/* Verhindert das Feststecken */

protected func CheckStuck()
{                   
  // Verhindert Festhängen am Mittelvertex
  if(!GetXDir()) if((Abs(GetYDir()) ?? 0) < 5)
    if(GBackSolid(nil, 3))
      SetPosition(GetX(), (GetY() ?? 0) + 1);
}


/* Status */

public func IsSorcerer() { return(1); }
