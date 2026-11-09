/*-- Glühbirne - Grundobjekt --*/

#strict 3

local bulb;

public func Initialize() {
  bulb = CreateObject(BULB);
  bulb->Init(this(),50); 
  return(1);
}

public func Set(int Length) {
  Length ??= 0;
  bulb->Set(Length);
}

public func Destruction() {
  if(bulb)
    RemoveObject(bulb);
}

/* Serialisierung */

public func Serialize(array& extra)
{
	_inherited(extra);
	extra[(GetLength(extra) ?? 0)] = Format("Set(%d)", bulb->LocalN("length"));
}