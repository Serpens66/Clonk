/*-- Schutt --*/

#strict

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
CreateMenu(GetID(clonkk),clonkk,this(),0,"Clonk verschieben");
AddMenuItem("Clonk Stückchen nach links verschieben","Verschieben",PFEI,clonkk,0,1,"Da der Clonk nach dem Bau feststeckt,kannst du ihn nun rausverschieben",2,2);
AddMenuItem("Clonk Stückchen nach oben verschieben","Verschieben",PFEI,clonkk,0,2,"Da der Clonk nach dem Bau feststeckt,kannst du ihn nun rausverschieben",2,0);
AddMenuItem("Clonk Stückchen nach rechts verschieben","Verschieben",PFEI,clonkk,0,3,"Da der Clonk nach dem Bau feststeckt,kannst du ihn nun rausverschieben",2,3);
}

func Verschieben(IdImage,izahl)
{
for (var clonkk in FindObjects(Find_Func("IsClonk"),Find_Distance(20)))
  {
if(izahl==1)
SetPosition(GetX()-25,GetY(),clonkk,1);
if(izahl==2)
SetPosition(GetX(),GetY()-22,clonkk,1);
if(izahl==3)
SetPosition(GetX()+22,GetY(),clonkk,1);
  }
}



// Überladen
public func SetDirr(int iDir)
{
  iDir = BoundBy(iDir, 0, 1);
  SetAction("Exist");
  SetPhase(iDir);
  SetSolidMask(50 * iDir, 0, 50, 30, 0, 0);
  return(1);
}

/* Serialisierung */

public func Serialize(array& extra)
{
	extra[GetLength(extra)] = Format("SetDir(%d)", GetDir());

}


func DrehDich(objID,objekt)
{
if(GetPhase(objekt)==0) {SetPhase(1);Log("setphase 1 gesetzt");SetSolidMask(50, 0, 50, 30, 0, 0);}
if(GetPhase(objekt)==1) {SetPhase(0);Log("setphase 0 gesetzt");SetSolidMask(0, 0, 50, 30, 0, 0);}
}