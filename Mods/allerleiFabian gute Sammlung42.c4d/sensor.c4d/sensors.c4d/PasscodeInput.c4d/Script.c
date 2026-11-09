/*-- PIN-Terminal --*/

#strict
#include SENS

local input, number;

func Initialize()
{
  _inherited();
  AddState("$CORRECT$");
  number = 1234;//Voreinstellung. :U

  //Damit niemand vergisst den Code einzustellen.
  if(FrameCounter() > 10)
    if(GetOwner() != NO_OWNER)
      if(ObjectDistance(GetCursor(GetOwner()),this()) <= 20)
      {
        pUser = GetCursor(GetOwner());
        Menu2(0);
      }
}

/* Eingabe */


protected func ControlThrow(object pClonk)
{
	pUser = pClonk;
	Menu(0);
}

protected func Menu(int iSel)
{
    var string_in;
    if(!input) string_in = "$InputCode$";
	else string_in = Format("%d",input);

	// Menü erstellen
	CreateMenu (GetID(), pUser, this(), 0, 0, 0, 0, 1);
	// weil das Clonkmenü 4 breit ist, machen wir halt Hexadezimal...
	for(var i=1; i<10; ++i) {
		AddMenuItem(string_in,Format("Input(%d)",i),PC_O,pUser,0,0,0,2,i);
	}
	AddMenuItem(string_in,"StartOver()",PC_O,pUser,0,0,0,2,10);
	AddMenuItem(string_in,"Input(0)",PC_O,pUser,0,0,0,2,0);
	AddMenuItem("$Cancel$","Cancel()",PC_O,pUser,0,0,0,2,11);

	SetMenuSize(3,4,pUser);
	Schedule(Format("SetMenuSize(3,4,Object(%d));", ObjectNumber(pUser)), 2, 0, this());
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
  input = 0;
}

protected func Input(int i)
{
  var menusel = GetMenuSelection(pUser);
  input = input*10+i;
  CloseMenu(pUser);

  
  // freundlich beepen
  Sound("Bip2");

  // fertig
  if(Length(input) == Length(number))
  {
    if(!CheckInput()) Menu(menusel);
    input = 0;
  }
  else {
    Menu(menusel);
  }

}

private func Length(int a)
{
  var i = 1;
  while(a>9) {
    a /= 10;
    ++i;
  }
  return(i);
}

private func CheckInput()
{
  // success!
  if(input == number)
  {
      // Effekt
      Sound("BipBipBip");
      CreateParticle("PSpark", -2, -3, 0, 0, 50, RGBa(0, 255, 0));
      SetState(state,0,1);
      TriggerActions();
	ResetState();
      return(true);
  }
  // wrong code!
  else
  {
      Sound("Error");
      CreateParticle("PSpark", -2, -3, 0, 0, 50, RGBa(255, 0, 0));
      SetState(state,0,2);
      TriggerActions();
	ResetState();
      return(false);
  }
}

private func ResetState()
{
	state = 0;
}

public func AdditionalInfo()
{
	return(pUser);
}

func ExtraSetup()
{
	AddMenuItem ("$SET_CODE$","Menu2(0)",GetID(),pUser,0,0,"$SET_CODE_DESC$");
}

protected func Menu2(int iSel)
{
    var string_in;
    if(!input) string_in = "$InputCode$";
	else string_in = Format("%d",input);

	// Menü erstellen
	CreateMenu (GetID(), pUser, this(), 0, 0, 0, 0, 1);
	// weil das Clonkmenü 4 breit ist, machen wir halt Hexadezimal...
	for(var i=1; i<10; ++i) {
		AddMenuItem(string_in,Format("Input2(%d)",i),PC_O,pUser,0,0,0,2,i);
	}
	AddMenuItem(string_in,"StartOver2()",PC_O,pUser,0,0,0,2,10);
	AddMenuItem(string_in,"Input2(0)",PC_O,pUser,0,0,0,2,0);
	AddMenuItem("$Cancel$","Cancel()",PC_O,pUser,0,0,0,2,11);

	SetMenuSize(3,4,pUser);
	Schedule(Format("SetMenuSize(3,4,Object(%d));", ObjectNumber(pUser)), 2, 0, this());
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
  var menusel = GetMenuSelection(pUser);
  input = input*10+i;
  CloseMenu(pUser);

  
  // freundlich beepen
  Sound("Bip2");

  // fertig
  if(Length(input) == 4)//4 ist die maximale codelänge
  {
    number = input;
    input = 0;
    Sound("BipBipBip");
  }
  else {
    Menu2(menusel);
  }

}
