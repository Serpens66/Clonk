#strict 3
#appendto CLNK


/* Jump throws and turn around midair */

public func ControlThrow() 
{	
	if (GetPlrDownDouble(GetOwner())) return(nil);
	// Träger bereit zum Werfen?
    
    var pCrew,i=1; // i bei 1 starten, weil der befehl nicht nochmal an den hauptclonk geschickt werden soll
    if (Contents(nil) && (S42MigrationEqual(GetAction(), "HoldJump") || S42MigrationEqual(GetAction(), "Jump")))
        if (SetAction("JumpThrow"))  
        { 
            while (pCrew=GetCursor(GetOwner(),S42MigrationStep(i, 1, true))) // gib den Befehl an alle anderen angewählten Clonks weiter
            {
                if (Contents(nil,pCrew) && (S42MigrationEqual(GetAction(pCrew), "HoldJump") || S42MigrationEqual(GetAction(pCrew), "Jump")))
                    pCrew->SetAction("JumpThrow");
            }
            return(1); 
        }

        // wenn Speer, dann richtig werfen
        if(S42MigrationEqual(GetID(Contents(nil)), SPER))
        return(Throwing(Contents(nil)));

	return(_inherited());
}

public func ControlLeft() 
{
	var pCrew,i=1; // i bei 1 starten, weil der befehl nicht nochmal an den hauptclonk geschickt werden soll   
    if (S42MigrationEqual(GetAction(), "HoldJump") || S42MigrationEqual(GetAction(), "Jump"))
    {
        SetDir(DIR_Left);
        while (pCrew=GetCursor(GetOwner(),S42MigrationStep(i, 1, true))) // gib den Befehl an alle anderen angewählten Clonks weiter
        {
            if (S42MigrationEqual(GetAction(pCrew), "HoldJump") || S42MigrationEqual(GetAction(pCrew), "Jump"))
                SetDir(DIR_Left,pCrew);
        }
    }        
	return(_inherited());
}

public func ControlRight() 
{
	var pCrew,i=1; // i bei 1 starten, weil der befehl nicht nochmal an den hauptclonk geschickt werden soll   
    if (S42MigrationEqual(GetAction(), "HoldJump") || S42MigrationEqual(GetAction(), "Jump"))
    {
        SetDir(DIR_Right);
        while (pCrew=GetCursor(GetOwner(),S42MigrationStep(i, 1, true))) // gib den Befehl an alle anderen angewählten Clonks weiter
        {
            if (S42MigrationEqual(GetAction(pCrew), "HoldJump") || S42MigrationEqual(GetAction(pCrew), "Jump"))
                SetDir(DIR_Right,pCrew);
        }
    }        
	return(_inherited());
}

func ExitItem() 
{
	return (Throwing(Contents(nil)));
}