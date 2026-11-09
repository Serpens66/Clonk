/*-- Bunker Deko --*/

//Damit die Dekoobjekte nur im Bunker gebaut werden können

#strict

// die Dekoobjekte
#appendto VENT
#appendto MVNT
#appendto SCR2
#appendto SCR1
#appendto HSGN
#appendto CLVM
#appendto LCKR
#appendto SPVM
#appendto PLNT
#appendto PLT2
#appendto PSTR
#appendto CGLO
#appendto MPPJ
#appendto LBPC
//#appendto GSBO  gasflasche und Tank sollen überall gebaut werden können
//#appendto GSTA
#appendto BLG3 //hintergrundlicht
#appendto CLG3 //Deckenlicht
#appendto GADG // deko apparaturen


public func Completion(a,b,c,d,e,f,g)
{
  SetPlrViewRange(100,this(),1); 
return(_inherited(a,b,c,d,e,f,g));
}

private func StructureCheck()
{
  //Log("Structurecheck der Deko ausgeführt");
  if(!FindCastlePart(1,1) )
    RemoveObject();
}

public func CastleChange()
{
  StructureCheck();
}

public func FindCastlePart(int iX, int iY, object pObj)
{
 //Log("FindCastle von Deko ausgeführt");
  while (pObj = FindObject(0, iX,iY,0,0, OCF_Fullcon(),0,0, NoContainer(), pObj))
   {
//Log("Folgendes Objekt gefunden: %v",pObj);
 if (pObj->~CastlePartWidth())
      return(pObj);
   }
  // Keins gefunden
  return(0);
}

private func CheckPlacement(int x, int y)
{
  // Nur vor Burgteile
  var castle = FindCastlePart(x, y - 10);
  if(!castle) return("Nur im Bunker baubar");
  
  // Nur vor Waende
  if(GetID(castle) != BP5C && GetID(castle) != BW5C && GetID(castle) != B15C && GetID(castle) != BT5C && GetID(castle) != BE5C && GetID(castle) != HL0D && GetID(castle) != HR0D && GetID(castle) != BH5C && GetID(castle) != RR5C && GetID(castle) != HR6Y && GetID(castle) != BG5C && GetID(castle) != BL5C && GetID(castle) != BR5C && GetID(castle) != CH5C)
    return("Muss vor einer schmalen/mittelbreiten Bunkerwand gebaut werden");
}

/* Callback vom Bausatz */

public func RejectConstruction(int x, int y, object clonk)
{
  var err = CheckPlacement(GetX(clonk) + x, GetY(clonk) + y);
  if(GetType(err) == C4V_String() )
  {
    Message(err, clonk);
    Sound("Error", 0, clonk);
    return(1);
  }
  else
  {
    return(0);
  }
}