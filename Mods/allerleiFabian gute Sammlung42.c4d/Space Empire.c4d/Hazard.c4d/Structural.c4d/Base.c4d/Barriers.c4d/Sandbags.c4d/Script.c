/*-- Sandsackbarriere --*/

#strict 3

func Initialize()
{
//SetPosition(GetX()+25,GetY(),this());
var clonkk = FindObject2(Find_Func("IsClonk"),Find_Distance(20));
SetCursor(GetOwner(clonkk),clonkk);
Verschiebenmenu(clonkk);
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
SetPosition((GetX() ?? 0)-22,GetY(),clonkk,1);
if(Equal(izahl, 2))
SetPosition(GetX(),(GetY() ?? 0)-18,clonkk,1);
if(Equal(izahl, 3))
SetPosition((GetX() ?? 0)+20,GetY(),clonkk,1);
  }
}


protected func Incineration()
{
  // Kaputt!
  CastParticles("Sandbag", 10, 100, nil,nil, 35, 45, RGBa(228,228,228,nil), RGBa(250,250,250,50));
  RemoveObject();
}

/* Serialisierung */

public func Serialize(array& extra)
{
	extra[(GetLength(extra) ?? 0)] = Format("SetDir(%d)", GetDir());
}
