/*---- MutantClonk ----*/

#strict
#include HZCK

local target;

func IsMutant() { return(1); }
public func IsThreat() {return(true);}

func ControlSpecial2()
{
 if (GetProcedure() eq "WALK" && Contents())
  if (Contents()->~IsGrenade())
   SetAction("ThrowGrenade");
 if (GetProcedure() eq "WALK") SetAction("Kick"); 
 return(1);
}

func ThrowGrenade()
{
 if (GetPhase() != 4) return(0);
 var Cont = Contents();
 if (!Cont) return(0);
 Sound("ThrowGrenade");
 Exit(Cont,-5+GetDir()*10,0,0,-5+GetDir()*10,-3,Random(200));
 return(1);
}

func Stab()
{
 if (GetPhase() != 3) return(0);
 for (var prey in FindObjects(Find_Distance(10),Find_OCF(OCF_Alive),Find_Owner(NO_OWNER)))
  Punch(prey,RandomX(12,20));  
 return(1);
}

func Kick()
{
 if (GetPhase() != 3) return(0);
 for (var prey in FindObjects(Find_Distance(5),Find_Exclude(),Find_Or(Find_OCF(OCF_Rotate),Find_OCF(OCF_Living))))
  if (GetAlive(prey)) Fling(prey,-2+GetDir()*4,-1);
   else if (SetXDir(-8+GetDir()*16,prey)) SetYDir(-50,prey);
 return(1);
}

func Initialize()
{ 
var zufall=Random(5);
if(zufall==0)  {CreateContents(ASHG); DoAmmo(SHTM,1000);} //abgesägte Schrotflinte
if(zufall==1)  {CreateContents(P90_); DoAmmo(STAM,2000);}  //P90
if(zufall==2)  {CreateContents(ENWP); DoAmmo(ENAM,1000);} //Energiegewehr
if(zufall==3)  {CreateContents(FTWP); DoAmmo(GSAM,1000);} //Flammenwerfer
if(zufall==4)  {CreateContents(BZWP); DoAmmo(MIAM,20);} //Bazooka
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
 if (!GetAlive()) return(0);
 if (Stuck()) if (SetAction("Dig")) DigFree(GetX(),GetY(),20);
 for (var i;i<ContentsCount();i++)
  if (Contents(i)->~IsWeapon())
   ShiftContents(0,0,GetID(Contents(i)));
 if (target)    // wenn bereits Ziel vorhanden
  {
 if (!GetAlive(target) || ObjectDistance(this(),target)>800) target=0;    //es löschen, wenn tot oder zu weit weg
 if (Killable(target,1,10) && GetAction() ne "Fight") FightWith(target);  // wenn Ziel zwischen 1 und 100 entfernt, dann Nahkampf
  }  
//target finden
 for (var clonk in FindObjects(Find_NoContainer(),Find_Not(Find_Func("IsAI"),Find_Owner(-1)),Find_Exclude(),Find_OCF(OCF_CrewMember),
//Find_Func("IsAttackable"),
Sort_Distance()))
   if (!target && !IsArmed())  // wenn nicht bewaffnet und keiner im umkreis von 800?
    if (GetAlive(clonk))
    if (ObjectDistance(this(),clonk)<100 && PathFree(GetX(),GetY(),GetX(clonk),GetY(clonk))) return(SetCommand(this(),"Attack",clonk)); // und Sichtbereich und näher als 100, dann angreifen
 target=clonk;
 if (!target || !IsArmed()) return(0);  // wenn kein Ziel oder nicht bewaffnet, dann hier beenden.
 if (Killable(target,100,300)) HasGrenade();             // wenn Ziel selbe Höhe und zwischen 100 und 300 weit weg, dann Granate werfen
 if (GetAlive(target) && ObjectDistance(this(),target)>500 && !GetCommand())  // wenn Ziel weiter weg als 500 und ich bisher keinen Befehl folge
   return(SetCommand(this(),"MoveTo",0,GetX(target),GetY(target)));   // dann zum Ziel bewegen
 if (GetAlive(target) && ObjectDistance(this(),target)<200 && !GetCommand() && !SameHeight(this(),target) && PathFree(GetX(),GetY(),GetX(target),GetY(target)))
   return(SetCommand(this(),"MoveTo",0,GetX(target),GetY(target))); // wenn Ziel näher als 200, nicht auf selber Höhe, aber sichtbar ist, in Richtung Ziel bewegen.
 if (Killable(target,20,300))  // wenn Ziel zwischen 20 und 300 entfernt, sichtbar und auf gleicher Höhe ist
  if (SetDir(GetX()<GetX(target)))   // k.A... 
    FireWeapon();   // dann feuer mit der Waffe
 return(1);
}

// -> schießt nur, wenn Ziel aufselber Höhe und zwischen 100 und 300 entfernt... Verbessern: reichweite der Waffen berücksichtigen, evlt einfach Aggrolevel "bewachen" setzen, damit auch gezielt wird?

func FireWeapon()
{
 var weapon;
 for (var i;i<ContentsCount();i++)
  if (Contents(i)->~IsWeapon())
    if (ShiftContents(0,0,GetID(Contents(i))))
     if (SetDir(GetX()<GetX(target)))
      ControlThrow();
 return(1);
}

func HasGrenade(pObj)
{
 if (Random(9)) return(0);
 for (var i;i<ContentsCount();i++)
  if (Contents(i)->~IsGrenade())
    if (ShiftContents(0,0,GetID(Contents(i))))
     if (SetDir(GetX()<GetX(target)))
      if (Contents(i)->~Activate())
       if (ObjectDistance(this(),pObj)>200) return(ControlSpecial2());
        else
	 {return(ControlThrow()); Sound("ThrowGrenade");}
 return(0);
}

global func SameHeight(p1,p2)
{
 return(Abs(Abs(GetY(p1))-Abs(GetY(p2))) < 4);
}

func Killable(pObj,iMin,iMax)
{ 
 return(PathFree(GetX(),GetY(),GetX(target),GetY(target)) && Inside(ObjectDistance(this(),pObj),iMin,iMax) && SameHeight(this(),target));
}
 