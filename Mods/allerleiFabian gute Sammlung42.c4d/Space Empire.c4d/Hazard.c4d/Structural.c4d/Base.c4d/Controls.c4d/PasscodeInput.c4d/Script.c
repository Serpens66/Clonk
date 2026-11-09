/*-- PIN-Terminal --*/

#strict 3

local target, input, number, clonk;

/* Eingabe */


protected func ControlThrow(object pClonk)	{
	clonk = pClonk;
	Menu(nil);
}


protected func Menu(int iSel) {
    iSel ??= 0;
    var string_in;
    if(!input) string_in = "$InputCode$";
	else string_in = Format("$Input$: %d",input);

	// Menü erstellen
	CreateMenu (GetID(), clonk, this(), nil, nil, nil, nil, 1);
	// weil das Clonkmenü 4 breit ist, machen wir halt Hexadezimal...
	for(var i=1; (i ?? 0)<10; S42MigrationStep(i, 1)) {
		AddMenuItem(string_in,Format("Input(%d)",i),PCSO,clonk,nil,nil,nil,2,i);
	}
	AddMenuItem(string_in,"StartOver()",PCSO,clonk,nil,nil,nil,2,10);
	AddMenuItem(string_in,"Input(0)",PCSO,clonk,nil,nil,nil,2,nil);
	AddMenuItem("$Cancel$","Cancel()",PCSO,clonk,nil,nil,nil,2,11);

	SetMenuSize(3,4,clonk);
	Schedule(Format("SetMenuSize(3,4,Object(%d));", ObjectNumber(clonk)), 2, nil, this());
	SelectMenuItem(iSel,clonk);
}

protected func StartOver() {
  var menusel = GetMenuSelection(clonk);
  Cancel();
  Menu(menusel);
}

protected func Cancel() {
  Sound("Bip");
  CloseMenu(clonk);
  input = nil;
}

protected func Input(int i) {
  i ??= 0;
  var menusel = GetMenuSelection(clonk);
  input = (input ?? 0)*10+(i ?? 0);
  CloseMenu(clonk);

  
  // freundlich beepen
  Sound("Bip");

  // fertig
  if(Equal(Length(input), Length(number))) {
    if(!CheckInput()) Menu(menusel);
    input = nil;
  }
  else {
    Menu(menusel);
  }

}

private func Length(int a) {
  a ??= 0;
  var i = 1;
  while((a ?? 0)>9) {
    S42MigrationAssign(a, "/=", 10);
    S42MigrationStep(i, 1);
  }
  return(i);
}

private func CheckInput() {
  // success!
  if(Equal(input, number)) {
    if(target) {
      target->~Access();

      // Effekt
      Sound("BipBipBip");
      CreateParticle("NoGravSpark", 5, 1, nil, nil, 50, RGBa(nil, 255, nil, 50));
      return(true);
    }
  }
  // wrong code!
  else {
      Sound("Error");
      CreateParticle("NoGravSpark", 5, 1, nil, nil, 50, RGBa(255, nil, nil, 50));
      return(nil);
  }
}

public func Set(object pTarget, int i)
{
  i ??= 0;
  number = i;
  target = pTarget;
}

/* Serialisierung */

public func Serialize(array& extra)
{
	if (target)
		extra[(GetLength(extra) ?? 0)] = [Format("Set(Object(%%d),%i)", number), target];
}
