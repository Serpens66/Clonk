/*-- Geschützsteuerung --*/

#strict 3


func IsConsoleTarget(){return(true);}

local segu,armed;


public func Initialize()
{
//Log("bin da");
}

public func ConsoleControl(int i)
{
  i ??= 0;
  if(Equal(i, 1))
  {
return("$TurnOff$");
  }

if (Equal(i, 2))
{
return("$TurnOn$");
};

if (Equal(i, 3))
{
//if(armed) return("$Unarm$");
//else 
     return("$Arm$");
};


}





public func ConsoleControlled(int i)
{
    i ??= 0;
    if(Equal(i, 1))
 { for(var gun in FindObjects(Find_ID(SEGU),Find_Owner(GetOwner()) ))
    gun->ConsoleControlled(1);}

    if(Equal(i, 2))
 { 
for(var gun in FindObjects(Find_ID(SEGU),Find_Owner(GetOwner()) ))
    {gun->ConsoleControlled(2);}
  }

    if(Equal(i, 3))
 { 
ChooseWeapon();
  }
}


func ChooseWeapon()
{
CreateMenu (GetID(), GetCursor(GetOwner()), nil, nil, "wähle Waffe", nil,1, nil, nil);
 
if(!armed)
AddMenuItem ("MG ausrüsten (300 C.)", "Arm", MGSA, GetCursor(GetOwner()), nil, nil, "Rüstet alle Geschütze mit einer MG aus, kostet 300 Clunker pro vorhandenes Geschütz. Nachladen kostet ebenfalls 300 Clunker");

if(!armed)
AddMenuItem ("Railgun ausrüsten (300 C.)", "Arm", RGSA, GetCursor(GetOwner()), nil, nil, "Rüstet alle Geschütze mit einer Railgun aus, kostet 300 Clunker pro vorhandenes Geschütz. Nachladen kostet ebenfalls 300 Clunker");

if(armed)
AddMenuItem ("Aktuelle Waffen entfernen", "UnArm", MGSA, GetCursor(GetOwner()), nil, nil, "Entfernt die aktuell gewählte Waffe, es gibt kein Geld zurück");
}

func Arm(IDWeapon)
{
for(var gun in FindObjects(Find_ID(SEGU),Find_Owner(GetOwner()) ))
    {gun->Arm(IDWeapon);armed=1;}
}

func UnArm()
{
for(var gun in FindObjects(Find_ID(SEGU),Find_Owner(GetOwner()) ))
    {gun->UnArm();armed=nil;}
}


func CheckforSegu()
{
segu = FindObject2(Find_ID(SEGU),Find_Owner(GetOwner()));
if (!segu) RemoveObject();
return;
}