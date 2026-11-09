/*-- Verbrannte Werkstatt --*/

#strict 3

#include BAS7

func Ejection(obj) 
{
  if (Equal(GetCon(obj), 100)) return;
  if (OnFire()) 
    if (Or(GetDefCoreVal("BlastIncinerate", "DefCore", GetID(obj)), GetDefCoreVal("ContactIncinerate", "DefCore", GetID(obj))))
      return(Incinerate(obj));
  if (~(GetOCF(obj) ?? 0) & (OCF_CrewMember ?? 0))
    Split2Components(obj);
}
