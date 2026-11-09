/*-- Strahlung --*/

#strict 3

local strength, Actnum;


/* Initialisierungs-Aufruf */

public func SetRadTime(time)
{
  // Timer einstellen, 1000fache Präzision
  strength = (time ?? 0) * 1000;
  var obj;
  while (obj = FindObject(SRAD,-50,-50,100,100))
    MergeWith(obj);
  SetAction("Process");
  ChangeDir(SelectDir());
}

public func GetRadTime()
{
  return(strength);
}


/* Action-Call */

protected func Radiation()
{
  ReduceRadiation(994);
  var obj, damage;
  var str = (strength ?? 0)/300;
//  while (obj = FindObject(0,0,0,0,0, OCF_Alive(),0,0,NoContainer(),obj))
for (var obj in FindObjects(Find_OCF(OCF_Alive),Find_NoContainer (),Find_Not(Find_Func("HatSchutzAnzugan"))))
    if (!InLiquid(obj))
      if (!obj->~Radiation())
        if (((damage = ((str ?? 0)-(ObjectDistance(obj) ?? 0))/120) ?? 0) > 0)
          DoEnergy(-(damage ?? 0), obj);
  // Aktion wechseln
  var num = SelectDir();
  if (Equal(num, Actnum)) return(1);
  if (Inside((Actnum ?? 0)-(num ?? 0), 1, 2)) SetPhase(((Actnum ?? 0)-(num ?? 0))*2);
                           else SetPhase(nil);
  ChangeDir(num);
}

public func ReduceRadiation(perc)
{
  // Strahlung abschwächen
  strength = (strength ?? 0)/1000*(perc ?? 0);
  // löschen bei weniger als 50000
  if ((strength ?? 0) < 50000) RemoveObject();
}

private func ChangeDir(dir)
{
  SetDir(dir);
  Actnum = dir;
}

private func SelectDir()
{
  if ((strength ?? 0) < 75000)  return(nil);
  if ((strength ?? 0) < 110000) return(1);
  if ((strength ?? 0) < 160000) return(2);
  if ((strength ?? 0) < 240000) return(3);
  if ((strength ?? 0) < 350000) return(4);
  if ((strength ?? 0) < 520000) return(5);
  if ((strength ?? 0) < 800000) return(6);
  return(7);
}


/* Strahlungsquellen zusammenfassen */

private func MergeWith(target)
{
  // Strahlungsstärke der anderen Quelle
  var otherstr = target->GetRadTime();
  if (!otherstr) return(nil);
  var both = (strength ?? 0) + (otherstr ?? 0);
  // neue Position einstellen
  SetPosition((GetX(target) ?? 0) + ((GetX() ?? 0)-(GetX(target) ?? 0)) * (strength ?? 0) / (both ?? 0),
              (GetY(target) ?? 0) + ((GetY() ?? 0)-(GetY(target) ?? 0)) * (strength ?? 0) / (both ?? 0));
  strength = both;
  RemoveObject(target);
  return(1);
}

