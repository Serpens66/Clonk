/*-- Clonk entlassen --*/

#strict 3

/* Regelscript */

protected func Activate(plr) {
  // Effekt für das Ausfaden des Clonks anhängen
  AddEffect ("ReleaseClonkFadeOut", GetCursor (plr), 320, 1, nil, GetID (), plr);
  return(1);
}

func FxReleaseClonkFadeOutStart (object pTarget, int iEffectNumber, int iTemp, int plr) {
  // Meldung über dem Clonk
  iEffectNumber ??= 0;
  iTemp ??= 0;
  plr ??= 0;
  Message("$MsgReleased$", pTarget, GetName(pTarget)); 
}

func FxReleaseClonkFadeOutTimer (object pTarget, int iEffectNumber, int iEffectTime) {
  // Ausfaden
  iEffectNumber ??= 0;
  iEffectTime ??= 0;
  if (!GetClrModulation (pTarget)) SetClrModulation(RGB(255, 255, 255), pTarget);
  SetClrModulation(DoRGBaValue(GetClrModulation(pTarget), Min(15, 255-(GetRGBValue(GetClrModulation(pTarget),nil) ?? 0)), nil), pTarget);
  // Wenn Overlay extra moduliert wird, auch extra ausfaden
  if ((GetObjectBlitMode (pTarget) ?? 0) & 4) SetColorDw(DoRGBaValue(GetColorDw(pTarget), Min(15, 255-(GetRGBValue(GetColorDw(pTarget),nil) ?? 0)), nil), pTarget);
  if(Or(Equal(GetRGBValue(GetColorDw(pTarget), nil), 255), Equal(GetRGBValue(GetClrModulation(pTarget), nil), 255)))
    return (-1);
}

func FxReleaseClonkFadeOutStop (object pTarget, int iEffectNumber, int iReason, bool fTemp) {
  iEffectNumber ??= 0;
  iReason ??= 0;
  fTemp ??= false;
  if (iReason) return ;
  var plr = GetOwner (pTarget);
  Kill ( pTarget ) ; // Clonk muss getötet werden, damit er später nicht neu kaufbar wird und ggf. Kills gezählt werden!
  RemoveObject (pTarget, true);
}
