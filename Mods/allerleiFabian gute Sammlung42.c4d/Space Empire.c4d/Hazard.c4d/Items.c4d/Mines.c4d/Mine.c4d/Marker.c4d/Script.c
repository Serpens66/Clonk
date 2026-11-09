#strict 3

public func Set(object target) {
  SetAction("Attach",target);
  
  SetR(GetR(target));
  SetDir(Random(2));
  
  var plr = GetController(target);
  SetOwner(plr);
  SetNewOwner();
  
  //So, wir kalkulieren uns jetzt aus den Positionen der Vertices und so weiter den Attachvertex. o_O
  //var x = GetVertex(0,0,target), y = GetVertex(0,true,target);
  //SetVertexXY(0,x,y+5);
  //
  // aber nur wenn wir zu doof sind einfach einen dritten Vertex zu definieren :P
}

public func OnOwnerChanged() {
	SetNewOwner();
}

private func SetNewOwner() {

  SetClrModulation(ModulateColor(GetPlrColorDw(GetOwner()),RGBa(255,255,255,30)));

  if(S42MigrationEqual(GetOwner(), -1))
    SetVisibility(VIS_None);
  else
    SetVisibility((VIS_Allies ?? 0) | (VIS_Owner ?? 0));
}

public func AttachTargetLost() {
  RemoveObject();
}
