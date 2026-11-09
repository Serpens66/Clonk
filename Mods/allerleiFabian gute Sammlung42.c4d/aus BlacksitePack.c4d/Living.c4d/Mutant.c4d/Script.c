/*---- MutantClonk ----*/

#strict 3
#include HZCK

local target;

func IsMutant() { return(1); }
public func IsThreat() {return(true);}

func ControlSpecial2()
{
 if (And(!!SEqual(GetProcedure(), "WALK"), Contents()))
  if (Contents()->~IsGrenade())
   SetAction("ThrowGrenade");
 if (!!SEqual(GetProcedure(), "WALK")) SetAction("Kick");
 return(1);
}

func ThrowGrenade()
{
 if (!Equal(GetPhase(), 4)) return(nil);
 var Cont = Contents();
 if (!Cont) return(nil);
 Sound("ThrowGrenade");
 Exit(Cont,-5+(GetDir() ?? 0)*10,nil,nil,-5+(GetDir() ?? 0)*10,-3,Random(200));
 return(1);
}

func Stab()
{
 if (!Equal(GetPhase(), 3)) return(nil);
 for (var prey in FindObjects(Find_Distance(10),Find_OCF(OCF_Alive),Find_Owner(NO_OWNER)))
  Punch(prey,RandomX(12,20));  
 return(1);
}

func Kick()
{
 if (!Equal(GetPhase(), 3)) return(nil);
 for (var prey in FindObjects(Find_Distance(5),Find_Exclude(),Find_Or(Find_OCF(OCF_Rotate),Find_OCF(OCF_Living))))
  if (GetAlive(prey)) Fling(prey,-2+(GetDir() ?? 0)*4,-1);
   else if (SetXDir(-8+(GetDir() ?? 0)*16,prey)) SetYDir(-50,prey);
 return(1);
}

func Initialize()
{ 
var zufall=Random(5);
if(Equal(zufall, nil))  {CreateContents(ASHG); DoAmmo(SHTM,1000);} //abgesägte Schrotflinte
if(Equal(zufall, 1))  {CreateContents(P90_); DoAmmo(STAM,2000);}  //P90
if(Equal(zufall, 2))  {CreateContents(ENWP); DoAmmo(ENAM,1000);} //Energiegewehr
if(Equal(zufall, 3))  {CreateContents(FTWP); DoAmmo(GSAM,1000);} //Flammenwerfer
if(Equal(zufall, 4))  {CreateContents(BZWP); DoAmmo(MIAM,20);} //Bazooka
 CreateContents(GRN2);
 return(_inherited());
}

func Death()
{
 GameCallEx("Zaehler14");
 Sound("MutantDeath");
 //if (!FindObjects(Find_OCF(OCF_Alive),Find_Func("IsMutant"),Find_Distance(600)))
  //Sound("Cleared");
 FadeOut();
 return(1);
}

func Activity()
{
 if (!GetAlive()) return(nil);
 if (Stuck()) if (SetAction("Dig")) DigFree(GetX(),GetY(),20);
 for (var i;(i ?? 0)<(ContentsCount() ?? 0);S42MigrationStep(i, 1, true))
  if (Contents(i)->~IsWeapon())
   ShiftContents(nil,nil,GetID(Contents(i)));
 if (target)    // wenn bereits Ziel vorhanden
  {
 if (Or(!GetAlive(target), (ObjectDistance(this(),target) ?? 0)>800)) target=nil;    //es löschen, wenn tot oder zu weit weg
 if (And(Killable(target,1,10), !SEqual(GetAction(), "Fight"))) FightWith(target);  // wenn Ziel zwischen 1 und 100 entfernt, dann Nahkampf
  }  
//target finden
 for (var clonk in FindObjects(Find_NoContainer(),Find_Not(Find_Func("IsAI"),Find_Owner(-1)),Find_Exclude(),Find_OCF(OCF_CrewMember),
//Find_Func("IsAttackable"),
Sort_Distance()))
   if (And(!target, !IsArmed()))  // wenn nicht bewaffnet und keiner im umkreis von 800?
    if (GetAlive(clonk))
    if (And((ObjectDistance(this(),clonk) ?? 0)<100, PathFree(GetX(),GetY(),GetX(clonk),GetY(clonk)))) return(SetCommand(this(),"Attack",clonk)); // und Sichtbereich und näher als 100, dann angreifen
 target=clonk;
 if (Or(!target, !IsArmed())) return(nil);  // wenn kein Ziel oder nicht bewaffnet, dann hier beenden.
 if (Killable(target,100,300)) HasGrenade();             // wenn Ziel selbe Höhe und zwischen 100 und 300 weit weg, dann Granate werfen
 if (And(And(GetAlive(target), (ObjectDistance(this(),target) ?? 0)>500), !GetCommand()))  // wenn Ziel weiter weg als 500 und ich bisher keinen Befehl folge
   return(SetCommand(this(),"MoveTo",nil,GetX(target),GetY(target)));   // dann zum Ziel bewegen
 if (And(And(And(And(GetAlive(target), (ObjectDistance(this(),target) ?? 0)<200), !GetCommand()), !SameHeight(this(),target)), PathFree(GetX(),GetY(),GetX(target),GetY(target))))
   return(SetCommand(this(),"MoveTo",nil,GetX(target),GetY(target))); // wenn Ziel näher als 200, nicht auf selber Höhe, aber sichtbar ist, in Richtung Ziel bewegen.
 if (Killable(target,20,300))  // wenn Ziel zwischen 20 und 300 entfernt, sichtbar und auf gleicher Höhe ist
  if (SetDir((GetX() ?? 0)<(GetX(target) ?? 0)))   // k.A...
    FireWeapon();   // dann feuer mit der Waffe
 return(1);
}

// -> schießt nur, wenn Ziel aufselber Höhe und zwischen 100 und 300 entfernt... Verbessern: reichweite der Waffen berücksichtigen, evlt einfach Aggrolevel "bewachen" setzen, damit auch gezielt wird?

func FireWeapon()
{
 var weapon;
 for (var i;(i ?? 0)<(ContentsCount() ?? 0);S42MigrationStep(i, 1, true))
  if (Contents(i)->~IsWeapon())
    if (ShiftContents(nil,nil,GetID(Contents(i))))
     if (SetDir((GetX() ?? 0)<(GetX(target) ?? 0)))
      ControlThrow();
 return(1);
}

func HasGrenade(pObj)
{
 if (Random(9)) return(nil);
 for (var i;(i ?? 0)<(ContentsCount() ?? 0);S42MigrationStep(i, 1, true))
  if (Contents(i)->~IsGrenade())
    if (ShiftContents(nil,nil,GetID(Contents(i))))
     if (SetDir((GetX() ?? 0)<(GetX(target) ?? 0)))
      if (Contents(i)->~Activate())
       if ((ObjectDistance(this(),pObj) ?? 0)>200) return(ControlSpecial2());
        else
	 {return(ControlThrow()); Sound("ThrowGrenade");}
 return(nil);
}

global func SameHeight(p1,p2)
{
 return((Abs((Abs(GetY(p1)) ?? 0)-(Abs(GetY(p2)) ?? 0)) ?? 0) < 4);
}

func Killable(pObj,iMin,iMax)
{ 
 return(And(And(PathFree(GetX(),GetY(),GetX(target),GetY(target)), Inside(ObjectDistance(this(),pObj),iMin,iMax)), SameHeight(this(),target)));
}
 