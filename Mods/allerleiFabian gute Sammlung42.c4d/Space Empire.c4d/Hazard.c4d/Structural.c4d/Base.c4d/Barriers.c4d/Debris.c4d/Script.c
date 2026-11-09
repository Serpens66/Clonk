/*-- Schutt --*/

#strict 3

func Initialize()
{
//SetPosition(GetX()+35,GetY(),this());
var clonkk = FindObject2(Find_Func("IsClonk"),Find_Distance(20));
SetCursor(GetOwner(clonkk),clonkk);
Verschiebenmenu(clonkk);
  //SetAction("Exist");
}


func Verschiebenmenu(clonkk)
{
CreateMenu(GetID(clonkk),clonkk,this(),nil,"Clonk verschieben");
AddMenuItem("Clonk Stückchen nach links verschieben","Verschieben",PFEI,clonkk,nil,1,"Da der Clonk nach dem Bau feststeckt,kannst du ihn nun rausverschieben",2,2);
AddMenuItem("Clonk Stückchen nach oben verschieben","Verschieben",PFEI,clonkk,nil,2,"Da der Clonk nach dem Bau feststeckt,kannst du ihn nun rausverschieben",2,nil);
AddMenuItem("Clonk Stückchen nach rechts verschieben","Verschieben",PFEI,clonkk,nil,3,"Da der Clonk nach dem Bau feststeckt,kannst du ihn nun rausverschieben",2,3);
}

func Verschieben(IdImage,izahl)
{
for (var clonkk in FindObjects(Find_Func("IsClonk"),Find_Distance(20)))
  {
if(Equal(izahl, 1))
SetPosition((GetX() ?? 0)-25,GetY(),clonkk,1);
if(Equal(izahl, 2))
SetPosition(GetX(),(GetY() ?? 0)-22,clonkk,1);
if(Equal(izahl, 3))
SetPosition((GetX() ?? 0)+22,GetY(),clonkk,1);
  }
}



// Überladen
public func SetDirr(int iDir)
{
  iDir ??= 0;
  iDir = BoundBy(iDir, nil, 1);
  SetAction("Exist");
  SetPhase(iDir);
  SetSolidMask(50 * (iDir ?? 0), nil, 50, 30, nil, nil);
  return(1);
}

/* Serialisierung */

public func Serialize(array& extra)
{
	extra[(GetLength(extra) ?? 0)] = Format("SetDir(%d)", GetDir());

}


func DrehDich(objID,objekt)
{
if(Equal(GetPhase(objekt), nil)) {SetPhase(1);Log("setphase 1 gesetzt");SetSolidMask(50, nil, 50, 30, nil, nil);}
if(Equal(GetPhase(objekt), 1)) {SetPhase(nil);Log("setphase 0 gesetzt");SetSolidMask(nil, nil, 50, 30, nil, nil);}
}