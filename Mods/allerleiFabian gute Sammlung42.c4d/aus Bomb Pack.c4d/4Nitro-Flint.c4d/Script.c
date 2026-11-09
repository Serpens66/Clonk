/*-- Nitro-Flint --*/

#strict 3

#include FLNT

public func ExplodeSize() 
{ 
  return(60); 
}

//protected func Hit()
//{
//  ObjectCall(CreateObject(SRAD,0,13),"SetTime", Random(1));
//   var obj; 
//  obj=CreateObject(BO5B); 
//  obj -> Bumm();
//  CreateParticle("Blast", 00,00, 00,00, Size*10+(Size/10), RGBa(128,128,255,0));
//  var Size=ExplodeSize();
//  for (var pObject in FindObjects(Find_Distance(Size/2), Find_Not(Find_Or(Find_ID(SS1C), Find_ID(STAR), Find_ID(EY3V), Find_ID(EW3V), Find_ID(EP3V), Find_ID(WP3V), Find_Category(524288), Find_Category(32)))))
//  	{
//    	if(GetCategory(pObject) & C4D_Living)
//    	if (pObject) Kill(pObject);   
//  	if(OCF_Chop)
//	if (pObject) pObject -> Incinerate();
//  	}
//  for (var pObject in FindObjects(Find_Distance(Size*2), Find_Or(Find_Category(32769), Find_ID(GRAS), Find_ID(SHEL), Find_Category(3332), Find_Category(2576), Find_Category(3600), Find_Category(2056), Find_Category(69640),  Find_Category(4104))))
//  	{
//    	if (pObject) pObject -> Incinerate();
//  	}
// weil zu mächtig wenn dann wirklich alles verbrennt, egal wie stark.
//  Explode(ExplodeSize());
//}
public func GetResearchBase() { return(EFLN); }
public func IsChemicalProduct() { return(1); }
func IsAlchemContainer() { return(true); }
