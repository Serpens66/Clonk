/*-- Strahlung --*/

#strict

local strength, Actnum;


/* Initialisierungs-Aufruf */

public func SetRadTime(time)
{
  // Timer einstellen, 1000fache Präzision
  strength = time * 1000;
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
  var str = strength/300;
//  while (obj = FindObject(0,0,0,0,0, OCF_Alive(),0,0,NoContainer(),obj))
for (var obj in FindObjects(Find_OCF(OCF_Alive),Find_NoContainer (),Find_Not(Find_Func("HatSchutzAnzugan"))))
    if (!InLiquid(obj))
      if (!obj->~Radiation())
        if ((damage = (str-ObjectDistance(obj))/120) > 0)
          DoEnergy(-damage, obj);
  // Aktion wechseln
  var num = SelectDir();
  if (num == Actnum) return(1);
  if (Inside(Actnum-num, 1, 2)) SetPhase((Actnum-num)*2);
                           else SetPhase(0);
  ChangeDir(num);
}

public func ReduceRadiation(perc)
{
  // Strahlung abschwächen
  strength = strength/1000*perc;
  // löschen bei weniger als 50000
  if (strength < 50000) RemoveObject();
}

private func ChangeDir(dir)
{
  SetDir(dir);
  Actnum = dir;
}

private func SelectDir()
{
  if (strength < 75000)  return(0);
  if (strength < 110000) return(1);
  if (strength < 160000) return(2);
  if (strength < 240000) return(3);
  if (strength < 350000) return(4);
  if (strength < 520000) return(5);
  if (strength < 800000) return(6);
  return(7);
}


/* Strahlungsquellen zusammenfassen */

private func MergeWith(target)
{
  // Strahlungsstärke der anderen Quelle
  var otherstr = target->GetRadTime();
  if (!otherstr) return(0);
  var both = strength + otherstr;
  // neue Position einstellen
  SetPosition(GetX(target) + (GetX()-GetX(target)) * strength / both,
              GetY(target) + (GetY()-GetY(target)) * strength / both);
  strength = both;
  RemoveObject(target);
  return(1);
}

