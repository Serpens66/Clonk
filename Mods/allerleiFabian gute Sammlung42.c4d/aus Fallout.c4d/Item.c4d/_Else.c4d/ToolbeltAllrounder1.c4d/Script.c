/*-- Gürtel klein Munition --*/

#strict 3
#include GEAR





/* Status */

public func GetGearType()	{ return(GEAR_Boots); }
protected func Hit()		{ Sound("Munch1"); }
public func IsEquipment()	{ return(true); }

/* Callbacks */

public func GearEffect(object pClonk)
{
pClonk->GetMoreAmmo(19);
}

public func GearEffectReset(object pClonk)
{
pClonk->GetNormalAmmo();
Ammomengekorrigieren(pClonk);
}

func Ammomengekorrigieren(pClonk)
{

//Dafür sorgen, dass beim ablegen des Gürtels das Maximum nicht überschritten ist und Munitionskisten erzeugen

var ammo_id1 = SSTM;
var pack_id1 = STLM;
var ammo_id2 = GSAM;
var pack_id2 = GSAP;
var ammo_id3 = ENAM;
var pack_id3 = ENAP;
var ammo_id4 = GRAM;
var pack_id4 = GRAP;
var ammo_id5 = PTAM;
var pack_id5 = PTAP;
var ammo_id6 = STAM;
var pack_id6 = STAP;
var ammo_id7 = MIAM;
var pack_id7 = MIAP;
var ammo_id8 = SHTM;
var pack_id8 = SHMU;
var ammo_id9 = SPBT;
var pack_id9 = SNI_;


var in_ammobag1= GetAmmo(ammo_id1,pClonk);
var maximum1 = ammo_id1->MaxAmmo(pClonk);
var packed1 = pack_id1->AmmoCount();
if((in_ammobag1 ?? 0)>(maximum1 ?? 0))
{ DoAmmo(ammo_id1,-(packed1 ?? 0),pClonk); CreateObject(pack_id1,AbsX(GetX()),AbsY(GetY()),GetOwner());}
in_ammobag1= GetAmmo(ammo_id1,pClonk);
if((in_ammobag1 ?? 0)>(maximum1 ?? 0))
{ DoAmmo(ammo_id1,-(packed1 ?? 0),pClonk); CreateObject(pack_id1,AbsX(GetX()),AbsY(GetY()),GetOwner());}
in_ammobag1= GetAmmo(ammo_id1,pClonk);
if((in_ammobag1 ?? 0)>(maximum1 ?? 0))
{ DoAmmo(ammo_id1,-(packed1 ?? 0),pClonk); CreateObject(pack_id1,AbsX(GetX()),AbsY(GetY()),GetOwner());}
in_ammobag1= GetAmmo(ammo_id1,pClonk);
if((in_ammobag1 ?? 0)>(maximum1 ?? 0))
{ DoAmmo(ammo_id1,-(packed1 ?? 0),pClonk); CreateObject(pack_id1,AbsX(GetX()),AbsY(GetY()),GetOwner());}
in_ammobag1= GetAmmo(ammo_id1,pClonk);
if((in_ammobag1 ?? 0)>(maximum1 ?? 0))
{ DoAmmo(ammo_id1,-(packed1 ?? 0),pClonk); CreateObject(pack_id1,AbsX(GetX()),AbsY(GetY()),GetOwner());}
in_ammobag1= GetAmmo(ammo_id1,pClonk);
if((in_ammobag1 ?? 0)>(maximum1 ?? 0))
{ DoAmmo(ammo_id1,-(packed1 ?? 0),pClonk); CreateObject(pack_id1,AbsX(GetX()),AbsY(GetY()),GetOwner());}
in_ammobag1= GetAmmo(ammo_id1,pClonk);
if((in_ammobag1 ?? 0)>(maximum1 ?? 0))
{ DoAmmo(ammo_id1,-(packed1 ?? 0),pClonk); CreateObject(pack_id1,AbsX(GetX()),AbsY(GetY()),GetOwner());}
in_ammobag1= GetAmmo(ammo_id1,pClonk);




var in_ammobag2= GetAmmo(ammo_id2,pClonk);
var maximum2 = ammo_id2->MaxAmmo(pClonk);
var packed2 = pack_id2->AmmoCount();
if((in_ammobag2 ?? 0)>(maximum2 ?? 0))
{ DoAmmo(ammo_id2,-(packed2 ?? 0),pClonk); CreateObject(pack_id2,AbsX(GetX()),AbsY(GetY()),GetOwner());}
in_ammobag2= GetAmmo(ammo_id2,pClonk);
if((in_ammobag2 ?? 0)>(maximum2 ?? 0))
{ DoAmmo(ammo_id2,-(packed2 ?? 0),pClonk); CreateObject(pack_id2,AbsX(GetX()),AbsY(GetY()),GetOwner());}
in_ammobag2= GetAmmo(ammo_id2,pClonk);
if((in_ammobag2 ?? 0)>(maximum2 ?? 0))
{ DoAmmo(ammo_id2,-(packed2 ?? 0),pClonk); CreateObject(pack_id2,AbsX(GetX()),AbsY(GetY()),GetOwner());}
in_ammobag2= GetAmmo(ammo_id2,pClonk);
if((in_ammobag2 ?? 0)>(maximum2 ?? 0))
{ DoAmmo(ammo_id2,-(packed2 ?? 0),pClonk); CreateObject(pack_id2,AbsX(GetX()),AbsY(GetY()),GetOwner());}
in_ammobag2= GetAmmo(ammo_id2,pClonk);
if((in_ammobag2 ?? 0)>(maximum2 ?? 0))
{ DoAmmo(ammo_id2,-(packed2 ?? 0),pClonk); CreateObject(pack_id2,AbsX(GetX()),AbsY(GetY()),GetOwner());}
in_ammobag2= GetAmmo(ammo_id2,pClonk);
if((in_ammobag2 ?? 0)>(maximum2 ?? 0))
{ DoAmmo(ammo_id2,-(packed2 ?? 0),pClonk); CreateObject(pack_id2,AbsX(GetX()),AbsY(GetY()),GetOwner());}
in_ammobag2= GetAmmo(ammo_id2,pClonk);
if((in_ammobag2 ?? 0)>(maximum2 ?? 0))
{ DoAmmo(ammo_id2,-(packed2 ?? 0),pClonk); CreateObject(pack_id2,AbsX(GetX()),AbsY(GetY()),GetOwner());}
in_ammobag2= GetAmmo(ammo_id2,pClonk);





var in_ammobag3= GetAmmo(ammo_id3,pClonk);
var maximum3 = ammo_id3->MaxAmmo(pClonk);
var packed3 = pack_id3->AmmoCount();
if((in_ammobag3 ?? 0)>(maximum3 ?? 0))
{ DoAmmo(ammo_id3,-(packed3 ?? 0),pClonk); CreateObject(pack_id3,AbsX(GetX()),AbsY(GetY()),GetOwner());}
in_ammobag3= GetAmmo(ammo_id3,pClonk);
if((in_ammobag3 ?? 0)>(maximum3 ?? 0))
{ DoAmmo(ammo_id3,-(packed3 ?? 0),pClonk); CreateObject(pack_id3,AbsX(GetX()),AbsY(GetY()),GetOwner());}
in_ammobag3= GetAmmo(ammo_id3,pClonk);
if((in_ammobag3 ?? 0)>(maximum3 ?? 0))
{ DoAmmo(ammo_id3,-(packed3 ?? 0),pClonk); CreateObject(pack_id3,AbsX(GetX()),AbsY(GetY()),GetOwner());}
in_ammobag3= GetAmmo(ammo_id3,pClonk);
if((in_ammobag3 ?? 0)>(maximum3 ?? 0))
{ DoAmmo(ammo_id3,-(packed3 ?? 0),pClonk); CreateObject(pack_id3,AbsX(GetX()),AbsY(GetY()),GetOwner());}
in_ammobag3= GetAmmo(ammo_id3,pClonk);
if((in_ammobag3 ?? 0)>(maximum3 ?? 0))
{ DoAmmo(ammo_id3,-(packed3 ?? 0),pClonk); CreateObject(pack_id3,AbsX(GetX()),AbsY(GetY()),GetOwner());}
in_ammobag3= GetAmmo(ammo_id3,pClonk);
if((in_ammobag3 ?? 0)>(maximum3 ?? 0))
{ DoAmmo(ammo_id3,-(packed3 ?? 0),pClonk); CreateObject(pack_id3,AbsX(GetX()),AbsY(GetY()),GetOwner());}
in_ammobag3= GetAmmo(ammo_id3,pClonk);
if((in_ammobag3 ?? 0)>(maximum3 ?? 0))
{ DoAmmo(ammo_id3,-(packed3 ?? 0),pClonk); CreateObject(pack_id3,AbsX(GetX()),AbsY(GetY()),GetOwner());}
in_ammobag3= GetAmmo(ammo_id3,pClonk);
if((in_ammobag3 ?? 0)>(maximum3 ?? 0))
{ DoAmmo(ammo_id3,-(packed3 ?? 0),pClonk); CreateObject(pack_id3,AbsX(GetX()),AbsY(GetY()),GetOwner());}
in_ammobag3= GetAmmo(ammo_id3,pClonk);




var in_ammobag4= GetAmmo(ammo_id4,pClonk);
var maximum4 = ammo_id4->MaxAmmo(pClonk);
var packed4 = pack_id4->AmmoCount();
if((in_ammobag4 ?? 0)>(maximum4 ?? 0))
{ DoAmmo(ammo_id4,-(packed4 ?? 0),pClonk); CreateObject(pack_id4,AbsX(GetX()),AbsY(GetY()),GetOwner());}
in_ammobag4= GetAmmo(ammo_id4,pClonk);
if((in_ammobag4 ?? 0)>(maximum4 ?? 0))
{ DoAmmo(ammo_id4,-(packed4 ?? 0),pClonk); CreateObject(pack_id4,AbsX(GetX()),AbsY(GetY()),GetOwner());}
in_ammobag4= GetAmmo(ammo_id4,pClonk);
if((in_ammobag4 ?? 0)>(maximum4 ?? 0))
{ DoAmmo(ammo_id4,-(packed4 ?? 0),pClonk); CreateObject(pack_id4,AbsX(GetX()),AbsY(GetY()),GetOwner());}
in_ammobag4= GetAmmo(ammo_id4,pClonk);
if((in_ammobag4 ?? 0)>(maximum4 ?? 0))
{ DoAmmo(ammo_id4,-(packed4 ?? 0),pClonk); CreateObject(pack_id4,AbsX(GetX()),AbsY(GetY()),GetOwner());}
in_ammobag4= GetAmmo(ammo_id4,pClonk);
if((in_ammobag4 ?? 0)>(maximum4 ?? 0))
{ DoAmmo(ammo_id4,-(packed4 ?? 0),pClonk); CreateObject(pack_id4,AbsX(GetX()),AbsY(GetY()),GetOwner());}
in_ammobag4= GetAmmo(ammo_id4,pClonk);
if((in_ammobag4 ?? 0)>(maximum4 ?? 0))
{ DoAmmo(ammo_id4,-(packed4 ?? 0),pClonk); CreateObject(pack_id4,AbsX(GetX()),AbsY(GetY()),GetOwner());}
in_ammobag4= GetAmmo(ammo_id4,pClonk);
if((in_ammobag4 ?? 0)>(maximum4 ?? 0))
{ DoAmmo(ammo_id4,-(packed4 ?? 0),pClonk); CreateObject(pack_id4,AbsX(GetX()),AbsY(GetY()),GetOwner());}
in_ammobag4= GetAmmo(ammo_id4,pClonk);
if((in_ammobag4 ?? 0)>(maximum4 ?? 0))
{ DoAmmo(ammo_id4,-(packed4 ?? 0),pClonk); CreateObject(pack_id4,AbsX(GetX()),AbsY(GetY()),GetOwner());}
in_ammobag4= GetAmmo(ammo_id4,pClonk);





var in_ammobag5= GetAmmo(ammo_id5,pClonk);
var maximum5 = ammo_id5->MaxAmmo(pClonk);
var packed5 = pack_id5->AmmoCount();
if((in_ammobag5 ?? 0)>(maximum5 ?? 0))
{ DoAmmo(ammo_id5,-(packed5 ?? 0),pClonk); CreateObject(pack_id5,AbsX(GetX()),AbsY(GetY()),GetOwner());}
in_ammobag5= GetAmmo(ammo_id5,pClonk);
if((in_ammobag5 ?? 0)>(maximum5 ?? 0))
{ DoAmmo(ammo_id5,-(packed5 ?? 0),pClonk); CreateObject(pack_id5,AbsX(GetX()),AbsY(GetY()),GetOwner());}
in_ammobag5= GetAmmo(ammo_id5,pClonk);
if((in_ammobag5 ?? 0)>(maximum5 ?? 0))
{ DoAmmo(ammo_id5,-(packed5 ?? 0),pClonk); CreateObject(pack_id5,AbsX(GetX()),AbsY(GetY()),GetOwner());}
in_ammobag5= GetAmmo(ammo_id5,pClonk);
if((in_ammobag5 ?? 0)>(maximum5 ?? 0))
{ DoAmmo(ammo_id5,-(packed5 ?? 0),pClonk); CreateObject(pack_id5,AbsX(GetX()),AbsY(GetY()),GetOwner());}
in_ammobag5= GetAmmo(ammo_id5,pClonk);
if((in_ammobag5 ?? 0)>(maximum5 ?? 0))
{ DoAmmo(ammo_id5,-(packed5 ?? 0),pClonk); CreateObject(pack_id5,AbsX(GetX()),AbsY(GetY()),GetOwner());}
in_ammobag5= GetAmmo(ammo_id5,pClonk);
if((in_ammobag5 ?? 0)>(maximum5 ?? 0))
{ DoAmmo(ammo_id5,-(packed5 ?? 0),pClonk); CreateObject(pack_id5,AbsX(GetX()),AbsY(GetY()),GetOwner());}
in_ammobag5= GetAmmo(ammo_id5,pClonk);
if((in_ammobag5 ?? 0)>(maximum5 ?? 0))
{ DoAmmo(ammo_id5,-(packed5 ?? 0),pClonk); CreateObject(pack_id5,AbsX(GetX()),AbsY(GetY()),GetOwner());}
in_ammobag5= GetAmmo(ammo_id5,pClonk);
if((in_ammobag5 ?? 0)>(maximum5 ?? 0))
{ DoAmmo(ammo_id5,-(packed5 ?? 0),pClonk); CreateObject(pack_id5,AbsX(GetX()),AbsY(GetY()),GetOwner());}
in_ammobag5= GetAmmo(ammo_id5,pClonk);





var in_ammobag6= GetAmmo(ammo_id6,pClonk);
var maximum6 = ammo_id6->MaxAmmo(pClonk);
var packed6 = pack_id6->AmmoCount();
if((in_ammobag6 ?? 0)>(maximum6 ?? 0))
{ DoAmmo(ammo_id6,-(packed6 ?? 0),pClonk); CreateObject(pack_id6,AbsX(GetX()),AbsY(GetY()),GetOwner());}
in_ammobag6= GetAmmo(ammo_id6,pClonk);
if((in_ammobag6 ?? 0)>(maximum6 ?? 0))
{ DoAmmo(ammo_id6,-(packed6 ?? 0),pClonk); CreateObject(pack_id6,AbsX(GetX()),AbsY(GetY()),GetOwner());}
in_ammobag6= GetAmmo(ammo_id6,pClonk);
if((in_ammobag6 ?? 0)>(maximum6 ?? 0))
{ DoAmmo(ammo_id6,-(packed6 ?? 0),pClonk); CreateObject(pack_id6,AbsX(GetX()),AbsY(GetY()),GetOwner());}
in_ammobag6= GetAmmo(ammo_id6,pClonk);
if((in_ammobag6 ?? 0)>(maximum6 ?? 0))
{ DoAmmo(ammo_id6,-(packed6 ?? 0),pClonk); CreateObject(pack_id6,AbsX(GetX()),AbsY(GetY()),GetOwner());}
in_ammobag6= GetAmmo(ammo_id6,pClonk);
if((in_ammobag6 ?? 0)>(maximum6 ?? 0))
{ DoAmmo(ammo_id6,-(packed6 ?? 0),pClonk); CreateObject(pack_id6,AbsX(GetX()),AbsY(GetY()),GetOwner());}
in_ammobag6= GetAmmo(ammo_id6,pClonk);
if((in_ammobag6 ?? 0)>(maximum6 ?? 0))
{ DoAmmo(ammo_id6,-(packed6 ?? 0),pClonk); CreateObject(pack_id6,AbsX(GetX()),AbsY(GetY()),GetOwner());}
in_ammobag6= GetAmmo(ammo_id6,pClonk);
if((in_ammobag6 ?? 0)>(maximum6 ?? 0))
{ DoAmmo(ammo_id6,-(packed6 ?? 0),pClonk); CreateObject(pack_id6,AbsX(GetX()),AbsY(GetY()),GetOwner());}
in_ammobag6= GetAmmo(ammo_id6,pClonk);
if((in_ammobag6 ?? 0)>(maximum6 ?? 0))
{ DoAmmo(ammo_id6,-(packed6 ?? 0),pClonk); CreateObject(pack_id6,AbsX(GetX()),AbsY(GetY()),GetOwner());}
in_ammobag6= GetAmmo(ammo_id6,pClonk);





var in_ammobag7= GetAmmo(ammo_id7,pClonk);
var maximum7 = ammo_id7->MaxAmmo(pClonk);
var packed7 = pack_id7->AmmoCount();
if((in_ammobag7 ?? 0)>(maximum7 ?? 0))
{ DoAmmo(ammo_id7,-(packed7 ?? 0),pClonk); CreateObject(pack_id7,AbsX(GetX()),AbsY(GetY()),GetOwner());}
in_ammobag7= GetAmmo(ammo_id7,pClonk);
if((in_ammobag7 ?? 0)>(maximum7 ?? 0))
{ DoAmmo(ammo_id7,-(packed7 ?? 0),pClonk); CreateObject(pack_id7,AbsX(GetX()),AbsY(GetY()),GetOwner());}
in_ammobag7= GetAmmo(ammo_id7,pClonk);
if((in_ammobag7 ?? 0)>(maximum7 ?? 0))
{ DoAmmo(ammo_id7,-(packed7 ?? 0),pClonk); CreateObject(pack_id7,AbsX(GetX()),AbsY(GetY()),GetOwner());}
in_ammobag7= GetAmmo(ammo_id7,pClonk);
if((in_ammobag7 ?? 0)>(maximum7 ?? 0))
{ DoAmmo(ammo_id7,-(packed7 ?? 0),pClonk); CreateObject(pack_id7,AbsX(GetX()),AbsY(GetY()),GetOwner());}
in_ammobag7= GetAmmo(ammo_id7,pClonk);
if((in_ammobag7 ?? 0)>(maximum7 ?? 0))
{ DoAmmo(ammo_id7,-(packed7 ?? 0),pClonk); CreateObject(pack_id7,AbsX(GetX()),AbsY(GetY()),GetOwner());}
in_ammobag7= GetAmmo(ammo_id7,pClonk);
if((in_ammobag7 ?? 0)>(maximum7 ?? 0))
{ DoAmmo(ammo_id7,-(packed7 ?? 0),pClonk); CreateObject(pack_id7,AbsX(GetX()),AbsY(GetY()),GetOwner());}
in_ammobag7= GetAmmo(ammo_id7,pClonk);
if((in_ammobag7 ?? 0)>(maximum7 ?? 0))
{ DoAmmo(ammo_id7,-(packed7 ?? 0),pClonk); CreateObject(pack_id7,AbsX(GetX()),AbsY(GetY()),GetOwner());}
in_ammobag7= GetAmmo(ammo_id7,pClonk);
if((in_ammobag7 ?? 0)>(maximum7 ?? 0))
{ DoAmmo(ammo_id7,-(packed7 ?? 0),pClonk); CreateObject(pack_id7,AbsX(GetX()),AbsY(GetY()),GetOwner());}
in_ammobag7= GetAmmo(ammo_id7,pClonk);
if((in_ammobag7 ?? 0)>(maximum7 ?? 0))
{ DoAmmo(ammo_id7,-(packed7 ?? 0),pClonk); CreateObject(pack_id7,AbsX(GetX()),AbsY(GetY()),GetOwner());}
in_ammobag7= GetAmmo(ammo_id7,pClonk);





var in_ammobag8= GetAmmo(ammo_id8,pClonk);
var maximum8 = ammo_id8->MaxAmmo(pClonk);
var packed8 = pack_id8->AmmoCount();
if((in_ammobag8 ?? 0)>(maximum8 ?? 0))
{ DoAmmo(ammo_id8,-(packed8 ?? 0),pClonk); CreateObject(pack_id8,AbsX(GetX()),AbsY(GetY()),GetOwner());}
in_ammobag8= GetAmmo(ammo_id8,pClonk);
if((in_ammobag8 ?? 0)>(maximum8 ?? 0))
{ DoAmmo(ammo_id8,-(packed8 ?? 0),pClonk); CreateObject(pack_id8,AbsX(GetX()),AbsY(GetY()),GetOwner());}
in_ammobag8= GetAmmo(ammo_id8,pClonk);
if((in_ammobag8 ?? 0)>(maximum8 ?? 0))
{ DoAmmo(ammo_id8,-(packed8 ?? 0),pClonk); CreateObject(pack_id8,AbsX(GetX()),AbsY(GetY()),GetOwner());}
in_ammobag8= GetAmmo(ammo_id8,pClonk);
if((in_ammobag8 ?? 0)>(maximum8 ?? 0))
{ DoAmmo(ammo_id8,-(packed8 ?? 0),pClonk); CreateObject(pack_id8,AbsX(GetX()),AbsY(GetY()),GetOwner());}
in_ammobag8= GetAmmo(ammo_id8,pClonk);
if((in_ammobag8 ?? 0)>(maximum8 ?? 0))
{ DoAmmo(ammo_id8,-(packed8 ?? 0),pClonk); CreateObject(pack_id8,AbsX(GetX()),AbsY(GetY()),GetOwner());}
in_ammobag8= GetAmmo(ammo_id8,pClonk);
if((in_ammobag8 ?? 0)>(maximum8 ?? 0))
{ DoAmmo(ammo_id8,-(packed8 ?? 0),pClonk); CreateObject(pack_id8,AbsX(GetX()),AbsY(GetY()),GetOwner());}
in_ammobag8= GetAmmo(ammo_id8,pClonk);
if((in_ammobag8 ?? 0)>(maximum8 ?? 0))
{ DoAmmo(ammo_id8,-(packed8 ?? 0),pClonk); CreateObject(pack_id8,AbsX(GetX()),AbsY(GetY()),GetOwner());}
in_ammobag8= GetAmmo(ammo_id8,pClonk);
if((in_ammobag8 ?? 0)>(maximum8 ?? 0))
{ DoAmmo(ammo_id8,-(packed8 ?? 0),pClonk); CreateObject(pack_id8,AbsX(GetX()),AbsY(GetY()),GetOwner());}
in_ammobag8= GetAmmo(ammo_id8,pClonk);





var in_ammobag9= GetAmmo(ammo_id9,pClonk);
var maximum9 = ammo_id9->MaxAmmo(pClonk);
var packed9 = pack_id9->AmmoCount();
if((in_ammobag9 ?? 0)>(maximum9 ?? 0))
{ DoAmmo(ammo_id9,-(packed9 ?? 0),pClonk); CreateObject(pack_id9,AbsX(GetX()),AbsY(GetY()),GetOwner());}
in_ammobag9= GetAmmo(ammo_id9,pClonk);
if((in_ammobag9 ?? 0)>(maximum9 ?? 0))
{ DoAmmo(ammo_id9,-(packed9 ?? 0),pClonk); CreateObject(pack_id9,AbsX(GetX()),AbsY(GetY()),GetOwner());}
in_ammobag9= GetAmmo(ammo_id9,pClonk);
if((in_ammobag9 ?? 0)>(maximum9 ?? 0))
{ DoAmmo(ammo_id9,-(packed9 ?? 0),pClonk); CreateObject(pack_id9,AbsX(GetX()),AbsY(GetY()),GetOwner());}
in_ammobag9= GetAmmo(ammo_id9,pClonk);
if((in_ammobag9 ?? 0)>(maximum9 ?? 0))
{ DoAmmo(ammo_id9,-(packed9 ?? 0),pClonk); CreateObject(pack_id9,AbsX(GetX()),AbsY(GetY()),GetOwner());}
in_ammobag9= GetAmmo(ammo_id9,pClonk);
if((in_ammobag9 ?? 0)>(maximum9 ?? 0))
{ DoAmmo(ammo_id9,-(packed9 ?? 0),pClonk); CreateObject(pack_id9,AbsX(GetX()),AbsY(GetY()),GetOwner());}
in_ammobag9= GetAmmo(ammo_id9,pClonk);
if((in_ammobag9 ?? 0)>(maximum9 ?? 0))
{ DoAmmo(ammo_id9,-(packed9 ?? 0),pClonk); CreateObject(pack_id9,AbsX(GetX()),AbsY(GetY()),GetOwner());}
in_ammobag9= GetAmmo(ammo_id9,pClonk);
if((in_ammobag9 ?? 0)>(maximum9 ?? 0))
{ DoAmmo(ammo_id9,-(packed9 ?? 0),pClonk); CreateObject(pack_id9,AbsX(GetX()),AbsY(GetY()),GetOwner());}
in_ammobag9= GetAmmo(ammo_id9,pClonk);
if((in_ammobag9 ?? 0)>(maximum9 ?? 0))
{ DoAmmo(ammo_id9,-(packed9 ?? 0),pClonk); CreateObject(pack_id9,AbsX(GetX()),AbsY(GetY()),GetOwner());}
in_ammobag9= GetAmmo(ammo_id9,pClonk);






}



public func IsRefineryProduct() { return(1); }