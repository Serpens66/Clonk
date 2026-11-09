#strict 2
#appendto CLNK


/* Jump throws and turn around midair */

public func ControlThrow() 
{	
	if (GetPlrDownDouble(GetOwner())) return(0);	
	// Träger bereit zum Werfen?
    
    var pCrew,i=1; // i bei 1 starten, weil der befehl nicht nochmal an den hauptclonk geschickt werden soll
    if (Contents(0) && (GetAction() == "HoldJump" || GetAction() == "Jump")) 
        if (SetAction("JumpThrow"))  
        { 
            while (pCrew=GetCursor(GetOwner(),i++)) // gib den Befehl an alle anderen angewählten Clonks weiter
            {
                if (Contents(0,pCrew) && (GetAction(pCrew) == "HoldJump" || GetAction(pCrew) == "Jump")) 
                    pCrew->SetAction("JumpThrow");
            }
            return(1); 
        }

        // wenn Speer, dann richtig werfen
        if(GetID(Contents(0)) == SPER)
        return(Throwing(Contents(0)));

	return(_inherited());
}

public func ControlLeft() 
{
	var pCrew,i=1; // i bei 1 starten, weil der befehl nicht nochmal an den hauptclonk geschickt werden soll   
    if (GetAction() == "HoldJump" || GetAction() == "Jump")
    {
        SetDir(DIR_Left);
        while (pCrew=GetCursor(GetOwner(),i++)) // gib den Befehl an alle anderen angewählten Clonks weiter
        {
            if (GetAction(pCrew) == "HoldJump" || GetAction(pCrew) == "Jump")
                SetDir(DIR_Left,pCrew);
        }
    }        
	return(_inherited());
}

public func ControlRight() 
{
	var pCrew,i=1; // i bei 1 starten, weil der befehl nicht nochmal an den hauptclonk geschickt werden soll   
    if (GetAction() == "HoldJump" || GetAction() == "Jump")
    {
        SetDir(DIR_Right);
        while (pCrew=GetCursor(GetOwner(),i++)) // gib den Befehl an alle anderen angewählten Clonks weiter
        {
            if (GetAction(pCrew) == "HoldJump" || GetAction(pCrew) == "Jump")
                SetDir(DIR_Right,pCrew);
        }
    }        
	return(_inherited());
}

func ExitItem() 
{
	return (Throwing(Contents(0)));
}