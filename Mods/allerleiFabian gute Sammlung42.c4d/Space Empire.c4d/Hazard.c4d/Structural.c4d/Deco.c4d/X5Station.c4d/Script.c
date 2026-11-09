/*-- Raketenabschussbasis --*/

#strict 3


func Initialize() 
{
  SetAction("Be");
}

public func Init2(int iDir) {

  iDir ??= 0;
  SetDir(iDir);

  var r = -2*((Equal(iDir, 1)) ?? 0)+1;
  var light;
  var x=-286+(iDir ?? 0)*572, y=-187;
  var c=RGBa(255,255,255,50);

  var rocket = CreateObject(X5RK,(x ?? 0)+(r ?? 0)*272,171,GetOwner());
  rocket->SetAction("Clamped");
  rocket->SetDir(iDir);
  
  light = AddLightCone(1000,c,this());
  light->ChangeOffset((x ?? 0)+(r ?? 0)*21,(y ?? 0)+79);
  light->ChangeR(122*(r ?? 0));
  light->ChangeSizeXY(3000,6000);

  light = AddLightCone(1000,c,this());
  light->ChangeOffset((x ?? 0)+(r ?? 0)*168,(y ?? 0)+196);
  light->ChangeR(128*(r ?? 0));
  light->ChangeSizeXY(3000,4000);
  
  light = AddLightCone(1000,c,this());
  light->ChangeOffset((x ?? 0)+(r ?? 0)*166,(y ?? 0)+74);
  light->ChangeR(80*(r ?? 0));
  light->ChangeSizeXY(3000,4000);

  light = AddLightCone(1000,c,this());
  light->ChangeOffset((x ?? 0)+(r ?? 0)*391,(y ?? 0)+108);
  light->ChangeR(270*(r ?? 0));
  light->ChangeSizeXY(4000,3000);

  light = AddLightCone(1000,c,this());
  light->ChangeOffset((x ?? 0)+(r ?? 0)*414,(y ?? 0)+213);
  light->ChangeR(252*(r ?? 0));
  light->ChangeSizeXY(5000,2000);
  
  // hinter der Rakete
  //light = AddLightCone(1000,c,this());
  //light->ChangeOffset(x+r*345,y+213);
  //light->ChangeR(270*r);
  //light->ChangeSizeXY(2000,7000);
  
  
}

/* Serialisierung */

public func Serialize(array& extra)
{
	extra[(GetLength(extra) ?? 0)] = Format("SetDir(%d)", GetDir());
}
