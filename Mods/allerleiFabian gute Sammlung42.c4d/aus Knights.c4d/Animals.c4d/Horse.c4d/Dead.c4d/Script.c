/*-- Totes Pferd --*/

#strict 3

local iGraphics;

/* Verwesung */

private func Decaying()
{
  DoCon(-4);
}

/* Zerlegen */

public func ControlDigDouble(pClonk) {
  [$TxtEmbowel$|Image=KNFE|Condition=GetAlive]
  // Der Clonk soll dieses Objekt zerlegen, wenn er kann
  return(ObjectSetAction(pClonk,"Embowel",this()));
}

/* Wird zerlegt */

public func Embowel(object pByClonk) {
  // Tote Pferde haben Sonderbehandlung 
  var pSkin, pBone;
  if (pSkin = CreateObject(SKIN, nil, 10)) pSkin->~SetSkinGraphics(Format("%d", LocalN("iGraphics")));
  if (pSkin = CreateObject(SKIN, nil, 10)) pSkin->~SetSkinGraphics(Format("%d", LocalN("iGraphics")));
  if (pBone = CreateObject(BBON, nil, 10)) SetName("$NameHorseBone$", pBone);
  if (pBone = CreateObject(BBON, nil, 10)) SetName("$NameHorseBone$", pBone);
  CreateObject(RMET, nil, 10);
  CreateObject(RMET, nil, 10);
  RemoveObject();
  return(1);
}
