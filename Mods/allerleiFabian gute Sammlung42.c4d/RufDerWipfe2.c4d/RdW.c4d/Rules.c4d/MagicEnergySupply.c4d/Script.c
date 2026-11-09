/*-- Zauberenergie-Versorgung --*/

#strict

protected func Activate(iPlr) {
  MessageWindow(GetDesc(),iPlr);
}

func CheckMana()
{
  var pObj;
  for(pObj in FindObjects(Find_Or(Find_Func("GetPhysical","Magic"),Find_Func("NeedsMagic")),Find_Not(Find_Func("NoMagicEnergy"))))
    pObj->DoMagicEnergy(+1);
}
