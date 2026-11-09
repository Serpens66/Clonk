/*-- Sandsackbarriere --*/

#strict

func Initialize()
{
//SetPosition(GetX()+25,GetY(),this());
var clonkk = FindObject2(Find_Func("IsClonk"),Find_Distance(20));
SetCursor(GetOwner(clonkk),clonkk);
Verschiebenmenu(clonkk);
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
SetPosition(GetX()-22,GetY(),clonkk,1);
if(izahl==2)
SetPosition(GetX(),GetY()-18,clonkk,1);
if(izahl==3)
SetPosition(GetX()+20,GetY(),clonkk,1);
  }
}


protected func Incineration()
{
  // Kaputt!
  CastParticles("Sandbag", 10, 100, 0,0, 35, 45, RGBa(228,228,228,0), RGBa(250,250,250,50));
  RemoveObject();
}

/* Serialisierung */

public func Serialize(array& extra)
{
	extra[GetLength(extra)] = Format("SetDir(%d)", GetDir());
}
