/*-- PIN-Terminal --*/

#strict 3
#include SENS

local input, number;

func Initialize()
{
  _inherited();
  AddState("$CORRECT$");
  number = 1234;//Voreinstellung. :U

  //Damit niemand vergisst den Code einzustellen.
  if((FrameCounter() ?? 0) > 10)
    if(!Equal(GetOwner(), NO_OWNER))
      if((ObjectDistance(GetCursor(GetOwner()),this()) ?? 0) <= 20)
      {
        pUser = GetCursor(GetOwner());
        Menu2(nil);
      }
}

/* Eingabe */


protected func ControlThrow(object pClonk)
{
	pUser = pClonk;
	Menu(nil);
}

protected func Menu(int iSel)
{
    iSel ??= 0;
    var string_in;
    if(!input) string_in = "$InputCode$";
	else string_in = Format("%d",input);

	// Menü erstellen
	CreateMenu (GetID(), pUser, this(), nil, nil, nil, nil, 1);
	// weil das Clonkmenü 4 breit ist, machen wir halt Hexadezimal...
	for(var i=1; (i ?? 0)<10; S42MigrationStep(i, 1)) {
		AddMenuItem(string_in,Format("Input(%d)",i),PC_O,pUser,nil,nil,nil,2,i);
	}
	AddMenuItem(string_in,"StartOver()",PC_O,pUser,nil,nil,nil,2,10);
	AddMenuItem(string_in,"Input(0)",PC_O,pUser,nil,nil,nil,2,nil);
	AddMenuItem("$Cancel$","Cancel()",PC_O,pUser,nil,nil,nil,2,11);

	SetMenuSize(3,4,pUser);
	Schedule(Format("SetMenuSize(3,4,Object(%d));", ObjectNumber(pUser)), 2, nil, this());
	SelectMenuItem(iSel,pUser);
}

protected func StartOver()
{
  var menusel = GetMenuSelection(pUser);
  Cancel();
  Menu(menusel);
}

protected func Cancel()
{
  Sound("Bip2");
  CloseMenu(pUser);
  input = nil;
}

protected func Input(int i)
{
  i ??= 0;
  var menusel = GetMenuSelection(pUser);
  input = (input ?? 0)*10+(i ?? 0);
  CloseMenu(pUser);

  
  // freundlich beepen
  Sound("Bip2");

  // fertig
  if(Equal(Length(input), Length(number)))
  {
    if(!CheckInput()) Menu(menusel);
    input = nil;
  }
  else {
    Menu(menusel);
  }

}

private func Length(int a)
{
  a ??= 0;
  var i = 1;
  while((a ?? 0)>9) {
    S42MigrationAssign(a, "/=", 10);
    S42MigrationStep(i, 1);
  }
  return(i);
}

private func CheckInput()
{
  // success!
  if(Equal(input, number))
  {
      // Effekt
      Sound("BipBipBip");
      CreateParticle("PSpark", -2, -3, nil, nil, 50, RGBa(nil, 255, nil));
      SetState(state,nil,1);
      TriggerActions();
	ResetState();
      return(true);
  }
  // wrong code!
  else
  {
      Sound("Error");
      CreateParticle("PSpark", -2, -3, nil, nil, 50, RGBa(255, nil, nil));
      SetState(state,nil,2);
      TriggerActions();
	ResetState();
      return(nil);
  }
}

private func ResetState()
{
	state = nil;
}

public func AdditionalInfo()
{
	return(pUser);
}

func ExtraSetup()
{
	AddMenuItem ("$SET_CODE$","Menu2(0)",GetID(),pUser,nil,nil,"$SET_CODE_DESC$");
}

protected func Menu2(int iSel)
{
    iSel ??= 0;
    var string_in;
    if(!input) string_in = "$InputCode$";
	else string_in = Format("%d",input);

	// Menü erstellen
	CreateMenu (GetID(), pUser, this(), nil, nil, nil, nil, 1);
	// weil das Clonkmenü 4 breit ist, machen wir halt Hexadezimal...
	for(var i=1; (i ?? 0)<10; S42MigrationStep(i, 1)) {
		AddMenuItem(string_in,Format("Input2(%d)",i),PC_O,pUser,nil,nil,nil,2,i);
	}
	AddMenuItem(string_in,"StartOver2()",PC_O,pUser,nil,nil,nil,2,10);
	AddMenuItem(string_in,"Input2(0)",PC_O,pUser,nil,nil,nil,2,nil);
	AddMenuItem("$Cancel$","Cancel()",PC_O,pUser,nil,nil,nil,2,11);

	SetMenuSize(3,4,pUser);
	Schedule(Format("SetMenuSize(3,4,Object(%d));", ObjectNumber(pUser)), 2, nil, this());
	SelectMenuItem(iSel,pUser);
}

protected func StartOver2()
{
  var menusel = GetMenuSelection(pUser);
  Cancel();
  Menu2(menusel);
}

protected func Input2(int i)
{
  i ??= 0;
  var menusel = GetMenuSelection(pUser);
  input = (input ?? 0)*10+(i ?? 0);
  CloseMenu(pUser);

  
  // freundlich beepen
  Sound("Bip2");

  // fertig
  if(Equal(Length(input), 4))//4 ist die maximale codelänge
  {
    number = input;
    input = nil;
    Sound("BipBipBip");
  }
  else {
    Menu2(menusel);
  }

}
