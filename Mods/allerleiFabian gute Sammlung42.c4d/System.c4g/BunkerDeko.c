/*-- Bunker Deko --*/

//Damit die Dekoobjekte nur im Bunker gebaut werden können

#strict 3

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
  iX ??= 0;
  iY ??= 0;
  while (pObj = FindObject(nil, iX,iY,nil,nil, OCF_Fullcon,nil,nil, NoContainer(), pObj))
   {
//Log("Folgendes Objekt gefunden: %v",pObj);
 if (pObj->~CastlePartWidth())
      return(pObj);
   }
  // Keins gefunden
  return(nil);
}

private func CheckPlacement(int x, int y)
{
  // Nur vor Burgteile
  x ??= 0;
  y ??= 0;
  var castle = FindCastlePart(x, (y ?? 0) - 10);
  if(!castle) return("Nur im Bunker baubar");
  
  // Nur vor Waende
  if(And(And(And(And(And(And(And(And(And(And(And(And(And(!Equal(GetID(castle), BP5C), !Equal(GetID(castle), BW5C)), !Equal(GetID(castle), B15C)), !Equal(GetID(castle), BT5C)), !Equal(GetID(castle), BE5C)), !Equal(GetID(castle), HL0D)), !Equal(GetID(castle), HR0D)), !Equal(GetID(castle), BH5C)), !Equal(GetID(castle), RR5C)), !Equal(GetID(castle), HR6Y)), !Equal(GetID(castle), BG5C)), !Equal(GetID(castle), BL5C)), !Equal(GetID(castle), BR5C)), !Equal(GetID(castle), CH5C)))
    return("Muss vor einer schmalen/mittelbreiten Bunkerwand gebaut werden");
}

/* Callback vom Bausatz */

public func RejectConstruction(int x, int y, object clonk)
{
  x ??= 0;
  y ??= 0;
  var err = CheckPlacement((GetX(clonk) ?? 0) + (x ?? 0), (GetY(clonk) ?? 0) + (y ?? 0));
  if(Equal(GetType(err), C4V_String) )
  {
    Message(err, clonk);
    Sound("Error", nil, clonk);
    return(1);
  }
  else
  {
    return(nil);
  }
}