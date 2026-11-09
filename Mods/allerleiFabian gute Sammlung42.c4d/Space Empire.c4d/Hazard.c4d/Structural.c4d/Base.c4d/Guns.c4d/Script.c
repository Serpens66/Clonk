/*-- Geschützsteuerung --*/

#strict


func IsConsoleTarget(){return(true);}

local segu,armed;


public func Initialize()
{
//Log("bin da");
}

public func ConsoleControl(int i)
{
  if(i == 1)
  {
return("$TurnOff$");
  }

if (i == 2)
{
return("$TurnOn$");
};

if (i == 3)
{
//if(armed) return("$Unarm$");
//else 
     return("$Arm$");
};


}





public func ConsoleControlled(int i)
{
    if(i == 1)
 { for(var gun in FindObjects(Find_ID(SEGU),Find_Owner(GetOwner()) ))
    gun->ConsoleControlled(1);}

    if(i == 2)
 { 
for(var gun in FindObjects(Find_ID(SEGU),Find_Owner(GetOwner()) ))
    {gun->ConsoleControlled(2);}
  }

    if(i == 3)
 { 
ChooseWeapon();
  }
}


func ChooseWeapon()
{
CreateMenu (GetID(), GetCursor(GetOwner()), 0, 0, "wähle Waffe", 0,1, 0, 0);
 
if(!armed)
AddMenuItem ("MG ausrüsten (300 C.)", "Arm", MGSA, GetCursor(GetOwner()), 0, 0, "Rüstet alle Geschütze mit einer MG aus, kostet 300 Clunker pro vorhandenes Geschütz. Nachladen kostet ebenfalls 300 Clunker"); 

if(!armed)
AddMenuItem ("Railgun ausrüsten (300 C.)", "Arm", RGSA, GetCursor(GetOwner()), 0, 0, "Rüstet alle Geschütze mit einer Railgun aus, kostet 300 Clunker pro vorhandenes Geschütz. Nachladen kostet ebenfalls 300 Clunker"); 

if(armed)
AddMenuItem ("Aktuelle Waffen entfernen", "UnArm", MGSA, GetCursor(GetOwner()), 0, 0, "Entfernt die aktuell gewählte Waffe, es gibt kein Geld zurück"); 
}

func Arm(IDWeapon)
{
for(var gun in FindObjects(Find_ID(SEGU),Find_Owner(GetOwner()) ))
    {gun->Arm(IDWeapon);armed=1;}
}

func UnArm()
{
for(var gun in FindObjects(Find_ID(SEGU),Find_Owner(GetOwner()) ))
    {gun->UnArm();armed=0;}
}


func CheckforSegu()
{
segu = FindObject2(Find_ID(SEGU),Find_Owner(GetOwner()));
if (!segu) RemoveObject();
return();
}